#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;

const string kInputFile = "./data/dayX.txt";

namespace {


}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int count = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [](int acc, const string& line) {

        });

    cout << "Answer: " << count;
}
