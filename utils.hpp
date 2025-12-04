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

static inline std::vector<std::string> read_file_lines(std::string path){
    const auto contents = read_file(path);
    return split_string(path, "\n");
}
static inline std::vector<std::vector<char>> read_file_line_chars(std::string path){
    const auto contents = read_file(path);
    const auto lines = split_string(contents, "\n");
    std::vector<std::vector<char>> lines_chars;
    lines_chars.reserve(lines.size());
    for(const auto &line: lines){
        std::vector<char> chars;
        chars.reserve(line.size());
        for(const char c: line){
            chars.push_back(c);
        }
        lines_chars.push_back(chars);
    }
    return lines_chars;
}