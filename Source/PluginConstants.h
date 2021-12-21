#pragma once

#include <JuceHeader.h>

static constexpr int INFO_PADDING = 8;
static constexpr int TEXTBOX_SIZE = 20;
static juce::Colour INFO_COLOR = juce::Colour(0x20, 0x20, 0x20);
static juce::Font FONT(juce::Typeface::createSystemTypefaceFor(BinaryData::YaHei_otf, BinaryData::YaHei_otfSize));
static constexpr float FONT_HEIGHT = 15.0;

static constexpr int NOTES_PER_OCTAVE = 12;
static const std::string NOTE_NAMES[NOTES_PER_OCTAVE] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };