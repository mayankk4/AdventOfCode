#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day17.txt";

struct Point {
    int row;
    int col;

    Point(int row, int col) : row(row), col(col) {}

    bool operator==(const Point& other) const {
        return (this->row == other.row) && (this->col == other.col);
    }
};

enum Direction {
    up, down, left, right
};

// Point -> min-heat loss.
map<Point, int> memo;
vector<string> grid;

namespace {

bool is_valid(const Point& curr_point) {
    return 
        (curr_point.row < grid.size()) && 
        (curr_point.col < grid[0].size());
}

int min_heat_loss(
    const Point& curr_point,
    const Direction& curr_direction,
    int direction_repeat_count,
    const int curr_cost) {

    if (curr_point == Point(grid.size() - 1, grid[0].length - 1)) {
        return curr_cost + grid[curr_point.row][curr_point.col];
    }

    int min_cost = INT_MAX;
    if (direction_repeat_count < 3) {
        min_cost = min();
    }

    
    memo[curr_point] = min_cost;
    return min_cost;
}


}  // namespace


int main() {
    grid = advent::read_file(kInputFile);

    int ans = min(
        min_heat_loss(Point(0,0), Direction::right, 1, 0),
        min_heat_loss(Point(0,0), Direction::down, 1, 0),
    );

    cout << "Ans: " << ans << endl;
}
