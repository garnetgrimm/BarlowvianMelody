/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "BitmapSwitch.h"
#include "BitmapKnob.h"
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
    void addAndAttachKnob(String ID, BitmapKnob& knob, Image& img, int minVal, int maxVal, Slider::SliderStyle style);

    Image whiteKey = ImageCache::getFromMemory(BinaryData::white_png, BinaryData::white_pngSize);
    Image blackKey = ImageCache::getFromMemory(BinaryData::black_png, BinaryData::black_pngSize);
    Image knobImg = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    Image backgroundImg = ImageCache::getFromMemory(BinaryData::outline_png, BinaryData::outline_pngSize);

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

    BitmapKnob phraseStratKnob;
    BitmapKnob phraseDensityKnob;
    BitmapKnob phraseCountKnob;

    BitmapKnob melodicStratKnob;
    BitmapKnob melodicNoteMinKnob;
    BitmapKnob melodicNoteMaxKnob;

    //move outlining chord forward
    BitmapKnob melodicAnticipationKnob;
    //move rhythm forward
    BitmapKnob rhythmicAnticipationKnob;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteSpewAudioProcessorEditor)
};
