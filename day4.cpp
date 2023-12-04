#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;
using advent::str_split;

const string kInputFile = "./data/day4.txt";

namespace {

// "  41 48 83 86 17 "
vector<int> vector_stoi(const string& input) {
    vector<string> points = str_split(input, ' ');
    vector<int> output;
    // output.reserve(points.size());
    points.erase(
        std::remove_if(
            points.begin(), points.end(),
            [](const string& p) {
                return p.empty();
            }
        ),
        points.end());
    std::transform(
        points.begin(), points.end(), std::inserter(output, output.end()),
        [](const string& p) {
            return stoi(p);
        }
    );
    return output;
}

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
int points_for_card_part1(const string& line) {
    if (line.empty()) return 0;
    auto winners_to_tickets = str_split(line, '|');
    vector<int> winning_points = vector_stoi(str_split(winners_to_tickets[0], ':')[1]);
    vector<int> ticket_points = vector_stoi(winners_to_tickets[1]);

    return std::count_if(
        ticket_points.begin(), ticket_points.end(),
        [&winning_points](int t) {
            return std::count(winning_points.begin(), winning_points.end(), t) > 0;
        });
}

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
int points_for_card_part2(const string& line, std::map<int, int>& line_multiplier) {
    if (line.empty()) return 0;
    auto winners_to_tickets = str_split(line, '|');
    vector<string> splits = str_split(str_split(winners_to_tickets[0], ':')[0], ' ');
    const int line_id = stoi(splits[splits.size() - 1]);
    vector<int> winning_points = vector_stoi(str_split(winners_to_tickets[0], ':')[1]);
    vector<int> ticket_points = vector_stoi(winners_to_tickets[1]);

    int winning_hits = std::count_if(
        ticket_points.begin(), ticket_points.end(),
        [&winning_points](int t) {
            return std::count(winning_points.begin(), winning_points.end(), t) > 0;
        });

    for (int i = 0; i < winning_hits; ++i) {
        if (line_multiplier.find(line_id + i + 1) != line_multiplier.end()) {
            line_multiplier[line_id + i + 1] += line_multiplier[line_id];
        }
    }

    return  line_multiplier[line_id];
}


}  // namespace


int main() {
    const vector<string> raw_text = advent::read_file(kInputFile);

    int count = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [](int acc, string line) {
            int hits = points_for_card_part1(line);
            if (hits == 0) return acc;
            return acc + int(pow(2, hits - 1));
        });

    cout << " ------ Part 1 ------" << endl;
    cout << "Answer: " << count << endl;

    cout << " ------ Part 2 ------" << endl;
    std::map<int, int> line_multiplier;
    for (int i = 1; i <= raw_text.size(); ++i) {
        line_multiplier[i] = 1;
    }
    int count_2 = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [&line_multiplier](int acc, string line) {
            return acc + points_for_card_part2(line, line_multiplier);
        });
    cout << "Answer: " << count_2 << endl;
}
