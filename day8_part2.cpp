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

    set<string> current_nodes;
    for (const string& k : stl_utils::MapKeys(traversal)) {
        if (k.at(k.size() - 3) == 'A') {
            current_nodes.insert(str_split(k, '_')[0]);   
        }
    }

    // Confirm that the 'Z' always comes at the multiple of instructions.size()
    // Thus we can take LCM of all the end_steps. This would not
    // have worked if all the cycles were not ending at instructions.size()
    for (const string& current_node : current_nodes) {
        vector<int> end_steps;
        vector<int> instruction_ptrs;
        int step = 0;
        int instruction_pointer = 0;
        cout << "Node " << current_node << endl; 
        while(true) {
            if (end_steps.size() >= 10) break;
            
            if (current_node.at(current_node.size() - 1) == 'Z') {
                end_steps.push_back(step);
                instruction_ptrs.push_back(instruction_pointer);
            }
            step++;

            if (instruction_pointer >= instructions.size()) instruction_pointer = 0;

            if (instructions.at(instruction_pointer) == 'L') {
                current_node = traversal[current_node + "_L"];
            } else {
                current_node = traversal[current_node + "_R"];
            }
            instruction_pointer++;
        }
        // Debug
        for (auto step : end_steps) {
            cout << step << " ";
        }
        cout << endl;
        for (auto p : instruction_ptrs) {
            cout << p << " ";
        }
        cout << endl << endl << endl;
    }

    vector<int> cycles;
    for (auto current_node : current_nodes) {
        int step = 0;
        int instruction_pointer = 0;
        while(true) {
            if (current_node.at(current_node.size() - 1) == 'Z') {
                cycles.push_back(step);
                break;
            }
            step++;

            if (instruction_pointer >= instructions.size()) instruction_pointer = 0;

            if (instructions.at(instruction_pointer) == 'L') {
                current_node = traversal[current_node + "_L"];
            } else {
                current_node = traversal[current_node + "_R"];
            }
            instruction_pointer++;
        }
    }    

    long lcm = cycles[0];
    for (int i : cycles) lcm = std::lcm(lcm, i);

    cout << "Answer: " << lcm << endl;
}
