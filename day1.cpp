/*
    clang++ -std=c++17 day1.cpp -o ~/Desktop/tmp/prog && ~/Desktop/tmp/prog
*/
#include <iostream>
#include <map>
#include <vector>

#include "advent_utils.cpp"

const string kInputFile = "./data/day1.txt";

namespace {

map<string, int> SPELLING_TO_DIGIT {
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

// returns the spelt out digit if found at the given index of line.
string is_digit_spelt(const string& line, const int index) {
    for (const auto& [key, val] : SPELLING_TO_DIGIT) {
        if (advent::is_prefix(key, line, index)) {
            return key;
        }
    }
    return "";
}

// Parses the line into a vector of numbers.
vector<int> parse_line(
    const string& line, bool advanced) {
    vector<int> numbers;

    for(int i = 0; i < line.size(); ++i) {
        char c = line.at(i);
        if (isdigit(c)) {
            numbers.push_back(c - '0');
            continue;
        }

        // if advanced parsing is enabled, we also check for
        // spelt out digits.
        if (advanced) {
            string spelt_digit = is_digit_spelt(line, i);
            if (!spelt_digit.empty()) {
                numbers.push_back(SPELLING_TO_DIGIT[spelt_digit]);
                // since we do ++i and the end of the loop anyway.
                // Do not move to the end because there can be
                // overlaps eg. twoneight = 218.
                // i+= (spelt_digit.size() - 1);
            }
        }
    }

    return numbers;
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int count = 0;
    for (const string& line : raw_text) {
        vector<int> numbers = parse_line(line, /*advanced=*/true);
        count+= ((numbers[0] * 10) + numbers[numbers.size() - 1]);
    }

    cout << count << endl;
}
