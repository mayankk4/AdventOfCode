#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day13.txt";

namespace {

// index
vector<vector<string>> parse_input(const vector<string>& input) {
    vector<vector<string>> patterns;
    for (int i = 0; i < input.size(); ++i) {
        vector<string> pattern;
        while(input[i] != "" && i < input.size()) {
            pattern.push_back(input[i]);
            i++;
        }
        if (!pattern.empty()) patterns.push_back(pattern);
    }
    return patterns;
}

// solve
int solve(const vector<string>& input) {
    map<int, vector<int>> matches;
    for (int i = 0; i < input.size(); i++) {
        matches[i] = vector<int>();
        for (int j = i + 1; j < input.size(); j++) {
            if (input[i] == input[j]) {
                matches[i].push_back(j);
            }
        }
    }

    // check every possible mirror locaiton from the top.
    for (int left = 0; left < input.size() - 1; left++) {
        // does mirror start at left.
        bool valid = true;
        int curr_left = left;
        int curr_right = curr_left + 1;
        while ((curr_left >= 0) && (curr_right < input.size()) && (valid)) {
            if (input[curr_left] != input[curr_right]) {
                valid = false;
            }
            curr_left--;
            curr_right++;
        }
        if (valid) return left + 1;
    }

    return 0;
}

int solve_horizontal(const vector<string>& input) {
    return solve(input) * 100;
}

int solve_vertical(const vector<string>& input) {
    vector<string> reshaped_input;
    for (int column = 0; column < input[0].size(); column++) {
        string s;
        for (int row = 0; row < input.size(); row++) {
            s += input[row][column];
        }
        reshaped_input.push_back(s);
    }

    return solve(reshaped_input);
}


int solve_pattern(const vector<string>& input) {
    int count = 0;
    count += solve_vertical(input);
    count += solve_horizontal(input);
    cout << "Count " << count << endl;
    return count;
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<vector<string>> patterns = parse_input(raw_text);

    int ans = std::accumulate(
        patterns.begin(), patterns.end(), 0,
        [](int acc, const vector<string>& input) {
            return acc += solve_pattern(input);
        }
    );

    cout << "Ans: " << ans << endl;
}
