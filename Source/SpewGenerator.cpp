#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "SpewGenerator.h"

int MidiNote::nameToNum(string midiName) {
	string nameStr;
	string octStr;
	for(int i = 0; i < midiName.size(); i++) {
		if(isdigit(midiName[i])) {
			octStr += midiName[i];
		} else {
			nameStr += midiName[i];
		}
	}
	auto it = find(allNotes.begin(), allNotes.end(), nameStr);
	if(it != allNotes.end()) {
		int index = it - allNotes.begin();
		return index + (stoi(octStr) + 1) * 12;
	} else {
		return -1;
	}
}

string MidiNote::numToName(int midiNote) {
	return allNotes[midiNote % 12] + to_string((midiNote / 12) - 1);
}	

MidiNote::MidiNote(string name) {
	noteNumber = nameToNum(name);
}

MidiNote::MidiNote(int num) {
	noteNumber = num;
}

MidiNote::MidiNote(MidiNote& other) {
	noteNumber = other.noteNumber;
}

MidiNote::MidiNote(const MidiNote& other) {
	this->noteNumber = other.noteNumber;
}

std::ostream& operator <<(std::ostream& outs, const MidiNote &midiNote) {
	outs << MidiNote::numToName(midiNote.noteNumber);
	return outs;
}

MidiNote operator+(const MidiNote& a, const MidiNote& b) {
	return MidiNote(a.noteNumber + b.noteNumber);
}

MidiNote operator-(const MidiNote& a, const MidiNote& b) {
	return MidiNote(a.noteNumber - b.noteNumber);
}

bool operator<(const MidiNote& a, const MidiNote& b)
{
	return a.noteNumber < b.noteNumber;
}

bool operator==(const MidiNote& a, const MidiNote& b)
{
	return a.noteNumber == b.noteNumber;
}

MidiNote::operator int() {
	return noteNumber;
}

DegreeList ScaleHandler::scaleToDegs(Scale scale, int thirds, int octave) {
	DegreeList degs;
	for(int i = 0; i < scale.size(); i++) {
		Chord chord;
		for(int n = 0; n < thirds; n++) {
			string currName = scale[(i + n*2) % scale.size()];
			int currOct = octave + ((i + n*2) / scale.size());
			MidiNote currNote = MidiNote(currName + to_string(currOct));
			chord.push_back(currNote);
		}
		degs.push_back(chord);
	}
	return degs;
}

Chord ScaleHandler::spewChordInRange(MidiNote lowerBound, MidiNote upperBound, Chord chord) {
	Chord newChord;
	for (MidiNote note : chord) {
		int dist = 0;
		int closestLowest = lowerBound;
		if (note < lowerBound) {
			while (closestLowest % 12 != (int)note % 12) closestLowest++;
		}
		note = closestLowest;
		while ((int)note + dist < (int)upperBound) {
			newChord.push_back((int)note + dist);
			dist += 12;
		}
	}
	std::sort(newChord.begin(), newChord.end());
	return newChord;
}

DegreeList ScaleHandler::spewDegsInRange(MidiNote lowerBound, MidiNote upperBound, DegreeList degs) {
	DegreeList newDegs;
	for(Chord degree : degs) {
		Chord spew = spewChordInRange(lowerBound, upperBound, degree);
		newDegs.push_back(spew);
	}
	return newDegs;
}

Scale SpewGenerator::getScale(void) {
	return scale;
}

void SpewGenerator::setScale(Scale scale) {
	this->scale = scale;
	degrees = ScaleHandler::scaleToDegs(scale,  thirdsToStack, 1);
	spewedDegs = ScaleHandler::spewDegsInRange(lowerBound, upperBound, degrees);
}

void SpewGenerator::setBounds(pair<MidiNote, MidiNote> bounds) {
	lowerBound = bounds.first;
	upperBound = bounds.second;
	spewedDegs = ScaleHandler::spewDegsInRange(lowerBound, upperBound, degrees);
}
pair<MidiNote, MidiNote> SpewGenerator::getBounds() {
	return { lowerBound, upperBound };
}

Chord SpewGenerator::getChord(int degree) {
	if (spewedDegs.size() == 0) return {};
	return spewedDegs[degree % spewedDegs.size()];
}