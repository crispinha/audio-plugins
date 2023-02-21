/*
  ==============================================================================

    CustomKnob.h
    Created: 17 Jan 2023 11:03:17pm
    Author:  Crispin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomKnob : public juce::Component {
public:
    juce::Slider knob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    juce::Font label_font;
    juce::Label label;
    
    CustomKnob() {
        label.setFont(label_font);
    }
    
    void setup(juce::AudioProcessorValueTreeState& plug_params, const juce::String& param_id, bool skew = false, double skew_midpoint = -1.0);
    void paint(juce::Graphics& g) override;
    void resized() override;
};
