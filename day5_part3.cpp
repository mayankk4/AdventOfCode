#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <algorithm>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;
using ::std::vector;
using ::std::map;
using ::std::thread;

using number_t = long long;
number_t stonumber_t(const string& s) {
    return stoll(s);
}
// vector of maps with <source, destination, offset>.
using data_t = vector<map<string, number_t>>;

struct range_t {
    number_t start;  // inclusive
    number_t duration;
};

const string kInputFile = "./data/day5.txt";

const string kSource = "source";
const string kDestination = "destination";
const string kOffset = "offset";

namespace {

// ------- ------- indexing ------- -------

void populate_seeds(const string& line, vector<range_t>& seeds) {
    vector<string> splits = advent::str_split(
        advent::str_split(line, ':')[1], ' ');
    vector<string> clean_splits;
    std::copy_if(
        splits.begin(), splits.end(), std::back_inserter(clean_splits),
        [](string s) {return !s.empty();});
    for (int i = 0; i < clean_splits.size(); i+=2) {
        range_t r;
        r.start = stonumber_t(clean_splits[i]);
        r.duration = stonumber_t(clean_splits[i+1]);
        seeds.push_back(r);
    }
}

void line_to_map(
    const string& line,
    data_t& index_data) {
    vector<string> splits = advent::str_split(line, ' ');
    number_t key_start = stonumber_t(splits[1]);
    number_t value_start = stonumber_t(splits[0]);
    number_t num_iter = stonumber_t(splits[2]);
    std::map<string, number_t> index_map;
    index_map[kSource] = key_start;
    index_map[kDestination] = value_start;
    index_map[kOffset] = num_iter;
    index_data.push_back(index_map);
}

void populate_index_map(
    const vector<string>& raw_text,
    const string& index_name,
    data_t& index_data) {
    number_t start = 0;
    for (; start < raw_text.size(); ++start) {
        if (raw_text[start].find(index_name) != string::npos) break;
    }

    start++;

    while(!raw_text[start].empty() && start < raw_text.size()) {
        line_to_map(raw_text[start], index_data);
        start++;
    }
}


void index(
    const vector<string>& raw_text,
    std::map<string, data_t>& data) {
    for (auto& [k,v] : data) {
        populate_index_map(raw_text, k, v);
    }
}

// ------- ------- retrieval ------- -------

// input: 1 data map, all ranges to process
// output: output-ranges for the datamap.
vector<range_t> process_all_input_ranges_for_data_map(
    const map<string, number_t>& data_map,
    // reference passed down
    vector<range_t>& input_ranges_left_to_process) {
    // keeps track of ranges not covered by this data-map.
    vector<range_t> remaining_ranges;
    vector<range_t> output_ranges;

    for (const auto& curr_range : input_ranges_left_to_process) {
        number_t curr_range_start = curr_range.start;
        // non inclusive
        number_t curr_range_end = curr_range.start + curr_range.duration;

        number_t data_map_start = data_map.at(kSource);
        number_t data_map_end = data_map.at(kSource) + data_map.at(kOffset);

        if ((data_map_start > curr_range_end) || 
            (data_map_end < curr_range_start)) {
            remaining_ranges.push_back(curr_range);
            continue;
        }

        number_t intersection_start = std::max(curr_range_start, data_map_start);
        number_t intersection_end = std::min(curr_range_end, data_map_end);

        if (intersection_start > curr_range_start) {
            // add lhs range to process
            range_t lhs_range_left;
            lhs_range_left.start = curr_range_start;
            lhs_range_left.duration = intersection_start - curr_range_start;
            remaining_ranges.push_back(lhs_range_left);
        }

        if (intersection_end < curr_range_end) {
            // add rhs range to process
            range_t rhs_range_left;
            rhs_range_left.start = intersection_end;
            rhs_range_left.duration = curr_range_end - intersection_end;
            remaining_ranges.push_back(rhs_range_left);
        }

        // process the intersection, add output range.
        range_t output_range;
        output_range.start = data_map.at(kDestination) + (intersection_start - data_map.at(kSource));
        output_range.duration = intersection_end - intersection_start;
        output_ranges.push_back(output_range);
    }

    input_ranges_left_to_process.clear();
    for (range_t r : remaining_ranges) input_ranges_left_to_process.push_back(r);

    return output_ranges;
}

// data : vector of maps with <source, destination, offset>
// input: all input-ranges for the step.
// output: all output ranges for the step.
vector<range_t> get_all_output_ranges_for_step(
    const vector<range_t> input_ranges,
    const data_t& data) {
    vector<range_t> input_ranges_left_to_process(input_ranges); 
    vector<range_t> all_output_ranges_for_step;

    // process all input range for each entry in data map, keep track of remaining
    // range and keep appending the output ranges.
    for (const map<string, number_t>& data_map : data) {
        vector<range_t> output_ranges_for_one_data_map = process_all_input_ranges_for_data_map(
            data_map, input_ranges_left_to_process);
        all_output_ranges_for_step.insert(
            all_output_ranges_for_step.end(),
            output_ranges_for_one_data_map.begin(),
            output_ranges_for_one_data_map.end());
    }

    if (!input_ranges_left_to_process.empty()) {
        all_output_ranges_for_step.insert(
            all_output_ranges_for_step.end(),
            input_ranges_left_to_process.begin(),
            input_ranges_left_to_process.end());        
    }

    return all_output_ranges_for_step;
}

number_t process_seed_ranges(
    const vector<range_t>& seed_ranges,
    const std::map<string, data_t>& data) {
    vector<range_t> soil = get_all_output_ranges_for_step(seed_ranges, data.at("seed-to-soil"));
    vector<range_t> fertilizer = get_all_output_ranges_for_step(soil, data.at("soil-to-fertilizer"));
    vector<range_t> water = get_all_output_ranges_for_step(fertilizer, data.at("fertilizer-to-water"));
    vector<range_t> light = get_all_output_ranges_for_step(water, data.at("water-to-light"));
    vector<range_t> temperature = get_all_output_ranges_for_step(light, data.at("light-to-temperature"));
    vector<range_t> humidity = get_all_output_ranges_for_step(temperature, data.at("temperature-to-humidity"));
    vector<range_t> location = get_all_output_ranges_for_step(humidity, data.at("humidity-to-location"));

    // Get the min number from the location ranges output.
    number_t min_location;
    for (const range_t& range : location) {
        min_location = std::min(min_location, range.start);
    }
    return min_location;
}

}  // namespace

int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<range_t> seeds;
    populate_seeds(raw_text[0], seeds);

    std::map<string, data_t> data {
        {"seed-to-soil", data_t()},
        {"soil-to-fertilizer", data_t()},
        {"fertilizer-to-water", data_t()},
        {"water-to-light", data_t()},
        {"light-to-temperature", data_t()},
        {"temperature-to-humidity", data_t()},
        {"humidity-to-location", data_t()},
    };

    index(raw_text, data);

    number_t ans = process_seed_ranges(seeds, data);
    cout << "Answer : " << ans << endl;
}
