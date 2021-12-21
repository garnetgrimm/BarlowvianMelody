/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginConstants.h"
#include "SpewGenerator.h"

//==============================================================================
/**
*/
class NoteSpewAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NoteSpewAudioProcessor();
    ~NoteSpewAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getValueTreeState();
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState pvts;
    vector<string> getCurrScale();

    SpewGenerator sg;
    int lastBar;
    int degree = 0;
    Chord currChord;

    bool noteOn[NOTES_PER_OCTAVE];
    int lowest;
    int highest;
    int thirds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteSpewAudioProcessor)
};
