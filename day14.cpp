#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day14.txt";

namespace {


int process(const string& s) {
    int max_score = s.size();
    string tilted = "";

    int start_index = 0;
    for (int i = 0; i < max_score; ++i) {
        int rock_count = 0;
        while((i < max_score) && (s.at(i) != '#')) {
            if (s.at(i) == 'O') rock_count++;
            i++;
        }

        cout << "here 1 " << i << endl;

        // found # at i
        for (int j = 0; j < rock_count; ++j) {
            tilted += 'O';
        }
        for (int j = start_index + rock_count; j < i; ++j) {
            tilted += '.';            
        }

        if (i >= max_score) break;
        tilted += "#";
        start_index = i + 1;
    }

    cout << "Tilted:" << tilted << endl;

    // process tilted input.
    int score = 0;
    for (int i = 0; i < tilted.size(); ++i) {
        if (tilted[i] == 'O') {
            score += (max_score - i);
        }
    }

    return score;
}

}  // namespace


int main() {
    vector<string> input = advent::read_file(kInputFile);

    // transpose
    vector<string> reshaped_input;
    for (int column = 0; column < input[0].size(); column++) {
        string s;
        for (int row = 0; row < input.size(); row++) {
            s += input[row][column];
        }
        reshaped_input.push_back(s);
    }

    cout << reshaped_input[1] << endl;
    cout << process(reshaped_input[1]) << endl;

    int ans = std::accumulate(
        reshaped_input.begin(), reshaped_input.end(), 0,
        [](int acc, const string& s) {
            return acc += process(s);
        }
    );

    cout << "Ans " << ans << endl;
}
