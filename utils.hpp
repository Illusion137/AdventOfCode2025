#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <print>

#define INPUT_FILE "input.txt"
#define SAMPLE_FILE "sample.txt"

static inline std::string read_file(std::string path){
    std::ifstream file(path);
    assert(file.is_open());

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();    
}
static inline std::string read_input(){ return read_file(INPUT_FILE); }
static inline std::string read_sample(){ return read_file(SAMPLE_FILE); }

static inline std::vector<std::string> split_string(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }
    result.push_back(input.substr(start));
    return result;
}