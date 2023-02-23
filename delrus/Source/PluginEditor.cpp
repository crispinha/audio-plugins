/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const int base_w = 700, base_h = 400;

//==============================================================================
DelrusAudioProcessorEditor::DelrusAudioProcessorEditor (DelrusAudioProcessor& p, juce::AudioProcessorValueTreeState& pts)
    : AudioProcessorEditor (&p), audioProcessor (p), plug_params(pts)
{
    setName("DelrusAudioProcessorEditor");
    setLookAndFeel(&look_and_feel);
        
    delay_panel.setup(plug_params);
    chorus_panel.setup(plug_params);
    
    addAndMakeVisible(delay_panel);
    addAndMakeVisible(chorus_panel);
    
    setResizable(true, true);
    constraints.setFixedAspectRatio(((double) base_w)/((double) base_h));
    constraints.setMinimumSize(base_w * (9.1f/10.f), base_h * (9.1f/10.f));
    setConstrainer(&constraints);
    setSize (base_w, base_h);

#ifdef INSP
    addAndMakeVisible(btn);
    btn.onClick = [this]{
        inspector.setVisible(true);
    };
#endif
}

DelrusAudioProcessorEditor::~DelrusAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

//==============================================================================
void DelrusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DelrusAudioProcessorEditor::resized()
{
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::stretch;
    
    fb.items = {
        juce::FlexItem(delay_panel).withWidth(getWidth()/2.f).withHeight(getHeight()).withMargin(0),
        juce::FlexItem(chorus_panel).withWidth(getWidth()/2.f).withHeight(getHeight()).withMargin(0)
    };
    
    fb.performLayout(getLocalBounds().toFloat());
    
#ifdef INSP
    btn.setBounds(20, 20, 50, 20);
#endif
}

#define GRIDITEM(inner) (juce::GridItem((inner)))

void DelrusAudioProcessorEditor::DelayPanel::setup(juce::AudioProcessorValueTreeState& plug_params) {
    setName("DelayPanel");
    l.setJustificationType(juce::Justification::Flags::centredLeft);
    l.setFont(l_font);
    l.setBorderSize(juce::BorderSize<int>{20});
    
    knob_d_feedback.setup(plug_params, "d_feedback");
    knob_d_time.setup(plug_params, "d_time");
    knob_d_drywet.setup(plug_params, "d_drywet");
    
    addAndMakeVisible(l);
    addAndMakeVisible(knob_d_feedback);
    addAndMakeVisible(knob_d_time);
    addAndMakeVisible(knob_d_drywet);
}

void DelrusAudioProcessorEditor::DelayPanel::paint(juce::Graphics &g) { 
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DelrusAudioProcessorEditor::DelayPanel::resized() {
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    
    auto font_height = (base_font_size/700.f) * getTopLevelComponent()->getWidth();
    l_font.setHeight(font_height);
    l.setFont(l_font);

    juce::Grid del_grid;
    del_grid.templateRows = { Track (Fr (1)), Track (Fr (1))};
    del_grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    
    del_grid.items = {
        juce::GridItem(l).withColumn({1, 4}),
        GRIDITEM(knob_d_feedback), GRIDITEM(knob_d_time), GRIDITEM(knob_d_drywet)
    };
    
    del_grid.performLayout(getLocalBounds());
}

void DelrusAudioProcessorEditor::ChorusPanel::setup(juce::AudioProcessorValueTreeState& plug_params) {
    setName("ChorusPanel");
    l.setJustificationType(juce::Justification::Flags::centredLeft);
    l.setFont(l_font);
    l.setBorderSize(juce::BorderSize<int>{20});
    
    knob_c_rate.setup(plug_params, "c_rate");
    knob_c_depth.setup(plug_params, "c_depth");
    knob_c_centredelay.setup(plug_params, "c_centredelay");
    knob_c_feedback.setup(plug_params, "c_feedback");
    knob_c_drywet.setup(plug_params, "c_drywet");
    
    addAndMakeVisible(l);
    addAndMakeVisible(knob_c_rate);
    addAndMakeVisible(knob_c_depth);
    addAndMakeVisible(knob_c_centredelay);
    addAndMakeVisible(knob_c_feedback);
    addAndMakeVisible(knob_c_drywet);
}

void DelrusAudioProcessorEditor::ChorusPanel::paint(juce::Graphics &g) {
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DelrusAudioProcessorEditor::ChorusPanel::resized() {
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    
    auto font_height = (base_font_size/700.f) * getTopLevelComponent()->getWidth();
    l_font.setHeight(font_height);
    l.setFont(l_font);
    
    juce::Grid chorus_grid;
    chorus_grid.templateRows = { Track (Fr (1)), Track (Fr (1))};
    chorus_grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    chorus_grid.items = {
        GRIDITEM(l),                        GRIDITEM(knob_c_rate),       GRIDITEM(knob_c_depth),
        GRIDITEM(knob_c_centredelay),       GRIDITEM(knob_c_feedback),   GRIDITEM(knob_c_drywet)
    };
    chorus_grid.performLayout(getLocalBounds());
}
