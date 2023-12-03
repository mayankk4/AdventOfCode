#include <iostream>

#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "advent_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;

const string kInputFile = "./data/day2.txt";

namespace {

// game = " 10 green, 5 blue"
bool is_color_valid(const string& color_id, int max_allowed, string game) {
    vector<string> game_draws = advent::str_split(game, ',');
    // draw = " 10 green"
    for (const string& draw : game_draws) {
        vector<string> draw_to_color = advent::str_split(draw, ' ');
        if (!(draw_to_color[draw_to_color.size() - 1] == color_id)) continue;
        return (stoi(draw_to_color[1]) <= max_allowed);
    }
    return true;
}


bool is_valid_game(int max_red, int max_green, int max_blue, string game) {
    return is_color_valid("red", max_red, game) &&
        is_color_valid("green", max_green, game) && 
        is_color_valid("blue", max_blue, game);
}


// Game 1: 10 green, 5 blue; 1 red, 9 green, 10 blue; 5 blue, 6 green, 2 red; 7 green, 9 blue, 1 red; 2 red, 10 blue, 10 green; 7 blue, 1 red
bool is_valid_line(int max_red, int max_green, int max_blue, string line) {
    vector<string> games = advent::str_split(advent::str_split(line, ':')[1], ';');
    return std::accumulate(
        games.begin(), games.end(), /*initial value*/ true,
        [&](bool accumulated, string game) {
            return accumulated & is_valid_game(max_red, max_green, max_blue, game);
        }
     );
}
}  // namespace


int main() {
    const int max_red = 12;
    const int max_green = 13;
    const int max_blue = 14;

    vector<string> raw_text = advent::read_file(kInputFile);
    vector<string> valid_lines;

    std::copy_if(
        raw_text.begin(), raw_text.end(),
        std::back_inserter(valid_lines),
        [&](string line) {
            return is_valid_line(max_red, max_green, max_blue, line);
        });

    int count = std::accumulate(
        valid_lines.begin(), valid_lines.end(), /*initial value*/0,
        [](int aggregate, string line) {
            int game_id = stoi(advent::str_split(advent::str_split(line, ':')[0], ' ')[1]);
            return aggregate + game_id;
        });

    cout << "Answer: " << count << endl;
}
