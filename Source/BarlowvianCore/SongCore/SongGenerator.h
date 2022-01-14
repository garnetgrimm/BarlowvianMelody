#pragma once

#include "MidiNote.h"
#include "ChordDescriptor.h"
#include "Scale.h"

using namespace std;

class SongGenerator {
	Progression progression;
	MidiNote lowerBound = MidiNote(string("C4"));
	MidiNote upperBound = MidiNote(string("A5"));
public:
	Progression getProgression();
	RawProgression getRawProgression();
	Scale scale;
	int degree = 0;
	pair<MidiNote, MidiNote> getBounds();
	void setScale(Scale scale);
	Scale getScale(void);
	void setBounds(pair<MidiNote, MidiNote> bounds);
	void setProgression(Progression prog);
	ChordDescriptor getChord();
	Progression getMelody(int pulses, int simMin, int simMax);

	SongGenerator() {}
};