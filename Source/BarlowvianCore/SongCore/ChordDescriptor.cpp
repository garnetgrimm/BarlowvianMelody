#include "ChordDescriptor.h"
#include <set>
#include <algorithm>

int ChordDescriptor::seekInt(string str, int& index) {
    int start = index;
    while (index < str.length()) {
        if (!std::isdigit(str[index])) {
            break;
        }
        index++;
    }

    if (index == start) return -1;

    string sub = str.substr(start, index);
    return std::stoi(sub);
}

pair<ChordDescriptor::modType, int> ChordDescriptor::seekMod(string str, int& index) {
    int n = int(str.length());
    auto err = pair<modType, int>{ INVALID, -1 };
    modType mod;
    if(index < n) {
        if (str[index] == '#') mod = SHARP;
        else if (str[index] == 'b') mod = FLAT;
        else if (str[index] == 'o') mod = OMIT;
        else if (str[index] == 'a') mod = ADD;
        else return err;

        index++;
    }
    if (index < n) {
        int degree = ChordDescriptor::seekInt(str, index);
        if(degree == -1) return err;
        return { mod, degree };
    }
    return err;
}

bool ChordDescriptor::isValid(string chordStr, Scale scale) {
    ChordDescriptor chord(chordStr, scale);
    return !chord.empty();
}

ChordDescriptor::ChordDescriptor(string chordStr, Scale scale) {
    Chord chord;

    if (chordStr.length() == 0) return;

    int readIdx = 0;
    int root = seekInt(chordStr, readIdx);

    if (root <= 0) return;
    if (root > scale.size()) return;

    for (int i = 0; i < scale.thirdsToStack; i++) {
        MidiNote note = scale[root - 1 + i * 2];
        chord.push_back(note);
    }

    while (readIdx < chordStr.length()) {
        auto mod = seekMod(chordStr, readIdx);
        if (mod.first == INVALID) {
            return;
        } else if (mod.first == SHARP) {
            chord.push_back(scale[root + mod.second - 2] + MidiNote(1));
        } else if (mod.first == FLAT) {
            chord.push_back(scale[root + mod.second - 2] - MidiNote(1));
        } else if (mod.first == ADD) {
            chord.push_back(scale[root + mod.second - 2]);
        } else if (mod.first == OMIT) {
            MidiNote target(scale[root + mod.second - 2]);
            chord.erase(std::remove(chord.begin(), chord.end(), target), chord.end());
        }
    }

    for (auto& note : chord) {
        this->push_back(note);
    }

    name = chordStr;
}

ChordDescriptor::ChordDescriptor(Chord chord, string name) {
    for (auto& note : chord) {
        this->push_back(note);
    }
    this->name = name;
}

string ChordDescriptor::getName() {
    return name;
}