#include "cui.h"

#include <memory>

namespace cui {
    void CustomKnob::setup(juce::AudioProcessorValueTreeState& plug_params, const juce::String& param_id, bool skew,
                           double skew_midpoint) {
        auto param = plug_params.getParameter(param_id);

        knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 140, 20);
        if (skew) {
            knob.setSkewFactorFromMidPoint(skew_midpoint);
        }
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(plug_params, param_id, knob);
        label.setText(param->getName(30), juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
        addAndMakeVisible(knob);
    }

    void CustomKnob::paint(juce::Graphics& g) {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void CustomKnob::resized() {
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        auto width = (120.f / 700.f) * static_cast<float>(getTopLevelComponent()->getWidth());
        auto text_height = (20.f / 700.f) * static_cast<float>(getTopLevelComponent()->getWidth());
        auto font_height = (14.f / 700.f) * static_cast<float>(getTopLevelComponent()->getWidth());

        knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
                             static_cast<int>(width / 3), static_cast<int>(text_height));
        // resize font based on size in document, assuming font creation is expensive and font setting is cheap
        label_font.setHeight(font_height);
        label.setFont(label_font);

        fb.items.add(juce::FlexItem(label).withWidth(width).withHeight(text_height));
        fb.items.add(juce::FlexItem(knob).withHeight(width).withMaxWidth(width));

        fb.performLayout(getLocalBounds());
    }

    BaseLookAndFeel::BaseLookAndFeel() {
	    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour{255, 255, 255});
	    setColour(juce::Label::textColourId , juce::Colour{0, 0, 0});
	    setColour(juce::Slider::backgroundColourId, juce::Colour{128, 128, 128});
	    setColour(juce::Slider::thumbColourId, juce::Colour{0, 0, 0});
	    setDefaultSansSerifTypefaceName("Courier New");
	    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Courier New");
	}

	// code derived from https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
	void BaseLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	                                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) {
	    g.saveState();
	    
	    auto radius = (float) juce::jmin(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f) - 4.f;
	    auto centre_x = (float) x + (float) width * 0.5f;
	    auto centre_y = (float) y + (float) height * 0.5f;
	    auto rx = centre_x - radius;
	    auto ry = centre_y - radius;
	    auto diameter = radius * 2.f;
	    auto angle =  rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // draw knob base
	    g.setColour(findColour(juce::Slider::backgroundColourId));
	    g.fillEllipse(rx, ry, diameter, diameter);

        // draw pointer
	    juce::Path p;
	    auto pointer_len = radius * 1.1f;
	    auto pointer_width = radius * 0.1f;
	    p.addRoundedRectangle(-pointer_width * 0.5f, -radius - (radius / 10.f), pointer_width, pointer_len, radius / 30.f);
	    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre_x, centre_y));
	    g.setColour(findColour(juce::Slider::thumbColourId));
	    g.fillPath(p);
	    
	    g.restoreState();
	}

	juce::Label* BaseLookAndFeel::createSliderTextBox (juce::Slider& slider) {
	    auto* l = LookAndFeel_V2::createSliderTextBox(slider);
	    l->setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);
	    l->setMinimumHorizontalScale(0.001f);
	    return l;
	}
}