#pragma once

#include <random>
#include <vector>
#include "RhythmAction.h"

using namespace std;

typedef shared_ptr<RhythmAction> ActionPointer;

static random_device rd;
static mt19937 gen = mt19937(rd());

class Sequence {
	vector<int> pulses;
	vector<int> mods;
	vector<int> modPulses;
	float syncopation;
public:
	vector<ActionPointer> actionVector;
	int offset;
	int maxSkip;
	int maxPulses;

	Sequence();
	Sequence(const Sequence& other);

	float getSyncopation();
	void setSyncopation(float amount);

	void genPulses(vector<int>& invPhi);
	void genMods(vector<int>& phi);
	void genModPulses(vector<int>& phi);

	void setPulses(vector<int> pulses);
	void setMods(vector<int> mods);
	void setModPulses(vector<int> modPulses);

	vector<int> rotateVector(vector<int> v);
	vector<int> getModPulses(bool rotate);
	vector<int> getMods(bool rotate);
	vector<int> getPulses(bool rotate);
};

class RhythmGenerator {
	vector<int> inverseVector(vector<int>& v);
public:
	vector<int> phi;
	vector<int> invPhi;
	int numerator = 0;
	int stratification = 0;

	void setSignature(int num, int strat);
	void newSequence(Sequence& sequence);
};
