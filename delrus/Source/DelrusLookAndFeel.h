/*
  ==============================================================================

    DelrusLookAndFeel.h
    Created: 21 Jan 2023 11:15:51pm
    Author:  Crispin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DelrusLookAndFeel : public juce::LookAndFeel_V4 {
public:
    DelrusLookAndFeel();
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
};
