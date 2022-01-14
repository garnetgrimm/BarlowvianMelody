#include "RhythmAction.h"

void ActionNoOp::apply(int i, vector<int>& phi, vector<int>& modPulses) {
}

void ActionMoveSTW::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] < phi[i] && !modPulses[i - 1]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] < phi[i] && !modPulses[i + 1]);

	if (modLeft && modRight) {
		if (RhythmAction::tieBreak()) modRight = false;
		else modLeft = false;
	}

	if (modRight) {
		modPulses[i] = 0;
		modPulses[i - 1] = 1;
	}
	else if (modLeft) {
		modPulses[i] = 0;
		modPulses[i + 1] = 1;
	}
}

void ActionCopySTW::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] < phi[i]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] < phi[i]);

	if (modLeft && modRight) {
		if (RhythmAction::tieBreak()) modRight = false;
		else modLeft = false;
	}

	if (modRight) {
		modPulses[i] = 1;
		modPulses[i - 1] = 1;
	}
	else if (modLeft) {
		modPulses[i] = 1;
		modPulses[i + 1] = 1;
	}
}

void ActionSplitSTW::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] < phi[i]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] < phi[i]);

	if (modRight) {
		modPulses[i] = 0;
		modPulses[i - 1] = 1;
	}
	if (modLeft) {
		modPulses[i] = 0;
		modPulses[i + 1] = 1;
	}
}

void ActionMoveWTS::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] > phi[i] && !modPulses[i - 1]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] > phi[i] && !modPulses[i + 1]);

	if (modLeft && modRight) {
		if (RhythmAction::tieBreak()) modRight = false;
		else modLeft = false;
	}

	if (modRight) {
		modPulses[i] = 0;
		modPulses[i - 1] = 1;
	}
	else if (modLeft) {
		modPulses[i] = 0;
		modPulses[i + 1] = 1;
	}
}

void ActionCopyWTS::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] > phi[i]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] > phi[i]);

	if (modLeft && modRight) {
		if (RhythmAction::tieBreak()) modRight = false;
		else modLeft = false;
	}

	if (modRight) {
		modPulses[i] = 1;
		modPulses[i - 1] = 1;
	}
	else if (modLeft) {
		modPulses[i] = 1;
		modPulses[i + 1] = 1;
	}
}

void ActionSplitWTS::apply(int i, vector<int>& phi, vector<int>& modPulses) {
	bool modRight = (i > 0 && phi[i - 1] > phi[i]);
	bool modLeft = (i < phi.size() - 1 && phi[i + 1] > phi[i]);

	if (modRight) {
		modPulses[i] = 0;
		modPulses[i - 1] = 1;
	}
	if (modLeft) {
		modPulses[i] = 0;
		modPulses[i + 1] = 1;
	}
}
