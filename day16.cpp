#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day16.txt";

struct Point {
    int row;
    int col;

    Point(int row, int col) : row(row), col(col) {}

    string to_string() {
        return std::to_string(this->row) + "_" + std::to_string(this->col);
    }
};

enum Direction {
    up, down, left, right
};

vector<string> grid;

map<string, set<Direction>> memo;
std::unordered_set<string> activations;

namespace {

void init_memo() {
    // init memo
    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid.size(); col++) {
            memo[Point(row, col).to_string()] = set<Direction>();
        }
    }
}

void draw_activations() {
    cout << ".0123456789" << endl;
    for (int row = 0; row < grid.size(); row++) {
        cout << row;
        for (int col = 0; col < grid.size(); col++) {
            if (activations.find(Point(row, col).to_string()) != activations.end()) {
                cout << "#";
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
    cout << endl;    
}

char get_tile(const Point& p) {
    return grid[p.row][p.col];
}

bool is_valid(const Point& p) {
    return (
        (p.row < grid.size()) && 
        (p.col < grid[0].size()) &&
        (p.row >= 0) &&
        (p.col >= 0)
    );
}

Point move(const Point& p, Direction d) {
    if (d == Direction::up) {
        return Point(p.row - 1, p.col);
    } else if (d == Direction::down) {
        return Point(p.row + 1, p.col);
    } else if (d == Direction::left) {
        return Point(p.row, p.col - 1);        
    } else { // (d == Direction::right)
        return Point(p.row, p.col + 1);
    }
}

//  tile -> incoming_direction -> list<outgoing_direction>
map<char, map<Direction, vector<Direction>>> direction_change {
    {'.',
        {
            {Direction::up, {Direction::up}},
            {Direction::down, {Direction::down}},
            {Direction::left, {Direction::left}},
            {Direction::right, {Direction::right}},
        }
    },

    {'\\',
        {
            {Direction::up, {Direction::left}},
            {Direction::down, {Direction::right}},
            {Direction::left, {Direction::up}},
            {Direction::right, {Direction::down}},
        }
    },

    {'/',
        {
            {Direction::up, {Direction::right}},
            {Direction::down, {Direction::left}},
            {Direction::left, {Direction::down}},
            {Direction::right, {Direction::up}},
        }
    },

    {'|',
        {
            {Direction::up, {Direction::up}},
            {Direction::down, {Direction::down}},
            {Direction::left, {Direction::up, Direction::down}},
            {Direction::right, {Direction::up, Direction::down}},
        }
    },

    {'-',
        {
            {Direction::up, {Direction::left, Direction::right}},
            {Direction::down, {Direction::left, Direction::right}},
            {Direction::left, {Direction::left}},
            {Direction::right, {Direction::right}},
        }
    },
};

int traversals = 0;

void traverse(
    Point p,
    Direction d) {
    traversals++;
    auto previous_ds = memo[p.to_string()];
    if (previous_ds.find(d) == previous_ds.end()) {
        memo[p.to_string()].insert(d);
    } else {
        return;
    }
    char tile = get_tile(p);
    vector<Direction> next_directions =
        direction_change[tile][d];

    for (auto d : next_directions) {
        Point p_next = move(p, d);
        // draw_activations();
        if (is_valid(p_next)) {
            activations.insert(p_next.to_string());
            traverse(p_next, d);
        }
    }
}

}  // namespace

int main() {
    grid = advent::read_file(kInputFile);
    init_memo();
    activations.clear();

    activations.insert(Point(0,0).to_string());
    traverse(Point(0,0), Direction::right);
    int ans = activations.size();
    cout << "Ans part-1 : " << ans << endl;

    int max_activations = 0;
    for (int row = 0; row < grid.size(); row++) {
        cout << "row " << row;
        init_memo();
        activations.clear();
        Point p1 = Point(row, 0);
        activations.insert(p1.to_string());
        traverse(p1, Direction::right);
        cout << " activation 1: " << activations.size() << " ";
        max_activations = max(max_activations, int(activations.size())); 

        init_memo();
        activations.clear();
        Point p2 = Point(row, grid[0].size() - 1);
        activations.insert(p2.to_string());
        traverse(p2, Direction::left);
        cout << " activation 2: " << activations.size() << " ";
        max_activations = max(max_activations, int(activations.size()));
        cout << " max: " << max_activations << endl;
    }

    for (int col = 0; col < grid[0].size(); col++) {
        cout << "col " << col << " ";
        init_memo();
        activations.clear();
        Point p1 = Point(0, col);
        activations.insert(p1.to_string());
        traverse(p1, Direction::down);
        cout << activations.size() << " ";
        max_activations = max(max_activations, int(activations.size())); 

        init_memo();
        activations.clear();
        Point p2 = Point(grid.size() - 1, col);
        activations.insert(p2.to_string());
        traverse(p2, Direction::up);
        cout << activations.size() << " ";
        max_activations = max(max_activations, int(activations.size()));
        cout << " max: " << max_activations << endl;
    }

    cout << "Ans part-2 : " << max_activations << endl;
}
