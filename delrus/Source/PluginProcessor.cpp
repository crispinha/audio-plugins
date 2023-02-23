/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
 
//==============================================================================
DelrusAudioProcessor::DelrusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
                , params(*this, nullptr, juce::Identifier("Delrus"), {
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("d_feedback"), "Delay Feedback",
                                                                        0.f, 1.f, 0.5f),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("d_time"), "Delay Time",
                                                                        normRangeWithCentreSkew(0.01f, max_delay_in_s, max_delay_in_s/4.f), 1.f,
                                                                        attrWithStringFunction(2, "s")),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("d_drywet"), "Dry/Wet",
                                                                        0.f, 1.f, 0.5f),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_rate"), "Chorus Rate",
                                                                        normRangeWithCentreSkew(0.f, 99.999f, 20.f), 1.f,
                                                                        attrWithStringFunction(1, "hz")),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_depth"), "Chorus Depth",
                                                                        0.f, 1.f, 0.25f),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_centredelay"), "Centre Delay",
                                                                        normRangeWithCentreSkew(0.f, 99.999f, 20.f), 7.f,
                                                                        attrWithStringFunction(1, "ms")),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_feedback"), "Chorus Feedback",
                                                                        -1.f, 1.f, 0.f),
                        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("c_drywet"), "Chorus Dry/Wet",
                                                                        0.f, 1.f, 0.5f)

                })
{
    param_d_feedback = params.getRawParameterValue("d_feedback");
    param_d_time = params.getRawParameterValue("d_time");
    param_d_drywet = params.getRawParameterValue("d_drywet");
    param_c_rate = params.getRawParameterValue("c_rate");
    param_c_depth = params.getRawParameterValue("c_depth");
    param_c_centredelay = params.getRawParameterValue("c_centredelay");
    param_c_feedback = params.getRawParameterValue("c_feedback");
    param_c_drywet = params.getRawParameterValue("c_drywet");
}

DelrusAudioProcessor::~DelrusAudioProcessor()
{
}

//==============================================================================
const juce::String DelrusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelrusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelrusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelrusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelrusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelrusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelrusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelrusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelrusAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelrusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelrusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numChannels = juce::jmax (totalNumInputChannels, totalNumOutputChannels);
    
    juce::dsp::ProcessSpec spec = {sampleRate, (juce::uint32) samplesPerBlock, (juce::uint32) numChannels};
    del.prepare(spec);
    chorus.prepare(spec);
    
    time_smoother.reset(sampleRate, 0.01);
    del.setMaximumDelayInSamples(sampleRate * max_delay_in_s);
    storedSampleRate = sampleRate;
    
    buf_from_delay.setSize(numChannels, samplesPerBlock);
    
    drywetmix.prepare(spec);
    drywetmix.setWetLatency(0);
}

void DelrusAudioProcessor::reset() {
    del.reset();
    chorus.reset();
    drywetmix.reset();
}

void DelrusAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) {
    tree_changed.store(true);
}

void DelrusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelrusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelrusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numChannels = juce::jmax (totalNumInputChannels, totalNumOutputChannels);
    
    if (tree_changed.load()) {
        tree_changed.store(false);
        
    }
    
    // needs to be called every block
    chorus.setRate(*param_c_rate);
    chorus.setDepth(*param_c_depth);
    chorus.setCentreDelay(*param_c_centredelay);
    chorus.setFeedback(*param_c_feedback);
    chorus.setMix(*param_c_drywet);
    
    float fbdbck = *param_d_feedback;
    float deltime = *param_d_time;
    if (deltime != last_time) {
        last_time = deltime;
        time_smoother.setTargetValue(deltime);
    }
    float dywt = *param_d_drywet;
    
    drywetmix.pushDrySamples(buffer);
    drywetmix.setWetMixProportion(dywt);
    
    for (int channel = 0; channel < numChannels; ++channel) {
        auto chan_from_delay = buf_from_delay.getWritePointer(channel);

        for (int s = 0; s < buf_from_delay.getNumSamples(); s++) {
            chan_from_delay[s] = del.popSample(channel, storedSampleRate * time_smoother.getNextValue(), true);
        }
    }
    
    auto chorus_block = juce::dsp::AudioBlock<float> {buf_from_delay};
    auto chorus_ctx = juce::dsp::ProcessContextReplacing<float> {chorus_block};
    chorus.process(chorus_ctx);
    
    for (int channel = 0; channel < numChannels; ++channel) {
        auto* channelData = buffer.getReadPointer (channel);
        
        buffer.addFrom(channel, 0, buf_from_delay, channel, 0, buffer.getNumSamples(), fbdbck);
        
        for (int s = 0; s < buffer.getNumSamples(); s++) {
            del.pushSample(channel, channelData[s]);
        }
    }
    
    drywetmix.mixWetSamples(buf_from_delay);
    buffer = buf_from_delay;
}

//==============================================================================
bool DelrusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelrusAudioProcessor::createEditor()
{
    return new DelrusAudioProcessorEditor (*this, params);
}

//==============================================================================
void DelrusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = params.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelrusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr) {
        if (xml->hasTagName(params.state.getType())) {
            params.replaceState(juce::ValueTree::fromXml(*xml));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelrusAudioProcessor();
}
