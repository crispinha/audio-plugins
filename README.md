# audio-plugins
This is the repo for my audio plugins built with C++/JUCE/CMake. It's structured into the following subprojects:
- cdsp: Library for common DSP algorithms, built with plain CMake so is cross-framework
- cui: Library for common JUCE UI elements and themes, built as a JUCE module
- delrus: Delay+chorus effect, with custom UI theme/layout and parameter value tree
- KorgFilter: A VA/TPT implementation of the Korg35 low pass filter, based on Pirkle's appnote. 