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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be

    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        Image& img = isSharpOrFlat[i] ? blackKey : whiteKey;
        addAndAttachButton(NOTE_NAMES[i] + "_key", keyButtons[i], img, img.getHeight() / 2, false);
    }
    setSize (400, 300);
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

NoteSpewAudioProcessorEditor::~NoteSpewAudioProcessorEditor()
{
}

//==============================================================================
void NoteSpewAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void NoteSpewAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int keyOffsetX = 0;

    keyOffsetX = 20;
    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        Image& img = isSharpOrFlat[i] ? blackKey : whiteKey;
        keyButtons[i].setBounds(keyOffsetX, 20, img.getWidth(), img.getHeight() / 2);
        if (!isSharpOrFlat[i]) keyOffsetX += whiteKey.getWidth() - (blackKey.getWidth() / 2);
        else keyOffsetX += (blackKey.getWidth() / 2);
    }

    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        if (isSharpOrFlat[i]) {
            keyButtons[i].toFront(false);
        }
    }
}
