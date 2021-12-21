/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "BitmapSwitch.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginConstants.h"

using namespace juce;
using namespace std;

//==============================================================================
/**
*/
class NoteSpewAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NoteSpewAudioProcessorEditor (NoteSpewAudioProcessor&);
    ~NoteSpewAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoteSpewAudioProcessor& audioProcessor;

    std::vector<std::shared_ptr<AudioProcessorValueTreeState::SliderAttachment>> knobAttachments;

    void addAndAttachButton(String ID, BitmapSwitch& button, Image& img, int animSize, bool isMomentary);

    Image whiteKey = ImageCache::getFromMemory(BinaryData::white_png, BinaryData::white_pngSize);
    Image blackKey = ImageCache::getFromMemory(BinaryData::black_png, BinaryData::black_pngSize);

    vector<bool> isSharpOrFlat = {
      false, //C
      true, //C#
      false, //D
      true, //D#
      false, //E
      false, //F
      true, //F#
      false, //G
      true, //G#
      false, //A
      true, //A#
      false, //B
    };

    BitmapSwitch keyButtons[NOTES_PER_OCTAVE];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteSpewAudioProcessorEditor)
};
