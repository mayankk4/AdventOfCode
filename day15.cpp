#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day15.txt";

namespace {
int hashx(const string& s) {
    int out = 0;
    for (const char c : s) {
        out += int(c);
        out *= 17;
        out %= 256;
    }
    return out;
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<string> input = advent::str_split(raw_text[0], ',');

    int ans = std::accumulate(
        input.begin(), input.end(), 0,
        [](int acc, const string& s) {
            return acc += hashx(s);
        }
    );

    cout << "Ans: " << ans << endl;
}
