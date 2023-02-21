/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
inline juce::NormalisableRange<float> normRangeWithCentreSkew(float lo, float hi, float centre) {
    auto range = juce::NormalisableRange<float>(lo, hi);
    range.setSkewForCentre(centre);
    return range;
}

inline juce::AudioParameterFloatAttributes attrWithStringFunction(int baseMaxChars, std::string suffix) {
    return juce::AudioParameterFloatAttributes()
        .withStringFromValueFunction([=] (auto x, auto) {
            auto s = juce::String (x, baseMaxChars);
            s.append(suffix, 8);
            return s;
        });
}

class DelrusAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
                             , public juce::ValueTree::Listener
{
public:
    constexpr static double max_delay_in_s = 5.0;
    
    juce::AudioProcessorValueTreeState params;

    std::atomic<float>* param_d_feedback;
    juce::SmoothedValue<float> time_smoother;
    float last_time = -1;
    std::atomic<float>* param_d_time;
    std::atomic<float>* param_d_drywet;
    std::atomic<float>* param_c_rate;
    std::atomic<float>* param_c_depth;
    std::atomic<float>* param_c_centredelay;
    std::atomic<float>* param_c_feedback;
    std::atomic<float>* param_c_drywet;
    
    juce::dsp::DelayLine<float> del{};
    double storedSampleRate = 0;
    juce::dsp::Chorus<float> chorus{};
    
    juce::dsp::DryWetMixer<float> drywetmix {10};
    juce::AudioBuffer<float> buf_from_delay;
    
    std::atomic<bool> tree_changed {true};
    
    
    //==============================================================================
    DelrusAudioProcessor();
    ~DelrusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelrusAudioProcessor)
};
