#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day6.txt";

struct Race {
    int total_time;
    int distance_record;
};

namespace {

// --- indexing
vector<Race> get_races(const vector<string>& raw_input) {
    vector<string> times = advent::str_split(advent::str_split(raw_input[0], ':')[1], ' ');
    vector<string> distances = advent::str_split(advent::str_split(raw_input[1], ':')[1], ' ');

    vector<Race> races;
    for (int i = 0; i < times.size(); ++i) {
        Race race;
        race.total_time = stoi(times[i]);
        race.distance_record = stoi(distances[i]);
        races.push_back(race);
    }
    return races;
}

// --- retrieval

bool beats_record(const Race& race, const int time_pressed) {
    int speed = time_pressed;
    int distance = speed * (race.total_time - time_pressed);
    return distance > race.distance_record;
}

// Possible approaches:
// 1. brute force
// 2. 2-pointer [used here]
// 3. binary search
int get_possibilities_to_win_race(const Race& race) {
    int count = 0;
    const int start_time = 0;
    const int end_time = race.total_time;

    int win_start_index = 0;
    int win_end_index = 0;

    for (int time_pressed = 1; time_pressed < end_time; ++time_pressed) {
        if (beats_record(race, time_pressed)) {
            win_start_index = time_pressed;
            break;
        }
    }

    for (int time_pressed = end_time - 1; time_pressed > 0; --time_pressed) {
        if (beats_record(race, time_pressed)) {
            win_end_index = time_pressed;
            break;
        }        
    }

    return win_end_index - win_start_index + 1;
}

}  // namespace


int main() {
    vector<string> raw_input = advent::read_file(kInputFile);
    vector<Race> races = get_races(raw_input);

    int ans = std::accumulate(
        races.begin(), races.end(), 1,
        [](int acc, const Race& race) {
            return acc *= get_possibilities_to_win_race(race);
        });

    cout << "Answer part 1: " << ans << endl;
}
