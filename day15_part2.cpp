#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day15.txt";

struct Lens {
    string label;
    int focal_length;
    bool is_valid;
};

// box -> vector<Lens>
map<int, vector<Lens>> box_to_lens;

namespace {
int hashx(const string& s) {
    int out = 0;
    for (const char c : s) {
        out += int(c);
        out *= 17;
        out %= 256;
    }
    return out;
}

string get_label(const string& input) {
    return advent::str_split(advent::str_split(input, '=')[0], '-')[0];
}

char get_operation(const string& input) {
    return (input.find('=') != string::npos) ? '=' : '-';
}

int get_focal_length(const string& input) {
    return stoi(advent::str_split(input, '=')[1]);
}

void process_one(const string& input) {
    string label = get_label(input);
    char operation = get_operation(input);
    int box_id = hashx(label);

    if (operation == '-') {
        vector<Lens>& lenses = box_to_lens[box_id];
        for (Lens& lens : lenses) {
            if ((lens.label == label) && lens.is_valid) {
                lens.is_valid = false;
                return;
            }
        }
    } else { // if (operation == '=') {
        int focal_length = get_focal_length(input);
        vector<Lens>& lenses = box_to_lens[box_id];
        for (Lens& lens : lenses) {
            if ((lens.label == label) && lens.is_valid) {
                lens.focal_length = focal_length;
                return;
            } 
        }
        Lens lens {
            .focal_length = focal_length,
            .is_valid = true,
            .label = label };
        lenses.push_back(lens);
    }
}

int process(const vector<string>& input) {

    for (const string& s : input) process_one(s);

    // compute answer.
    int sum = 0;
    for (auto [box, lenses] : box_to_lens) {
        int box_id = box;
        int valid_count = 0;
        for (int i = 0; i < lenses.size(); i++) {
            Lens lens = lenses[i];
            if (lens.is_valid) {
                valid_count++;
                sum += ((box_id + 1) * (valid_count) * lens.focal_length);
            }
        }
    }

    return sum;
}


}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<string> input = advent::str_split(raw_text[0], ',');
 
    // init boxes ds.
    for (int i = 0; i < 256; i++) box_to_lens[i] = vector<Lens>();

    cout << "Ans: " << process(input) << endl;
}
