#include "Scale.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

DegreeList Scale::getDegrees() {
	DegreeList degs;
	for (int i = 0; i < rawScale.size(); i++) {
		Chord chord;
		set<MidiNote> noteSet;
		for (int n = 0; n < thirdsToStack; n++) {
			string currName = rawScale[(i + n * 2) % rawScale.size()];
			int currOct = (((int)rootNote + i + n * 2) / 12);
			MidiNote currNote = MidiNote(currName + to_string(currOct));

			if (!noteSet.count(currNote)) {
				chord.push_back(currNote);
				noteSet.insert(currNote);
			}
		}
		degs.push_back(chord);
	}
	return degs;
}

Chord Scale::spewChordInRange(MidiNote lowerBound, MidiNote upperBound, Chord chord) {
	Chord newChord;
	for (MidiNote note : chord) {
		int dist = 0;
		MidiNote minNote = MidiNote::findFirstGreater(lowerBound, note.getRawName());
		while ((int)minNote + dist < (int)upperBound) {
			newChord.push_back((int)minNote + dist);
			dist += 12;
		}
	}
	std::sort(newChord.begin(), newChord.end());
	return newChord;
}

Chord Scale::spewScaleInRange(MidiNote lowerBound, MidiNote upperBound) {
	set<MidiNote> fullSpew;
	for (string note : rawScale) {
		Chord unichord = { MidiNote(note + "1") };
		Chord spew = spewChordInRange(lowerBound, upperBound, unichord);
		for (MidiNote note : spew) {
			fullSpew.insert(note);
		}
	}
	return { fullSpew.begin(), fullSpew.end() };
}

RawScale Scale::getRawScale() {
	return rawScale;
}

Chord Scale::getFullScale() {
	return spewScaleInRange(0, 127);
}

MidiNote Scale::getRootNote() {
	return rootNote;
}

MidiNote& Scale::operator [](int i) {
	vector<int> pattern = rawScale.getPattern();
	int octInc = i / rawScale.size();
	string noteType = rawScale[i % rawScale.size()];
	MidiNote firstG = MidiNote::findFirstGreater(rootNote, noteType);
	MidiNote octs(NOTES_PER_OCTAVE * octInc);
	MidiNote f = firstG + octs;
	return f;
}

size_t Scale::size() {
	return rawScale.size();
}

const vector<int>& Scale::getDegreeOrder() {
	return degreeOrder;
}

void Scale::setRoot(MidiNote rootNote) {
	this->rootNote = rootNote;

	//root has to be in scale
	if (std::find(rawScale.begin(), rawScale.end(), rootNote.getRawName()) == rawScale.end()); {
		//TODO - better error handling
		int z = 10;
	}

	degreeOrder.resize(NOTES_PER_OCTAVE, -1);
	std::sort(rawScale.begin(), rawScale.end());

	int scaleRootIndex;
	auto it = std::find(rawScale.begin(), rawScale.end(), rootNote.getRawName());
	scaleRootIndex = it - rawScale.begin();

	std::rotate(rawScale.begin(), rawScale.begin() + scaleRootIndex, rawScale.end());

	int notesSoFar = 0;
	for (int i = 0; i < rawScale.size(); i++) {
		//all notes have to be real notes
		//TODO - better error handling
		auto it = std::find(NOTE_NAMES.begin(), NOTE_NAMES.end(), rawScale[i]);
		if (it == NOTE_NAMES.end()) throw 123;
		degreeOrder[it - NOTE_NAMES.begin()] = notesSoFar++;
	}
}