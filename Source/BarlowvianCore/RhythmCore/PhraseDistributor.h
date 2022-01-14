#pragma once

#include "BarlowAnalysis.h"

using namespace std;

class PhraseDistributor {

	int time_sig_num = 4;

	int total_bar_count = 1;
	int phrase_strat = 2;
	int melodic_strat = 4;
	pair<int, int> melodic_note_len = { 1, 3 };

	float melodic_density = 0.8;
	pair<int, int> melodic_note_interval = { 0, 8 };
	int rhythmic_anticipation = 1;

	BarlowAnalysis clarenceBarlow;

	vector<vector<float>> genAllPhraseRhythms();
	vector<int> genPhraseRhythm(int note_per_phrase);
	int randint(int min, int max);

};