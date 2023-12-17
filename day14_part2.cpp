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

bool is_same(const vector<string>& original, const vector<string>& current) {
    for (int i = 0; i < original.size(); i++) {
        if (original[i] != current[i]) return false;
    }
    return true;
}

}  // namespace


int main() {
    const vector<string> original_input = advent::read_file(kInputFile);
    vprint(original_input);

    // cout << "Tilt north" << endl;
    // vector<string> north_tilted_input = tilt_north(original_input);
    // vprint(north_tilted_input);

    // cout << "Tilt west" << endl;
    // vector<string> west_tilted_input = tilt_west(north_tilted_input);
    // vprint(west_tilted_input);

    // cout << "Tilt south" << endl;
    // vector<string> south_tilted_input = tilt_south(west_tilted_input);
    // vprint(south_tilted_input);

    // cout << "Tilt east" << endl;
    // vector<string> east_tilted_input = tilt_east(south_tilted_input);
    // vprint(east_tilted_input);

    vector<string> reshaped_input = original_input;
    int i = 0;
    for (i = 0; i < 3; ++i) {
        // if ((i % 1000000) == 0) cout << i << endl;
        reshaped_input = tilt_east(tilt_south(tilt_west(tilt_north(reshaped_input))));
        // if (is_same(original_input, reshaped_input)) break;
    }

    // cout << "iterations needed: " << i << endl;

    // cout << "After  1000000000 iterations: " << endl;
    vprint(reshaped_input);
    // cout << "Ans: " << score(reshaped_input) << endl;

}
