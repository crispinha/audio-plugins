/*
  ==============================================================================

    DelrusLookAndFeel.h
    Created: 21 Jan 2023 11:15:51pm
    Author:  Crispin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DelrusLookAndFeel : public cui::BaseLookAndFeel {
 public:
    DelrusLookAndFeel() {
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour{140, 12, 97});
        setColour(juce::Label::textColourId , juce::Colour{255, 255, 255});
        setColour(juce::Slider::backgroundColourId, juce::Colour{89, 8, 62});
        setColour(juce::Slider::thumbColourId, juce::Colour{106, 213, 217});
    }
};
