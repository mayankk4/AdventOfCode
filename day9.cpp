#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day9.txt";

namespace {

template<typename T>
T last_elem(const vector<T>& v) {
    return v[v.size()-1];
}

vector<int> parse_line(const string& line) {
    vector<string> splits = advent::str_split(line, ' ');
    vector<int> parsed_line;
    std::transform(
        splits.begin(), splits.end(), std::back_inserter(parsed_line),
        [](const string& s) { return stoi(s); });
    return parsed_line;
}

vector<vector<int>> generate_diff_pyramid(const vector<int>& input) {
    vector<vector<int>> diff_pyramid;
    diff_pyramid.push_back(input);
    while(true) {
        vector<int> last_diffs = last_elem(diff_pyramid);
        vector<int> next_diffs;
        int sum_next_diffs = 0;
        for (int i = 1; i < last_diffs.size(); i++) {
            int x = last_diffs[i] - last_diffs[i - 1];
            next_diffs.push_back(x);
            sum_next_diffs += x;
        }
        if (sum_next_diffs == 0) break;
        diff_pyramid.push_back(next_diffs);
    }
    return diff_pyramid;
}

int extrapolate_left(const vector<int> input) {
    vector<vector<int>> diff_pyramid = generate_diff_pyramid(input);

    for (int i = diff_pyramid.size() - 2; i >= 0; --i) {
        diff_pyramid[i].push_back(diff_pyramid[i][0] - last_elem(diff_pyramid[i + 1]));
    }

    return last_elem(diff_pyramid[0]);
}

int extrapolate_right(const vector<int>& input) {
    vector<vector<int>> diff_pyramid = generate_diff_pyramid(input);

    for (int i = diff_pyramid.size() - 2; i >= 0; --i) {
        diff_pyramid[i].push_back(last_elem(diff_pyramid[i + 1]) + last_elem(diff_pyramid[i]));
    }

    return last_elem(diff_pyramid[0]);
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int part_1 = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [](int acc, const string& line) {
            vector<int> input = parse_line(line);
            return acc += extrapolate_right(input);
        }
    );

    int part_2 = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [](int acc, const string& line) {
            vector<int> input = parse_line(line);
            return acc += extrapolate_left(input);
        }
    );

    cout << "Ans part1: " << part_1 << endl;
    cout << "Ans part2: " << part_2 << endl;
}
