#include <iostream>
#include <map>

#include "advent_utils.cpp"

using ::std::isdigit;

const string kInputFile = "./data/day3.txt";

namespace {

void find_adjacent_symbol(
    int start_index, int end_index, const string& line,
    int line_index, int num_extracted,
    map<string, vector<int>>& connected_numbers) {
    if (line.empty()) return;
    int start = max(0, start_index - 1);
    int end = min(end_index, int(line.size()) - 1);
    for (int i = start; i <= end; ++i) {
        if (line.at(i) == '*') {
            string key = std::to_string(line_index) + "_" + std::to_string(i);
            if (connected_numbers.find(key) == connected_numbers.end()) {
                connected_numbers[key] = vector<int>();
            }
            connected_numbers[key].push_back(num_extracted);
        }
    }
}

// Marks if a number is connected to a *
int get_connections(
    const string& prev_line, const string& curr_line, const string& next_line,
    int curr_index,
    map<string, vector<int>>& connected_numbers) {
    int count = 0;
    for (int i = 0; i < curr_line.size(); ++i) {
        if (!isdigit(curr_line.at(i))) continue;
        int start_index = i;
        string curr_number = "";
        // check if there is a number starting at the current i
        while (i < curr_line.size() && isdigit(curr_line.at(i))) {
            curr_number += curr_line.at(i);
            i++;
        }
        // end index is non-inclusive )
        int end_index = i;
        int num_extracted = stoi(curr_number);

        // collect all the connections for the number.
        find_adjacent_symbol(start_index, end_index, prev_line, curr_index - 1, num_extracted, connected_numbers);
        find_adjacent_symbol(start_index, end_index, curr_line, curr_index, num_extracted, connected_numbers);
        find_adjacent_symbol(start_index, end_index, next_line, curr_index + 1, num_extracted, connected_numbers);
    }
    return count;
}
}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    map<string, vector<int>> connected_numbers;

    // collect connected numbers
    for (int i = 0; i < raw_text.size(); i++) {
        string prev_line, curr_line, next_line = "";
        if (i - 1 >= 0) prev_line = raw_text[i - 1];
        curr_line = raw_text[i];
        if (i + 1 >= 0) next_line = raw_text[i + 1];

        get_connections(prev_line, curr_line, next_line, i, connected_numbers);
    }

    // count sum of product of connected numbers
    int count = 0;
    for (auto [k, v] : connected_numbers) {
        if (v.size() > 1) {
            count += (v[0] * v[1]);
        }
    }

    cout << "Answer " << count << endl;
}
