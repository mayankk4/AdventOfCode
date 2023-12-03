#include <iostream>

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


// returns the id of the valid game, else returns 0.
// Game 1: 10 green, 5 blue; 1 red, 9 green, 10 blue; 5 blue, 6 green, 2 red; 7 green, 9 blue, 1 red; 2 red, 10 blue, 10 green; 7 blue, 1 red
int is_valid_line(int max_red, int max_green, int max_blue, string line) {
    vector<string> id_to_games = advent::str_split(line, ':');
    int game_id = stoi(advent::str_split(id_to_games[0], ' ')[1]);
    vector<string> games = advent::str_split(id_to_games[1], ';');

    for (const string& game : games) {
        if (!is_valid_game(max_red, max_green, max_blue, game)) return 0;
    }

    return game_id;
}
}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    const int max_red = 12;
    const int max_green = 13;
    const int max_blue = 14;

    int count = 0;
    for (const string& line : raw_text) {
        count += is_valid_line(max_red, max_green, max_blue, line);
    }

    cout << "Answer: " << count << endl;
}
