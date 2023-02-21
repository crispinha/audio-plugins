/*
  ==============================================================================

    CustomKnob.cpp
    Created: 17 Jan 2023 11:03:17pm
    Author:  Crispin

  ==============================================================================
*/

#include "CustomKnob.h"
#include "PluginEditor.h"

void CustomKnob::setup(juce::AudioProcessorValueTreeState& plug_params, const juce::String& param_id, bool skew, double skew_midpoint) {
    auto param = plug_params.getParameter(param_id);
        
    knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 140, 20);
    if (skew) {
        knob.setSkewFactorFromMidPoint(skew_midpoint);
    }
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(plug_params, param_id, knob));
    label.setText(param->getName(30), juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
    addAndMakeVisible(knob);
}

void CustomKnob::paint(juce::Graphics &g) {
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void CustomKnob::resized() {
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    
    auto width = (120.f/700.f) * getTopLevelComponent()->getWidth();
    auto text_height = (20.f/700.f) * getTopLevelComponent()->getWidth();
    auto font_height = (14.f/700.f) * getTopLevelComponent()->getWidth();
    
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, width/3, text_height);
    // assuming font creation is spenny and font setting is cheap
    label_font.setHeight(font_height);
    label.setFont(label_font);
    
    fb.items.add(juce::FlexItem(label).withWidth(width).withHeight(text_height));
    fb.items.add(juce::FlexItem(knob).withHeight(width).withMaxWidth(width));
    
    fb.performLayout(getLocalBounds());
}
