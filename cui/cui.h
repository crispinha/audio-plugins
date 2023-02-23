# pragma once
/*
BEGIN_JUCE_MODULE_DECLARATION

ID: 		cui
vendor:		crispin
version:	0.1.0

END_JUCE_MODULE_DECLARATION
*/

//#include <JuceHeader.h>
//#include "cui/CustomKnob.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>


namespace cui {

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

	class BaseLookAndFeel : public juce::LookAndFeel_V4 {
	public:
        BaseLookAndFeel();
	    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
	    juce::Label* createSliderTextBox(juce::Slider& slider) override;
	};

} // common