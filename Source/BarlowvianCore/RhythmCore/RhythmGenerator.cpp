#include "RhythmGenerator.h"
#include "BarlowAnalysis.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <type_traits>
#include <vector>

void Sequence::genPulses(vector<int>& invPhi) {
	pulses.clear();
	pulses.resize(invPhi.size(), 0);
	uniform_int_distribution<int> distr(1, maxSkip);
	int i = 0;
	for (int pulse = 0; pulse < maxPulses; pulse++) {
		int pulseIdx = 0;
		i--;
		do {
			i++;
			pulseIdx = invPhi[invPhi.size() - 1 - i % invPhi.size()];
		} while (pulses[pulseIdx] == 1);
		pulses[pulseIdx] = 1;
		i += distr(gen);
	}
}

void Sequence::genMods(vector<int>& phi) {
	mods.clear();
	uniform_real_distribution<> distr(0.0, 1.0);
	mods.resize(phi.size(), 0);
	for (int i = 0; i < phi.size(); i++) {
		int action = 0;
		float target = distr(gen);
		float current = 0.0f;
		for (int n = 0; n < actionVector.size(); n++) {
			current += actionVector[n]->prob;
			if (current > target) {
				action = n;
				break;
			}
		}
		mods[i] = action;
	}
}

void Sequence::genModPulses(vector<int>& phi) {
	modPulses.clear();
	modPulses = pulses;
	for (int i = 0; i < phi.size(); i++) {
		if(pulses[i] != 0) actionVector[mods[i]]->apply(i, phi, modPulses);
	}
}

vector<int> Sequence::rotateVector(vector<int> v) {
	if (v.size() == 0) return vector<int>();
	int local_offset = -offset % v.size();
	std::rotate(v.begin(), v.begin() + local_offset, v.end());
	return v;
}

void Sequence::setPulses(vector<int> pulses) {
	this->pulses = pulses;
}

void Sequence::setMods(vector<int> mods) {
	this->mods = mods;
}

void Sequence::setModPulses(vector<int> modPulses) {
	this->modPulses = modPulses;
}

vector<int> Sequence::getModPulses(bool rotate) {
	if (rotate) return rotateVector(modPulses);
	else return modPulses;
}

vector<int> Sequence::getMods(bool rotate) {
	if (rotate) return rotateVector(mods);
	else return mods;
}

vector<int> Sequence::getPulses(bool rotate) {
	if (rotate) return rotateVector(pulses);
	else return pulses;
}

Sequence::Sequence() {
	actionVector = {
		ActionPointer(new ActionNoOp(1.0))
	};

	maxPulses = 1;
	maxSkip = 1;
	offset = 0;
}

Sequence::Sequence(const Sequence& other) {
	pulses = other.pulses;
	mods = other.mods;
	modPulses = other.modPulses;
	actionVector = other.actionVector;
	maxPulses = other.maxPulses;
	maxSkip = other.maxSkip;
	offset = other.offset;
}

float Sequence::getSyncopation() {
	return syncopation;
}

void Sequence::setSyncopation(float amount) {
	vector<ActionPointer> newActionVector = {};

	if (amount > 0) {
		newActionVector.push_back(ActionPointer(new ActionMoveSTW(amount)));
	} else {
		newActionVector.push_back(ActionPointer(new ActionMoveWTS(-amount)));
	}

	newActionVector.push_back(ActionPointer(new ActionNoOp(1.0 - abs(amount))));

	syncopation = amount;
	actionVector = newActionVector;
}

vector<int> RhythmGenerator::inverseVector(vector<int>& v) {
	vector<int> res;
	for (int i = 0; i < v.size(); i++) {
		std::vector<int>::iterator itr = std::find(v.begin(), v.end(), i);
		if (itr != v.cend()) {
			res.push_back(std::distance(v.begin(), itr));
		}
	}
	return res;
}

void RhythmGenerator::newSequence(Sequence& sequence) {
	sequence.genPulses(invPhi);
	sequence.genMods(phi);
	sequence.genModPulses(phi);
}

void RhythmGenerator::setSignature(int num, int strat) {
	numerator = num;
	stratification = strat;
	phi = BarlowAnalysis::calculateSequence(num, strat);
	invPhi = inverseVector(phi);
}