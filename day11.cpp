#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day11.txt";

struct Position {
    int row;
    int col;
};

vector<Position> galaxies;
set<int> expand_rows;
set<int> expand_columns;

long distance(const Position& a, const Position& b) {
    int start_row = min(a.row, b.row);
    int end_row = max(a.row, b.row);

    int start_col = min(a.col, b.col);
    int end_col = max(a.col, b.col);

    int expansions = 0;
    for (int i = start_row + 1; i < end_row; i++) {
        if (expand_rows.find(i) != expand_rows.end()) expansions++;
    }
    for (int i = start_col + 1; i < end_col; i++) {
        if (expand_columns.find(i) != expand_columns.end()) expansions++;
    }

    // return (end_row - start_row) + (end_col - start_col) + (expansions * 999999);
    return (end_row - start_row) + (end_col - start_col) + (expansions * 1);
}

int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    set<int> seen_rows;
    set<int> seen_columns;

    for (int row = 0; row < raw_text.size(); ++row) {
        for (int col = 0; col < raw_text[row].size(); ++col) {
            if (raw_text[row][col] == '#') {
                Position p {.row = row, .col = col};
                galaxies.push_back(p);
                seen_rows.insert(row);
                seen_columns.insert(col);
            }
        }
    }

    for (int row = 0; row < raw_text.size(); ++row) {
        if (seen_rows.find(row) == seen_rows.end()) expand_rows.insert(row);
    }

    for (int col = 0; col < raw_text[0].size(); col++) {
        if (seen_columns.find(col) == seen_columns.end()) expand_columns.insert(col);      
    }

    cout << galaxies.size() << endl;
    cout << expand_rows.size() << endl;
    cout << expand_columns.size() << endl;

    long sum = 0;
    for (int i = 0; i < galaxies.size(); ++i) {
        for (int j = i + 1; j < galaxies.size(); ++j) {
            sum += distance(galaxies[i], galaxies[j]);
        }
    }

    cout << "Ans " << sum << endl;
}
