#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::string;
using ::std::cout;
using ::std::endl;
using ::std::vector;
using ::std::map;

using number_t = long long;
number_t stonumber_t(const string& s) {
    return stoll(s);
}
// vector of maps with <source, destination, offset>.
using data_t = vector<map<string, number_t>>;


const string kInputFile = "./data/day5.txt";

const string kSource = "source";
const string kDestination = "destination";
const string kOffset = "offset";

namespace {

// ------- ------- indexing ------- -------

void populate_seeds(const string& line, vector<number_t>& seeds) {
    vector<string> splits = advent::str_split(
        advent::str_split(line, ':')[1], ' ');
    for (const auto& s : splits) {
        if (!s.empty()) seeds.push_back(stonumber_t(s));
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
        // cout << "Populating key: " << k << endl;
        populate_index_map(raw_text, k, v);
    }
}

// ------- ------- retrieval ------- -------
number_t get_value_or_default(
    const number_t key,
    const data_t& data) {
    for (const std::map<string, number_t>& m : data) {
        number_t key_range_start = m.at(kSource);
        number_t key_range_end = m.at(kSource) + m.at(kOffset);
        if (key >= key_range_start && key < key_range_end) {
            // cout << "return range " << key_range_start << " and <" << key_range_end
            //     << " from the map source:" << m.at(kSource) << " offset: " << m.at(kOffset)
            //     << " destination: " << m.at(kDestination) << " for INPUT : " << key << endl; 
            return m.at(kDestination) + (key - m.at(kSource));
        }
    }
    // cout << "no key for " << key << endl; 
    return key;
}

number_t location_for_seed(
    const number_t seed,
     const std::map<string, data_t>& data) {
    // cout << "***" << endl;
    // cout << "seed " << seed << endl;
    number_t soil = get_value_or_default(seed, data.at("seed-to-soil"));
    // cout << "soil " << soil << endl;
    number_t fertilizer = get_value_or_default(soil, data.at("soil-to-fertilizer"));
    // cout << "fertilizer " << fertilizer << endl;
    number_t water = get_value_or_default(fertilizer, data.at("fertilizer-to-water"));
    // cout << "water " << water << endl;
    number_t light = get_value_or_default(water, data.at("water-to-light"));
    // cout << "light " << light << endl;
    number_t temperature = get_value_or_default(light, data.at("light-to-temperature"));
    // cout << "temperature " << temperature << endl;
    number_t humidity = get_value_or_default(temperature, data.at("temperature-to-humidity"));
    // cout << "humidity " << humidity << endl;
    number_t location = get_value_or_default(humidity, data.at("humidity-to-location"));
    // cout << "location " << location << endl;
    return location;
}

number_t get_min_location(
    const vector<number_t>& seeds,
    const std::map<string, data_t>& data) {
    // max-long
    number_t min_location = 9223372036854775807;
    for (const number_t seed : seeds) {
        min_location = std::min(min_location, location_for_seed(seed, data));
    }
    return min_location;
}

}  // namespace

int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<number_t> seeds;
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
    // cout << "-- debug maps --" << endl;
    // for (auto [k, v] : data) {
    //     cout << k << endl;
    //     for (auto map_data : v) {
    //         for (auto [k,v] : map_data) {
    //             cout << k << " " << v << ", ";
    //         }
    //         cout << endl;
    //     }
    //  }
    //  cout << endl;
    
    number_t ans = get_min_location(seeds, data);
    cout << "Answer : " << ans << endl;
}
