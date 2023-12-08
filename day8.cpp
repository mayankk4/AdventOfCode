#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day8.txt";

using advent::str_split;

int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    // index
    const string instructions = raw_text[0];

    map<string, string> traversal;
    for (int i = 2; i < raw_text.size(); ++i) {
        // "AAA = (BBB, CCC)"
        string line = raw_text[i];

        vector<string> splits = str_split(line, '=');

        string key_root = str_split(splits[0], ' ')[0];
        string value_lhs = str_split(str_split(str_split(splits[1], ',')[0], '(')[0], ' ')[0];
        string value_rhs = str_split(str_split(str_split(splits[1], ',')[1], ')')[0], ' ')[0];

        traversal[key_root + "_L"] = value_lhs;
        traversal[key_root + "_R"] = value_rhs;
    }

    // retrieve
    int steps = 0;
    int instruction_pointer = 0;
    string current_node = "AAA";
    while(true) {
        if (current_node == "ZZZ") break;
        steps++;

        if (instruction_pointer >= instructions.size()) instruction_pointer = 0;

        if (instructions.at(instruction_pointer) == 'L') {
            current_node = traversal[current_node + "_L"];
        } else {
            current_node = traversal[current_node + "_R"];
        }
        instruction_pointer++;
    }

    cout << "Answer: " << steps << endl;
}
