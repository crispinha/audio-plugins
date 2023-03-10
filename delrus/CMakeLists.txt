cmake_minimum_required(VERSION 3.15)

project(delrus VERSION 0.1.0)

add_subdirectory(../JUCE build)                    # If you've put JUCE in a subdirectory called JUCE

juce_add_plugin(Delrus
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
     COMPANY_NAME "crispin-demos"                          # Specify the name of the plugin's author
    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Chit               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dlrs                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Delrus")        # The name of the final executable, which can differ from the target name

juce_generate_juce_header(Delrus)


#add_subdirectory(../common Builds)

juce_add_module(../cui)
target_link_libraries(Delrus PUBLIC cui)

juce_add_module(modules/melatonin_inspector)
target_link_libraries(Delrus PUBLIC melatonin_inspector)

target_sources(Delrus
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        )


target_compile_definitions(Delrus
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(Delrus SOURCES ...)

target_link_libraries(Delrus
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
IF(BUILD_TYPE MATCHES DEBUG)
    juce_enable_copy_plugin_step(Delrus)
ENDIF()
