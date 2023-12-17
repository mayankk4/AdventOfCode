#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day10.txt";

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return (this->x == other.x && this->y == other.y);
    }
};

namespace {

char pipe_at(const Position& position, const vector<string>& grid) {
    return grid[position.y].at(position.x);
}

Position next(
    const Position& prev, const Position& curr,
    const vector<string>& grid) {
    Position next;
    char pipe = pipe_at(curr, grid);
    if (pipe == '|') {
        next.x = prev.x;
        next.y = (prev.y == curr.y - 1) ? curr.y + 1 : curr.y - 1;
    } else if (pipe == '-') {
        next.x = (prev.x == curr.x - 1) ? curr.x + 1 : curr.x - 1;
        next.y = prev.y;
    } else if (pipe == 'L') {
        next.x = (prev.x == curr.x) ? curr.x + 1 : curr.x ;
        next.y = (prev.y == curr.y) ? curr.y - 1 : curr.y ;
    } else if (pipe == 'J') {
        next.x = (prev.x == curr.x) ? curr.x - 1 : curr.x;
        next.y = (prev.y == curr.y) ? curr.y - 1 : curr.y;
    } else if (pipe == '7') {
        next.x = (prev.x == curr.x) ? curr.x - 1 : curr.x ;
        next.y = (prev.y == curr.y) ? curr.y + 1 : curr.y;
    } else if (pipe == 'F') {
        next.x = (prev.x == curr.x) ? curr.x + 1 : curr.x;
        next.y = (prev.y == curr.y) ? curr.y + 1 : curr.y;
    }

    return next;
}


// finds starting position and updates it in the grid.
Position find_starting_position(const vector<string>& grid) {
    // origin is top-left.
    int x = 0;
    int y = 0;
    for (int row = 0; row < grid.size(); ++row) {
        for (int column = 0; column < grid[row].size(); ++ column) {
            if (grid[row][column] == 'S') {
                Position pos { .x = column, .y = row };
                return pos;
            }
        }
    }
    return Position({.x = 0, .y = 0});
}

Position find_initial_move(
    const Position& starting_position, const vector<string>& grid) {
    int x = starting_position.x;
    int y = starting_position.y;

    Position p {.x = starting_position.x, .y = starting_position.y + 1};
    return p;
}

int find_loop_size(const vector<string>& raw_text) {
    const vector<string> grid = raw_text;
    Position starting_position = find_starting_position(grid);
    vector<Position> visited_positions;
    visited_positions.push_back(starting_position);

    Position current_position = find_initial_move(starting_position, grid);
    Position previous_position = starting_position;

    while(pipe_at(current_position, grid) != 'S') {
        visited_positions.push_back(current_position);
        cout << "*** Debug move ***" << endl;
        cout << "Previous pos: " << previous_position.x << " " << previous_position.y << endl;
        cout << "Current pos: " << current_position.x << " " << current_position.y << endl;
        cout << "Walk : " << pipe_at(current_position, grid) << endl;
        Position temp = current_position;
        current_position = next(previous_position, current_position, grid);
        previous_position = temp;
        cout << "Next pos: " << current_position.x << " " << current_position.y << endl;
    }

    cout << endl << visited_positions.size() << endl;
    return visited_positions.size();
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int loop_size = find_loop_size(raw_text);

    cout << "Answer : " << int((loop_size + 1) / 2) << endl;
}
