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
    long total_time;
    long distance_record;
};

namespace {

// --- indexing
Race get_races(const vector<string>& raw_input) {
    vector<string> times = advent::str_split(advent::str_split(raw_input[0], ':')[1], ' ');
    vector<string> distances = advent::str_split(advent::str_split(raw_input[1], ':')[1], ' ');

    string time_string = "";
    string distance_string = "";
    for (const string& t : times) time_string += t;    
    for (const string& d : distances) distance_string += d;    

    Race race;
    race.total_time = stol(time_string);
    race.distance_record = stol(distance_string);

    // cout << "Input: total-time: " << race.total_time << " distance_record: " << race.distance_record << endl; 
    return race;
}

// --- retrieval

bool beats_record(const Race& race, const long time_pressed) {
    long speed = time_pressed;
    long distance = speed * (race.total_time - time_pressed);
    return distance > race.distance_record;
}

// for a race, returns the first index where we can start winning
long get_win_start_index(const Race& race) {
    long start_time = 0;
    long end_time = race.total_time / 2;

    // cout << "get_win_start_index: " << endl;
    while(start_time <= end_time) {
        // cout << "Search range : " << start_time << " " << end_time << endl;
        long mid = start_time + ((end_time - start_time) / 2);
        if (!beats_record(race, mid)) start_time = mid + 1;
        if (beats_record(race, mid)) {
            if (!beats_record(race, mid - 1)) return mid;
            end_time = mid - 1;
        }
    }
    return start_time;
}

// for a race, returns the first index where we can start winning
long get_win_end_index(const Race& race) {
    long start_time = race.total_time / 2;
    long end_time = race.total_time;

    // cout << "get_win_end_index: " << endl;
    while(start_time <= end_time) {
       // cout << "Search range : " << start_time << " " << end_time << endl;
       long mid = start_time + ((end_time - start_time) / 2);
        if (!beats_record(race, mid)) end_time = mid - 1;
        if (beats_record(race, mid)) {
            if (!beats_record(race, mid + 1)) return mid;
            start_time = mid + 1;
        }
    }

    return start_time;
}

// Possible approaches:
// 1. brute force
// 2. 2-pointer
// 3. derievative + binary search
long get_possibilities_to_win_race_binary_search(const Race& race) {
    long win_start_index = get_win_start_index(race);
    long win_end_index = get_win_end_index(race);

    return win_end_index - win_start_index + 1;
}

long get_possibilities_to_win_race_2_ptr(const Race& race) {
    long count = 0;
    const long start_time = 0;
    const long end_time = race.total_time;

    long win_start_index = 0;
    long win_end_index = 0;

    for (long time_pressed = 1; time_pressed < end_time; ++time_pressed) {
        if (beats_record(race, time_pressed)) {
            win_start_index = time_pressed;
            break;
        }
    }

    for (long time_pressed = end_time - 1; time_pressed > 0; --time_pressed) {
        if (beats_record(race, time_pressed)) {
            win_end_index = time_pressed;
            break;
        }        
    }

    return win_end_index - win_start_index + 1;
}

// Benchmark different algorithms.
void timed_run(const string& run_id, std::function<long(const Race& race)> func, const Race& race) {
    cout << run_id << endl;
    auto start = std::chrono::steady_clock::now();
    cout << "Answer: " << func(race) << endl;
    auto end = std::chrono::steady_clock::now();
    cout << "Time: " << std::chrono::duration <double, std::milli> (end - start).count() << " ms" << endl;
    cout << endl;
}


}  // namespace

#include <chrono>
#include <functional>

int main() {
    vector<string> raw_input = advent::read_file(kInputFile);

    Race race = get_races(raw_input);

    timed_run("Binary Search", get_possibilities_to_win_race_binary_search, race);
    timed_run("Two pointer", get_possibilities_to_win_race_2_ptr, race);
}
