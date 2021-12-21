#pragma once

using namespace std;

static vector<string> allNotes = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

class MidiNote {
	int noteNumber = 0;
public:	
	static int nameToNum(string midiName);
	static string numToName(int midiNote);

	MidiNote(string name);
	MidiNote(int num);
	MidiNote(MidiNote& other);
	MidiNote(const MidiNote& other);

	friend std::ostream& operator << (std::ostream& outs, const MidiNote& midiNote);
	friend MidiNote operator+(const MidiNote& a, const MidiNote& b);
	friend MidiNote operator-(const MidiNote& a, const MidiNote& b);
	friend bool operator<(const MidiNote& a, const MidiNote& b);
	friend bool operator==(const MidiNote& a, const MidiNote& b);

	operator int();
};

typedef vector<string> Scale;
typedef vector<MidiNote> Chord;
typedef vector<Chord> DegreeList;

struct ScaleHandler {
	static DegreeList scaleToDegs(Scale scale, int thirds, int octave);
	static Chord spewChordInRange(MidiNote lowerBound, MidiNote upperBound, Chord chord);
	static DegreeList spewDegsInRange(MidiNote lowerBound, MidiNote upperBound, DegreeList degs);
};

class SpewGenerator {
	Scale scale;
	DegreeList degrees;
	MidiNote lowerBound = MidiNote(string("C4"));
	MidiNote upperBound = MidiNote(string("A5"));
	DegreeList spewedDegs;
	int thirdsToStack = 3;
public:
	void setScale(Scale scale);
	Scale getScale(void);
	void setBounds(pair<MidiNote, MidiNote> bounds);
	pair<MidiNote, MidiNote> getBounds();
	Chord getChord(int degree);
};