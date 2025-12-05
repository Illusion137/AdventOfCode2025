#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <print>

#define INPUT_FILE "input.txt"
#define SAMPLE_FILE "sample.txt"

static inline std::string read_file(std::string path){
    std::ifstream file{path};
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
    std::ifstream file{path};
    assert(file.is_open());
    std::string line;
    std::vector<std::string> lines;
    lines.reserve(4);
    while (std::getline(file, line)) {
        lines.emplace_back(std::move(line));
    }
    return lines;
}
static inline std::vector<std::vector<char>> read_file_line_chars(std::string path){
    const auto lines = read_file_lines(path);
    std::vector<std::vector<char>> lines_chars;
    lines_chars.reserve(lines.size());
    for(const auto &line: lines){
        std::vector<char> chars;
        chars.reserve(line.size());
        for(const char c: line){
            chars.emplace_back(c);
        }
        lines_chars.emplace_back(std::move(chars));
    }
    return lines_chars;
}

static inline std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1LL, 1000000000LL>>> TIME_START;
static inline void time_start(){
    TIME_START = std::chrono::high_resolution_clock::now();
}
static inline void time_end(){
    auto TIME_END = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = TIME_END - TIME_START;
    std::println("\033[0;34mProgram Time: {} ms\033[0m", duration.count());
}