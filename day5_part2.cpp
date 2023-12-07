#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <algorithm>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

using ::std::thread;

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

void populate_seeds(const string& line, vector<vector<number_t>>& seeds) {
    vector<string> splits = advent::str_split(
        advent::str_split(line, ':')[1], ' ');
    vector<string> clean_splits;
    std::copy_if(
        splits.begin(), splits.end(), std::back_inserter(clean_splits),
        [](string s) {return !s.empty();});
    for (int i = 0; i < clean_splits.size(); i+=2) {
        vector<number_t> v {
            stonumber_t(clean_splits[i]),
            stonumber_t(clean_splits[i+1])
        };
        seeds.push_back(v);
    }
}

void line_to_map(
    const string& line,
    data_t& index_data) {
    vector<string> splits = advent::str_split(line, ' ');
    number_t key_start = stonumber_t(splits[1]);
    number_t value_start = stonumber_t(splits[0]);
    number_t num_iter = stonumber_t(splits[2]);
    map<string, number_t> index_map;
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
    map<string, data_t>& data) {
    for (auto& [k,v] : data) {
        // cout << "Populating key: " << k << endl;
        populate_index_map(raw_text, k, v);
    }
}

// ------- ------- retrieval ------- -------
number_t get_value_or_default(
    const number_t key,
    const data_t& data) {
    for (const map<string, number_t>& m : data) {
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
    const map<string, data_t>& data) {
    number_t soil = get_value_or_default(seed, data.at("seed-to-soil"));
    number_t fertilizer = get_value_or_default(soil, data.at("soil-to-fertilizer"));
    number_t water = get_value_or_default(fertilizer, data.at("fertilizer-to-water"));
    number_t light = get_value_or_default(water, data.at("water-to-light"));
    number_t temperature = get_value_or_default(light, data.at("light-to-temperature"));
    number_t humidity = get_value_or_default(temperature, data.at("temperature-to-humidity"));
    number_t location = get_value_or_default(humidity, data.at("humidity-to-location"));
    return location;
}

number_t get_min_location(
    const vector<vector<number_t>>& seeds,
    const map<string, data_t>& data) {
    vector<thread> threads;
    vector<number_t> min_locations;
    int thread_id = 0;
    for (const auto& seed_range : seeds) {
        thread_id++;
        threads.push_back(thread(
            [thread_id, &min_locations, &seed_range, &data]() {
            number_t min_location = LLONG_MAX;
            set<int> flushed_data;
            for (number_t i = seed_range[0]; i < seed_range[0] + seed_range[1]; ++i) {
                int percent = ((i - seed_range[0]) * 100) / seed_range[1];
                if (flushed_data.find(percent) == flushed_data.end()) {
                    flushed_data.insert(percent);
                    string debug = 
                        "Thread-" + std::to_string(thread_id) +
                        " finished: " + std::to_string(percent) + "%";
                    cout << debug << endl;
                }
                min_location = min(min_location, location_for_seed(i, data));
            }
            min_locations.push_back(min_location);
        }));
    }

    for (thread& t : threads) t.join();

    number_t min_location = LLONG_MAX;
    for (const number_t m : min_locations) {
        min_location = min(min_location, m);
    }

    return min_location;
}

}  // namespace

int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    vector<vector<number_t>> seeds;
    populate_seeds(raw_text[0], seeds);

    map<string, data_t> data {
        {"seed-to-soil", data_t()},
        {"soil-to-fertilizer", data_t()},
        {"fertilizer-to-water", data_t()},
        {"water-to-light", data_t()},
        {"light-to-temperature", data_t()},
        {"temperature-to-humidity", data_t()},
        {"humidity-to-location", data_t()},
    };

    index(raw_text, data);

    number_t ans = get_min_location(seeds, data);
    cout << "Answer : " << ans << endl;
}
