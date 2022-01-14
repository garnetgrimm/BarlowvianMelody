#include "PhraseDistributor.h"

int PhraseDistributor::randint(int min, int max)
{
	return min + rand() % ((max + 1) - min);
}

vector<int> PhraseDistributor::genPhraseRhythm(int note_per_phrase) {
	int note_in_phrase = 0;
	int next_step = 0;
	vector<int> notes;
	while (note_in_phrase + next_step <= note_per_phrase) {
		if (note_in_phrase + next_step != 0)
			notes.push_back(next_step);
		note_in_phrase += next_step;
		next_step = randint(melodic_note_len.first, melodic_note_len.second);
	}
	while (note_in_phrase < note_per_phrase && note_in_phrase + melodic_note_len.first <= note_per_phrase) {
		int new_max = note_per_phrase - note_in_phrase;
		next_step = randint(melodic_note_len.first, new_max);
		notes.push_back(next_step);
		note_in_phrase += next_step;
	}
	return notes;
}

vector<vector<float>> PhraseDistributor::genAllPhraseRhythms() {
	vector<vector<float>> phrases;

	float phrase_per_bar = (float)clarenceBarlow.calculateSequence(time_sig_num, phrase_strat).size() / (float)time_sig_num;
	int total_phrase_count = phrase_per_bar * total_bar_count;

	int note_phi_len = clarenceBarlow.calculateSequence(time_sig_num, melodic_strat).size();
	int note_per_phrase = note_phi_len / phrase_per_bar;

	for (int phrase_id = 0; phrase_id < total_phrase_count; phrase_id++) {
		vector<float> phrase;
		vector<int> phraseScalars = genPhraseRhythm(note_per_phrase);
		for (int scalar : phraseScalars) {
			phrase.push_back((float)scalar / (float)note_phi_len);
		}
		phrases.push_back(phrase);
	}

	return phrases;
}