#include "../utils.hpp"
#include <cmath>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#define SAMPLE_1 1227775554
#define SAMPLE_2 4174379265

typedef std::pair<uint64_t, uint64_t> Range;

std::vector<Range> get_ranges(const std::string& path){
    const std::string contents = read_file(path);
    const auto range_strings = split_string(contents, ",");
    std::vector<Range> ranges;
    ranges.reserve(range_strings.size());

    for(const auto &range_string: range_strings){
        const auto split_range_string = split_string(range_string, "-");
        ranges.emplace_back(
            std::atoll(split_range_string[0].c_str()),
            std::atoll(split_range_string[1].c_str())
        );
    }
    return ranges;
}

inline uint8_t count_digits(const uint64_t num){
    return std::floor(std::log10(num)) + 1;
}
inline uint8_t get_digit(uint64_t num, uint8_t digit){
    return ((uint64_t)(num / std::pow(10, digit))) % 10;
}

bool is_invalid_id_half(const uint64_t id, const uint32_t digits){
    const uint64_t exp = std::pow(10, digits / 2);
    return std::floor(id / exp) == id % exp;
}

bool is_invalid_id(const uint64_t id, const uint32_t splits, const uint32_t digits){
    const uint32_t split_size = digits / splits;
    const uint32_t slice = std::pow(10, split_size);
    const uint64_t first_slice = id % slice;
    for(uint32_t i = 1; i < splits; i++){
        const uint64_t other_slice = (uint64_t)(id / std::pow(10, i * split_size)) % slice;
        if(first_slice != other_slice) return false;
    }
    return true;
}

uint64_t sol1(const std::string& path){
    uint64_t sum = 0;
    const auto ranges = get_ranges(path);
    for(const auto &range: ranges){
        for(uint64_t id = range.first; id <= range.second; id++){
            const uint32_t digits = count_digits(id);
            if(digits % 2 != 0) {
                id = std::pow(10, digits);
                continue;
            }
            if(!is_invalid_id_half(id, digits)) continue;
            sum += id;
            id += std::pow(10, (digits / 2));
        }
    }
    return sum;
}

uint64_t sol2(const std::string& path){
    uint64_t sum = 0;
    const auto ranges = get_ranges(path);
    for(const auto &range: ranges){
        for(uint64_t id = range.first; id <= range.second; id++){
            const uint32_t digits = count_digits(id);
            for(uint32_t i = 2; i <= digits; i++){
                if(digits % i != 0) continue;
                if(!is_invalid_id(id, i, digits)) continue;
                sum += id;
                id += std::pow(10, (digits / 2));
                break;
            }
        }
    }
    return sum;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result == SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result == SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}