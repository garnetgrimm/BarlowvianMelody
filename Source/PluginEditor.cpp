/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoteSpewAudioProcessorEditor::NoteSpewAudioProcessorEditor (NoteSpewAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(506, 643);

    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        Image& img = isSharpOrFlat[i] ? blackKey : whiteKey;
        addAndAttachButton(NOTE_NAMES[i] + "_key", keyButtons[i], img, img.getHeight() / 2, false);
    }

    addAndAttachKnob("phraseStrat", phraseStratKnob, knobImg, 0, 100, Slider::Rotary);
    addAndAttachKnob("phraseDensity", phraseDensityKnob, knobImg, 0, 100, Slider::Rotary);
    addAndAttachKnob("phraseCount", phraseCountKnob, knobImg, 0, 100, Slider::Rotary);

    addAndAttachKnob("melodicStrat", melodicStratKnob, knobImg, 0, 100, Slider::Rotary);
    addAndAttachKnob("melodicNoteMin", melodicNoteMinKnob, knobImg, 0, 100, Slider::Rotary);
    addAndAttachKnob("melodicNoteMax", melodicNoteMaxKnob, knobImg, 0, 100, Slider::Rotary);

    addAndAttachKnob("melodicAnticipation", melodicAnticipationKnob, knobImg, 0, 100, Slider::Rotary);
    addAndAttachKnob("rhythmicAnticipation", rhythmicAnticipationKnob, knobImg, 0, 100, Slider::Rotary);
}

void NoteSpewAudioProcessorEditor::addAndAttachButton(String ID, BitmapSwitch& button, Image& img, int animSize, bool isMomentary) {
    addAndMakeVisible(button);
    button.isMomentary = isMomentary;
    button.setRange(0, 1);
    String name = audioProcessor.getValueTreeState().getParameter(ID)->getName(20);
    button.setState(img, animSize, name.toStdString());
    std::shared_ptr<AudioProcessorValueTreeState::SliderAttachment> attachment;
    attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getValueTreeState(), ID, button));
    knobAttachments.push_back(attachment);
}

void NoteSpewAudioProcessorEditor::addAndAttachKnob(String ID, BitmapKnob& knob, Image& img, int minVal, int maxVal, Slider::SliderStyle style) {
    addAndMakeVisible(knob);
    String name = audioProcessor.getValueTreeState().getParameter(ID)->getName(20);
    knob.setRange(minVal, maxVal);
    knob.setSliderStyle(style);
    knob.setBitmap(img, name.toStdString());
    std::shared_ptr<AudioProcessorValueTreeState::SliderAttachment> attachment;
    attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getValueTreeState(), ID, knob));
    knobAttachments.push_back(attachment);
}

NoteSpewAudioProcessorEditor::~NoteSpewAudioProcessorEditor()
{
}

//==============================================================================
void NoteSpewAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.drawImageAt(backgroundImg, 0, 0);

    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        if (isSharpOrFlat[i]) {
            keyButtons[i].toFront(false);
        }
    }
}

void NoteSpewAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int keyBaseX = 265;
    int keyBaseY = 175;

    int keyOffsetX = 0;
    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        Image& img = isSharpOrFlat[i] ? blackKey : whiteKey;
        keyButtons[i].setBounds(keyBaseX + keyOffsetX, keyBaseY, img.getWidth(), img.getHeight() / 2);
        if (!isSharpOrFlat[i]) keyOffsetX += whiteKey.getWidth() - (blackKey.getWidth() / 2);
        else keyOffsetX += (blackKey.getWidth() / 2);
    }

    int knobSpaceX = 80;
    int knobSpaceY = 113;

    int knobBaseX = 30;
    int knobBaseY = 195;

    phraseStratKnob.setXY(knobBaseX, knobBaseY);
    phraseDensityKnob.setXY(knobBaseX + knobSpaceX, knobBaseY);
    phraseCountKnob.setXY(knobBaseX + knobSpaceX*2, knobBaseY);

    melodicStratKnob.setXY(knobBaseX, knobBaseY + knobSpaceY);
    melodicNoteMinKnob.setXY(knobBaseX + knobSpaceX, knobBaseY + knobSpaceY);
    melodicNoteMaxKnob.setXY(knobBaseX + knobSpaceX*2, knobBaseY + knobSpaceY);

    melodicAnticipationKnob.setXY(knobBaseX + knobSpaceX / 2, knobBaseY + knobSpaceY * 2);
    rhythmicAnticipationKnob.setXY(knobBaseX + knobSpaceX / 2 + knobSpaceX, knobBaseY + knobSpaceY*2);
}
