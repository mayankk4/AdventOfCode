#include <iostream>

#include "advent_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;

const string kInputFile = "./data/day2.txt";

namespace {

// game = " 10 green, 5 blue"
int get_balls_of_color(const string& color_id, string game) {
    vector<string> game_draws = advent::str_split(game, ',');
    // draw = " 10 green"
    for (const string& draw : game_draws) {
        vector<string> draw_to_color = advent::str_split(draw, ' ');
        if (!(draw_to_color[draw_to_color.size() - 1] == color_id)) continue;
        return stoi(draw_to_color[1]);
    }
    return 0;
}

// returns the id of the valid game, else returns 0.
// Game 1: 10 green, 5 blue; 1 red, 9 green, 10 blue; 5 blue, 6 green, 2 red; 7 green, 9 blue, 1 red; 2 red, 10 blue, 10 green; 7 blue, 1 red
int get_power(string line) {
    vector<string> id_to_games = advent::str_split(line, ':');
    vector<string> games = advent::str_split(id_to_games[1], ';');

    // In case of wrong answer, init this to 1.
    int max_red = 0;
    int max_blue = 0;
    int max_green = 0;
    for (const string& game : games) {
        max_red = std::max(get_balls_of_color("red", game), max_red);
        max_blue = std::max(get_balls_of_color("blue", game), max_blue);
        max_green = std::max(get_balls_of_color("green", game), max_green);        
    }

    return max_red * max_blue * max_green;
}
}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int count = 0;
    for (const string& line : raw_text) {
        count += get_power(line);
    }

    cout << "Answer: " << count << endl;
}
