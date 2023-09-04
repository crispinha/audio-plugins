# pragma once
/*
BEGIN_JUCE_MODULE_DECLARATION

ID: 		cui
vendor:		crispin
version:	0.1.0

END_JUCE_MODULE_DECLARATION
*/

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace cui {

    // Component that contains a label and slider so they can be laid out as a single item and automatically attaches to a value tree parameter
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

    // Look and feel that sets fonts/colours/label size and custom slider design to "house style",
    // can be subclassed per-plugin
	class BaseLookAndFeel : public juce::LookAndFeel_V4 {
	public:
        BaseLookAndFeel();
	    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	                           float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
	    juce::Label* createSliderTextBox(juce::Slider& slider) override;
	};

} // common