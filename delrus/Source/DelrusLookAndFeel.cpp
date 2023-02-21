/*
  ==============================================================================

    DelrusLookAndFeel.cpp
    Created: 21 Jan 2023 11:15:51pm
    Author:  Crispin

  ==============================================================================
*/

#include "DelrusLookAndFeel.h"

DelrusLookAndFeel::DelrusLookAndFeel() {
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour{140, 12, 97});
    setColour(juce::Slider::backgroundColourId, juce::Colour{89, 8, 62});
    setColour(juce::Slider::thumbColourId, juce::Colour{106, 213, 217});
    setDefaultSansSerifTypefaceName("Courier New");
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Courier New");
}

// modified from tutorial
void DelrusLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) {
    g.saveState();
    
    auto radius = (float) juce::jmin(width / 2.f, height / 2.f) - 4.f;
    auto centre_x = (float) x + (float) width * 0.5f;
    auto centre_y = (float) y + (float) height * 0.5f;
    auto rx = centre_x - radius;
    auto ry = centre_y - radius;
    auto rw = radius * 2.f;
    auto angle =  rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    g.setColour(findColour(juce::Slider::backgroundColourId));
    g.fillEllipse(rx, ry, rw, rw);
        
    juce::Path p;
    auto pointer_len = radius * 1.1f;
    auto pointer_thicc = radius * 0.1f;
    p.addRoundedRectangle(-pointer_thicc * 0.5f, -radius - (radius / 10.f), pointer_thicc, pointer_len, radius / 30.f);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre_x, centre_y));
    g.setColour(findColour(juce::Slider::thumbColourId));
    g.fillPath(p);
    
    g.restoreState();
}

juce::Label* DelrusLookAndFeel::createSliderTextBox (juce::Slider& slider) {
    auto* l = LookAndFeel_V2::createSliderTextBox(slider);
    l->setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);
    l->setMinimumHorizontalScale(0.001f);
    return l;
}

