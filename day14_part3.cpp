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

void vprint(const vector<string>& v) {
    for (const string& s : v) cout << s << endl;
    cout << endl;
}

vector<string> tilt_north(const vector<string>& input) {
    vector<string> reshaped_input;

    // first transpose
    for (int column = 0; column < input[0].size(); column++) {
        string s;
        for (int row = 0; row < input.size(); row++) {
            s += input[row][column];
        }
        reshaped_input.push_back(s);
    }

    vector<string> tilted_input;
    int max_score = input.size();
    for (const string& s : reshaped_input) {
        string tilted = "";
        int start_index = 0;
        for (int i = 0; i < max_score; ++i) {
            int rock_count = 0;
            while((i < max_score) && (s.at(i) != '#')) {
                if (s.at(i) == 'O') rock_count++;
                i++;
            }

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

        tilted_input.push_back(tilted);
    }

    // transpose back
    vector<string> final_output;
    for (int column = 0; column < tilted_input[0].size(); column++) {
        string s;
        for (int row = 0; row < tilted_input.size(); row++) {
            s += tilted_input[row][column];
        }
        final_output.push_back(s);
    }

    return final_output;
}

vector<string> tilt_west(const vector<string>& input) {
    vector<string> tilted_input;
    int max_score = input.size();
    for (const string& s : input) {
        string tilted = "";
        int start_index = 0;
        for (int i = 0; i < max_score; ++i) {
            int rock_count = 0;
            while((i < max_score) && (s.at(i) != '#')) {
                if (s.at(i) == 'O') rock_count++;
                i++;
            }

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

        tilted_input.push_back(tilted);
    }

    return tilted_input;
}

vector<string> tilt_south(const vector<string>& input) {
    vector<string> reshaped_input;

    // first transpose
    for (int column = 0; column < input[0].size(); column++) {
        string s;
        for (int row = 0; row < input.size(); row++) {
            s += input[row][column];
        }
        reshaped_input.push_back(s);
    }

    vector<string> tilted_input;
    int max_score = input.size();
    for (const string& s : reshaped_input) {
        string tilted = "";
        int start_index = 0;
        for (int i = 0; i < max_score; ++i) {
            int dot_count = 0;
            while((i < max_score) && (s.at(i) != '#')) {
                if (s.at(i) == '.') dot_count++;
                i++;
            }

            // found # at i
            for (int j = 0; j < dot_count; ++j) {
                tilted += '.';
            }
            for (int j = start_index + dot_count; j < i; ++j) {
                tilted += 'O';            
            }

            if (i >= max_score) break;
            tilted += "#";
            start_index = i + 1;
        }

        tilted_input.push_back(tilted);
    }

    // transpose back
    vector<string> final_output;
    for (int column = 0; column < tilted_input[0].size(); column++) {
        string s;
        for (int row = 0; row < tilted_input.size(); row++) {
            s += tilted_input[row][column];
        }
        final_output.push_back(s);
    }

    return final_output;
}

vector<string> tilt_east(const vector<string>& input) {
    vector<string> tilted_input;
    int max_score = input.size();
    for (const string& s : input) {
        string tilted = "";
        int start_index = 0;
        for (int i = 0; i < max_score; ++i) {
            int dot_count = 0;
            while((i < max_score) && (s.at(i) != '#')) {
                if (s.at(i) == '.') dot_count++;
                i++;
            }

            // found # at i
            for (int j = 0; j < dot_count; ++j) {
                tilted += '.';
            }
            for (int j = start_index + dot_count; j < i; ++j) {
                tilted += 'O';            
            }

            if (i >= max_score) break;
            tilted += "#";
            start_index = i + 1;
        }

        tilted_input.push_back(tilted);
    }

    return tilted_input;
}

int score(const vector<string>& input) {
    int score = 0;
    int max_score = input.size();
    for (int i = 0; i < max_score; ++i) {
        for (char c : input.at(i)) {
            if (c == 'O') {
                score += (max_score - i);
            }
        }
    }

    return score;    
}

string flatten(const vector<string>& input) {
    string out;
    for (const auto& s : input) out += s;
    return out;
}


}  // namespace


int main() {
    const vector<string> original_input = advent::read_file(kInputFile);

    vector<string> reshaped_input = original_input;
    set<string> seen_patterns;
    seen_patterns.insert(flatten(reshaped_input));
    long loop_size_upper_bound = 0;
    for (loop_size_upper_bound = 0; loop_size_upper_bound < 1000000000; ++loop_size_upper_bound) {
        reshaped_input = tilt_east(tilt_south(tilt_west(tilt_north(reshaped_input))));
        string flattened = flatten(reshaped_input);
        if (seen_patterns.find(flattened) != seen_patterns.end()) break;
        seen_patterns.insert(flattened);
    }

    cout << "Loop found after: " << loop_size_upper_bound << endl;

    string loop_start_flat = flatten(reshaped_input);
    long loop_size;
    for (loop_size = 1; loop_size <= loop_size_upper_bound; loop_size++) {
        reshaped_input = tilt_east(tilt_south(tilt_west(tilt_north(reshaped_input))));
        if (flatten(reshaped_input) == loop_start_flat) break;
    }
    cout << "Loop size: " << loop_size << endl;

    long iterations_needed = (1000000000  - loop_size_upper_bound - 1) % loop_size;
    cout << "Iterations needed " << iterations_needed << endl;

    for (int i = 0; i < iterations_needed; ++i) {
        reshaped_input = tilt_east(tilt_south(tilt_west(tilt_north(reshaped_input))));
    }

    cout << "Ans: " << score(reshaped_input) << endl;
}
