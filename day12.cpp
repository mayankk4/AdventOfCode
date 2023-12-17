#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

#include "advent_utils.cpp"
#include "stl_utils.cpp"

const string kInputFile = "./data/day12.txt";

const char kOperational = '.';
const char kDamaged = '#';
const char kUnknown = '?';

namespace {

bool is_valid_case(const string& condition_record, const vector<int>& damaged_log) {
    int damaged_log_index = 0;

    for (int record_index = 0;
         record_index < condition_record.size();
         record_index++) {
            char curr_char = condition_record.at(record_index);
            if (curr_char != kDamaged) continue;

            // kDamaged
            int count = 0;
            bool end = false;
            while((curr_char == kDamaged) && !end) {
                count++;
                record_index++;
                if (record_index >= condition_record.size()) {
                    end = true;
                } else {
                    curr_char = condition_record.at(record_index);
                }
            }
            if (count > 0 && damaged_log_index >= damaged_log.size()) return false;
            if (count > 0 && damaged_log[damaged_log_index] != count) return false;
            damaged_log_index++;
    }

    // saw all damaged entries.
    return damaged_log_index == damaged_log.size();
}

int count_valid_possibilities(
    const string& condition_record, const vector<int>& damaged_log) {
    bool base_case = true;
    int i;
    for (i = 0; i < condition_record.size(); ++i) {
        if (condition_record.at(i) == kUnknown) {
            base_case = false;
            break;
        }
    }

    if (base_case) {
        return is_valid_case(condition_record, damaged_log) ? 1 : 0;
    }

    string modified_record_operational = condition_record;
    modified_record_operational[i] = kOperational;

    string modified_record_damaged = condition_record;
    modified_record_damaged[i] = kDamaged;

    return (
        count_valid_possibilities(modified_record_operational, damaged_log) + 
        count_valid_possibilities(modified_record_damaged, damaged_log)
    );
}

// ???.### 1,1,3
int process_line(const string& s) {
    vector<string> splits = advent::str_split(s, ' ');
    string condition_record = splits[0];
    vector<string> damaged_s = advent::str_split(splits[1], ',');
    vector<int> damaged_log;
    std::transform(
        damaged_s.begin(), damaged_s.end(), std::back_inserter(damaged_log),
        [](const string& s) { return stoi(s);}
    );

    return count_valid_possibilities(condition_record, damaged_log);
}

}  // namespace


int main() {
    vector<string> raw_text = advent::read_file(kInputFile);

    int ans = std::accumulate(
        raw_text.begin(), raw_text.end(), 0,
        [](int acc, const string& s) {
            int possibilities = process_line(s);
            cout << s << " : " << possibilities << endl;
            return acc += possibilities;
        }
    );

    cout << "Ans : " << ans << endl; 
}
