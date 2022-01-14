#pragma once

#include "MidiNote.h"

using namespace std;

struct RawScale : public vector<string> {
	vector<int> getPattern() {
		vector<int> pattern;
		string root = this->at(0);
		auto it = find(NOTE_NAMES.begin(), NOTE_NAMES.end(), root);
		int startIndex = it - NOTE_NAMES.begin();
		for (auto c : *this) {
			it = find(NOTE_NAMES.begin(), NOTE_NAMES.end(), c);
			int index = it - NOTE_NAMES.begin();
			int dist;
			if (index < startIndex) {
				dist = (NOTE_NAMES.size() - startIndex) + index + 1;
			}
			else {
				dist = index - startIndex;
			}
			pattern.push_back(dist);
		}
		return pattern;
	}
};

class Scale {
	MidiNote rootNote;
	RawScale rawScale;
	vector<int> degreeOrder;
	void setRoot(MidiNote rootNote);
public:
	int thirdsToStack = 3;
	DegreeList getDegrees();
	Chord spewChordInRange(MidiNote lowerBound, MidiNote upperBound, Chord chord);
	Chord spewScaleInRange(MidiNote lowerBound, MidiNote upperBound);
	Scale() {}
	Scale(RawScale rawNotes) : rawScale(rawNotes) {
		if(!rawScale.empty()) setRoot(MidiNote(rawScale[0] + "1"));
	}
	Scale(RawScale rawNotes, MidiNote rootNote) : rawScale(rawNotes) {
		setRoot(rootNote);
	}
	RawScale getRawScale();
	Chord getFullScale();
	MidiNote getRootNote();
	const vector<int>& getDegreeOrder();
	MidiNote operator [](int i) const;
	MidiNote& operator [](int i);
	size_t size();
};