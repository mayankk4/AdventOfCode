#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::cout;
using ::std::endl;
using ::std::string;
using ::std::map;
using ::std::set;
using ::std::vector;

const string kInputFile = "./data/dayX.txt";

namespace {
// ----- indexing -----
void index(const vector<string>& raw_text) {

}

// ----- retrieval -----



}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    // index
    index(raw_text)


    // retrieve
    int answer;


    cout << "Answer: " << answer << endl;
}
