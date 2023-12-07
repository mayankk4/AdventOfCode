#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day7.txt";

// Poker utils
namespace {
map<char, int> card_score = {
    {'A', 14},
    {'K', 13},
    {'Q', 12},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2},
    {'J', 1},
};

map<char, int> get_count_map(const string& hand) {
    map<char, int> counts;
    for (const char& card : hand) {
        counts[card]+=1;
    }
    return counts;
}

int max_repetitions(const string& hand) {
    const map<char, int> counts = get_count_map(hand);
    int max_count = 0;
    for (const auto& [k,v] : counts) {
        if (k == 'J') continue;
        max_count = max(max_count, v);
    }

    return (counts.find('J') != counts.end()) ? max_count + counts.at('J') : max_count;
}

int min_repetitions(const string& hand) {
    map<char, int> counts = get_count_map(hand);
    int min_count = 6;
    for (const auto& [k,v] : counts) {
        if (k == 'J') continue;
        min_count = min(min_count, v);
    }
    return min_count;
}

bool five_of_a_kind(const string& hand) {
    return (max_repetitions(hand) == 5);
}

bool four_of_a_kind(const string& hand) {
    return (max_repetitions(hand) == 4);
}

bool full_house(const string& hand) {
    return (max_repetitions(hand) == 3) && (min_repetitions(hand) == 2);
}

bool three_of_a_kind(const string& hand) {
    return (max_repetitions(hand) == 3);
}

bool two_pair(const string& hand) {
    const map<char, int> counts = get_count_map(hand);
    int pairs = 0;
    for (const auto& [k,v] : counts) {
        if (v == 2) pairs++;
    }
    return (pairs == 2);
}

bool one_pair(const string& hand) {
    return (hand.find('J') != string::npos) || ((max_repetitions(hand) == 2) && (min_repetitions(hand) == 1));
}

// true if left > right
bool has_higher_card_than(const string& left, const string& right) {
    for (int i = 0; i < left.size(); ++i) {
        if (left.at(i) == right.at(i)) continue;
        return card_score[left.at(i)] > card_score[right.at(i)];
    }
    return false;
}

// get score for the entire hand.
int get_hand_score(const string& hand) {
    if (five_of_a_kind(hand)) return 7;
    if (four_of_a_kind(hand)) return 6;
    if (full_house(hand)) return 5;
    if (three_of_a_kind(hand)) return 4;
    if (two_pair(hand)) return 3;
    if (one_pair(hand)) return 2;
    return 1;
}

} // namespace

struct Hand {
    string cards;
    int bid;

    bool operator>(const Hand& a) const{
        int score = get_hand_score(this->cards);
        int a_score = get_hand_score(a.cards);

        if (score == a_score) return has_higher_card_than(this->cards, a.cards);
        return score > a_score;
    }
};


namespace {

// index
vector<Hand> parse_input(const vector<string>& raw_text) {
    vector<Hand> hands;
    for (const string& line : raw_text) {
        auto splits = advent::str_split(line, ' ');
        Hand hand {.cards = splits[0], .bid = stoi(splits[1])};
        hands.push_back(hand);
    }
    return hands;
}

// sort
void hand_sort(vector<Hand>& hands) {
    for (int i = 0; i < hands.size(); i++) {
        for (int j = i + 1; j < hands.size(); j++) {
            if (hands[i] > hands[j]) {
                Hand temp = hands[i];
                hands[i] = hands[j];
                hands[j] = temp;
            }
        } 
    }
}
}  // namespace

int main() {
    const vector<string> raw_text = advent::read_file(kInputFile);

    vector<Hand> hands = parse_input(raw_text);

    hand_sort(hands);

    long ans = 0;
    for (int i = 0; i < hands.size(); ++i) {
        int rank = i + 1;
        ans += long(hands[i].bid * rank);
    }
    cout << "Ans : " << ans << endl;
}
