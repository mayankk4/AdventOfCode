#include <iostream>

#include "advent_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;
using ::std::max;
using ::std::min;
using ::std::isdigit;

const string kInputFile = "./data/day3.txt";

namespace {

bool has_adjacent_symbol(int start_index, int end_index, const string& line) {
    if (line.empty()) return false;
    int start = max(0, start_index - 1);
    int end = min(end_index, int(line.size()) - 1);
    for (int i = start; i <= end; ++i) {
        if (!isdigit(line.at(i)) && (line.at(i) != '.')) return true;
    }
    return false;
}

int get_curr_line_sum(
    const string& prev_line, const string& curr_line, const string& next_line) {
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


        // check if the number is valid.
        if (has_adjacent_symbol(start_index, end_index, prev_line) ||
            has_adjacent_symbol(start_index, end_index, curr_line) ||
            has_adjacent_symbol(start_index, end_index, next_line)) {
            count += num_extracted;
        }
    }
    return count;
}
}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int count = 0;
    for (int i = 0; i < raw_text.size(); i++) {
        string prev_line, curr_line, next_line = "";
        if (i - 1 >= 0) prev_line = raw_text[i - 1];
        curr_line = raw_text[i];
        if (i + 1 >= 0) next_line = raw_text[i + 1];

        count += get_curr_line_sum(prev_line, curr_line, next_line);
    }

    cout << "Answer " << count << endl;
}
