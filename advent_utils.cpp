#include <vector>
#include <fstream>
#include <sstream>

using ::std::vector;
using ::std::string;
using ::std::cout;
using ::std::endl;
using ::std::ifstream;

namespace advent {

// Split a string by delimiter. Ignores " " and empty splits.
std::vector<std::string> str_split(const string& target, char c) {
	string temp;
	std::stringstream s { target };
	vector<string> result;
	while (std::getline(s, temp, c)) {
        if (!temp.empty() && (temp != " ")) result.push_back(temp);
	}
	return result;
}



// Returns true if key is a prefix of the input starting at given index.
bool is_prefix(const string& key, const string& input, const int index) {
    for (int i = 0; i < key.size(); ++i) {
        if (index + i > input.size() - 1) return false;
        if (key.at(i) != input.at(index + i)) return false;
    }
    return true;
}

// Reads a file and returns a vector of lines read.
vector<string> read_file(const string& file_path) {
    ifstream file;
    vector<string> result;

    cout << "Reading filename: " << file_path << endl;
    file.open(file_path);
    if (file.fail()) {
        cout << "File failed to open." << endl;
        return {};
    }

    while (!file.eof()) {
        string line;
        getline(file, line);
        result.push_back(line);
    }

    cout << "Finished reading " << result.size() << " lines." << endl;
    cout << "Printing a few lines from the file.." << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < 3 && i < result.size(); ++i) {
        cout << result[i] << endl;
    }
    cout << "--------------------------" << endl;

    file.close();
    return result;
}

}