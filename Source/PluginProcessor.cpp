/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoteSpewAudioProcessor::NoteSpewAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    pvts(*this, nullptr, Identifier("processorValueTree"),
        {
            std::make_unique<AudioParameterBool>("C_key", "C", 0),
            std::make_unique<AudioParameterBool>("C#_key", "C#", 0),
            std::make_unique<AudioParameterBool>("D_key", "D", 0),
            std::make_unique<AudioParameterBool>("D#_key", "D#", 0),
            std::make_unique<AudioParameterBool>("E_key", "E", 0),
            std::make_unique<AudioParameterBool>("F_key", "F", 0),
            std::make_unique<AudioParameterBool>("F#_key", "F#", 0),
            std::make_unique<AudioParameterBool>("G_key", "G", 0),
            std::make_unique<AudioParameterBool>("G#_key", "G#", 0),
            std::make_unique<AudioParameterBool>("A_key", "A", 0),
            std::make_unique<AudioParameterBool>("A#_key", "A#", 0),
            std::make_unique<AudioParameterBool>("B_key", "B", 0),
        })
{
}

NoteSpewAudioProcessor::~NoteSpewAudioProcessor()
{
}

//==============================================================================
const juce::String NoteSpewAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoteSpewAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoteSpewAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoteSpewAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoteSpewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoteSpewAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoteSpewAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoteSpewAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoteSpewAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoteSpewAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoteSpewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NoteSpewAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoteSpewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NoteSpewAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }

    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        bool keyVal = pvts.getParameter(NOTE_NAMES[i] + "_key")->getValue();
        if (noteOn[i] != keyVal) {
            noteOn[i] = keyVal;
            sg.setScale(getCurrScale());
        }
    }

    bool newChord = false;
    bool isPlaying = false;
    AudioPlayHead* playHead = this->getPlayHead();
    if (playHead) {
        AudioPlayHead::CurrentPositionInfo currentPositionInfo;
        playHead->getCurrentPosition(currentPositionInfo);

        float beat = fmod((currentPositionInfo.ppqPosition / currentPositionInfo.timeSigNumerator), 1.0);
        int bar = (int)((currentPositionInfo.ppqPosition / currentPositionInfo.timeSigNumerator) / 1.0);

        if (lastBar != bar) {
            newChord = true;
            lastBar = bar;
        }

        isPlaying = currentPositionInfo.isPlaying;
    }

    if (!isPlaying || newChord) {
        for (MidiNote note : currChord) {
            auto messageOff = juce::MidiMessage::noteOff(10, (int)note, (juce::uint8)(0));
            midiMessages.addEvent(messageOff, 0);
        }
        currChord = Chord();
    }

    if (newChord) {
        degree++;
        if(degree > sg.getScale().size()) degree = 0;

        currChord = sg.getChord(degree);
        
        for (MidiNote note : currChord) {
            auto messageOn = juce::MidiMessage::noteOn(10, (int)note, (juce::uint8)(127));
            midiMessages.addEvent(messageOn, 1);
        }
    }
}

vector<string> NoteSpewAudioProcessor::getCurrScale()
{
    vector<string> validNotes;
    for (int i = 0; i < NOTES_PER_OCTAVE; i++) {
        if (pvts.getParameter(NOTE_NAMES[i] + "_key")->getValue()) {
            validNotes.push_back(NOTE_NAMES[i]);
        }
    }
    return validNotes;
}

//==============================================================================
bool NoteSpewAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoteSpewAudioProcessor::createEditor()
{
    return new NoteSpewAudioProcessorEditor (*this);
}

//==============================================================================
void NoteSpewAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NoteSpewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

AudioProcessorValueTreeState& NoteSpewAudioProcessor::getValueTreeState()
{
    return pvts;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoteSpewAudioProcessor();
}
