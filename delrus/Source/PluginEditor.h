/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "cui/cui.h"
#include "DelrusLookAndFeel.h"


//#define INSP

#ifdef INSP
#include "melatonin_inspector/melatonin_inspector.h"
#endif

//==============================================================================

class DelrusAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // value tree is passed to sub-panels and they pass it to components to attach to knobs
    DelrusAudioProcessorEditor (DelrusAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~DelrusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    // flexbox-based layout
    void resized() override;
    
private:
    // take a nested/tree approach - the Edior has two panels, which have individual controls and labels

    juce::ComponentBoundsConstrainer resize_constraints;
    DelrusLookAndFeel look_and_feel;

    DelrusAudioProcessor& audioProcessor;

    // left-hand "Delay" params
    struct DelayPanel : public juce::Component {
        const float base_font_size = 132;
        juce::Label l {"", "Delay"};
        juce::Font l_font {base_font_size};
        
        cui::CustomKnob knob_d_feedback;
        cui::CustomKnob knob_d_time;
        cui::CustomKnob knob_d_drywet;
        
        void setup(juce::AudioProcessorValueTreeState& plug_params);
        void paint(juce::Graphics& g) override;
        void resized() override;
    };

    // right hand "Chorus" params
    struct ChorusPanel : public juce::Component {
        const float base_font_size = 40;
        juce::Label l {"", "Cho\nrus"};
        juce::Font l_font {base_font_size};

        cui::CustomKnob knob_c_rate;
        cui::CustomKnob knob_c_depth;
        cui::CustomKnob knob_c_centredelay;
        cui::CustomKnob knob_c_feedback;
        cui::CustomKnob knob_c_drywet;
        
        void setup(juce::AudioProcessorValueTreeState& plug_params);
        void paint(juce::Graphics& g) override;
        void resized() override;
    };
    
    DelayPanel  delay_panel;
    ChorusPanel chorus_panel;
    
#ifdef INSP
    melatonin::Inspector inspector { *this };
    juce::TextButton debug_button {"Debug"};
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelrusAudioProcessorEditor)
};
