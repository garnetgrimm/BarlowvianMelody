#ifndef RHYTHMACTION_HEADER
#define RHYTHMACTION_HEADER

#include <random>
#include <vector>
#include <memory>
#include <set>

using namespace std;

struct RhythmAction {
	float prob;
	string type;
	virtual void apply(int i, vector<int>& phi, vector<int>& modPulses) = 0;
	static bool tieBreak() {
		return rand() > (RAND_MAX / 2);
	}
};

#define DECLARE_ACTION(name) 											\
	struct Action##name : public RhythmAction {							\
		Action##name(float prob) {										\
			this->prob = prob;											\
			this->type = "Action" #name;								\
		}																\
		void apply(int i, vector<int>& phi, vector<int>& modPulses);	\
	}

DECLARE_ACTION(NoOp);
DECLARE_ACTION(MoveSTW);
DECLARE_ACTION(CopySTW);
DECLARE_ACTION(SplitSTW);
DECLARE_ACTION(MoveWTS);
DECLARE_ACTION(CopyWTS);
DECLARE_ACTION(SplitWTS);

#endif
#pragma once