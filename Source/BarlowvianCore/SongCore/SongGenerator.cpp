#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include "SongGenerator.h"

Scale SongGenerator::getScale(void) {
	return scale;
}

void SongGenerator::setProgression(Progression prog) {
	Progression fullProg;
	for (auto chord : prog) {
		ChordDescriptor rebuiltChord = ChordDescriptor(chord.getName(), scale);
		fullProg.push_back({ scale.spewChordInRange(lowerBound, upperBound, rebuiltChord), rebuiltChord.getName() });
	}
	progression = fullProg;
}

void SongGenerator::setScale(Scale scale) {
	this->scale = scale;
	setProgression(progression);
}

void SongGenerator::setBounds(pair<MidiNote, MidiNote> bounds) {
	lowerBound = bounds.first;
	upperBound = bounds.second;
	setProgression(progression);
}

pair<MidiNote, MidiNote> SongGenerator::getBounds() {
	return { lowerBound, upperBound };
}

ChordDescriptor SongGenerator::getChord() {
	if (progression.size() == 0) return ChordDescriptor({ {}, ""});
	return progression[degree % progression.size()];
}

Progression SongGenerator::getMelody(int pulses, int simMin, int simMax) {
	ChordDescriptor legalNotes = getChord();
	if (legalNotes.size() == 0) return {};
	Progression melody;
	float randFloat;
	for (int i = 0; i < pulses; i++) {
		Chord chord;
		randFloat = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		int numNotes = (randFloat * (simMax - simMin)) + simMin;
		for (int n = 0; n < numNotes; n++) {
			randFloat = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			float nextNoteFlt = static_cast<float>(legalNotes.size() - 1) * randFloat;
			int nextNoteIdx = std::min((int)nextNoteFlt, (int)legalNotes.size() - 1);
			chord.push_back(legalNotes[nextNoteIdx]);
		}
		melody.push_back(ChordDescriptor(chord, legalNotes.getName()));
	}

	return melody;
}

Progression SongGenerator::getProgression() {
	return progression;
}

RawProgression SongGenerator::getRawProgression() {
	RawProgression progVec;
	for (ChordDescriptor& chord : progression) {
		progVec.push_back(chord.getName());
	}
	return progVec;
}