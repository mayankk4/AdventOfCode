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

int find_enclosed_tiles(const vector<string>& raw_text) {
    const vector<string> grid = raw_text;
    Position starting_position = find_starting_position(grid);
    vector<Position> visited_positions;
    visited_positions.push_back(starting_position);

    Position current_position = find_initial_move(starting_position, grid);
    Position previous_position = starting_position;

    while(pipe_at(current_position, grid) != 'S') {
        visited_positions.push_back(current_position);
        Position temp = current_position;
        current_position = next(previous_position, current_position, grid);
        previous_position = temp;
    }

    map<int, vector<int>> positions_by_row;
    for (const auto& pos : visited_positions) {
        int row = pos.y;
        int col = pos.x;
        if (positions_by_row.find(row) == positions_by_row.end()) {
            positions_by_row[row] = vector<int>();
        }
        positions_by_row[row].push_back(col);
    }

    int grid_rows = grid.size();
    int grid_cols = grid[0].size();
    cout << "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9" << endl;
    for (int row = 0; row < grid_rows; ++row) {
        for (int col = 0; col < grid_cols; col++) {
            if (positions_by_row.find(row) == positions_by_row.end()) {
                cout << "  ";
            } else {
                vector<int> pipe_indices_to_mark = positions_by_row[row];
                if (std::count(pipe_indices_to_mark.begin(), pipe_indices_to_mark.end(), col) == 0) {
                    cout << "  ";
                } else {
                    cout << grid[row][col] << " ";
                }
            }
        }
        cout << endl;
    }

    cout << endl << endl;

    int max_row_with_loop = 0;
    for (int k : stl_utils::MapKeys(positions_by_row)) {
        max_row_with_loop  = max(k, max_row_with_loop);
    }

    int count = 0;
    for (auto& [k,v] : positions_by_row) {
        cout << "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9" << endl;
        if (k == max_row_with_loop) continue;
        for (int ii =0; ii < grid[k].size(); ++ii ) {
            vector<int> pipe_indices_to_mark = positions_by_row[k];
            if (std::count(pipe_indices_to_mark.begin(), pipe_indices_to_mark.end(), ii) == 0) {
                cout << "  ";
            } else {
                cout << grid[k][ii] << " ";
            }
        }
        cout << endl;
        cout << "key " << k << endl;
        std::sort(v.begin(), v.end());
        if (v.size() < 2) continue; // this is not possible

        for (int curr_col = 0; curr_col + 1 < v.size(); curr_col++) {
            cout << "**dbg** loop start: " << v[curr_col] << endl;
            // get the start of starting-wall.
            int start_index = v[curr_col];
            // move to end of starting-wall.
            if (grid[k][start_index] != '|' && grid[k][start_index] != '7' && grid[k][start_index] != 'J') {
                cout << "**here** " << grid[k][start_index] << endl;
                curr_col++;
                while(grid[k][v[curr_col]] == '-') curr_col++;
                start_index = v[curr_col];
            }

            cout << "Start index " << start_index << endl;

            curr_col++;
            // get the start of end-wall.
            int end_index = v[curr_col];
            cout << "End index " << end_index << endl;
            // first add tiles in betweeen
            for (int j = start_index + 1; j < end_index; ++j) {
                cout << "Adding col: " << j << endl;
                count++;
            }

            if (grid[k][start_index] != '|' && grid[k][start_index] != '7' && grid[k][start_index] != 'J') {
                curr_col++;
                while(grid[k][v[curr_col]] == '-') curr_col++;
            }

            // Now i++ will skip the next section in between the end of
            // current wall and start of next wall, which is what we want.
        }
        cout << endl;
    }

    return count;
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);
    int tiles = find_enclosed_tiles(raw_text);
    cout << "Answer : " << tiles << endl;
}
