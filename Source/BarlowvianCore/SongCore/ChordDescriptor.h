#pragma once

#include "MidiNote.h"
#include "Scale.h"
#include <string>

class ChordDescriptor : public Chord {
	enum modType {
		INVALID,
		SHARP,
		FLAT,
		ADD,
		OMIT
	};
	static int seekInt(string str, int& index);
	static pair<modType, int> seekMod(string str, int& index);
	string name;
public:
	static bool isValid(string chordStr, Scale scale);
	string getName();
	ChordDescriptor(string chordStr, Scale scale);
	ChordDescriptor(Chord chord, string name);
};

typedef vector<string> RawProgression;
typedef vector<ChordDescriptor> Progression;