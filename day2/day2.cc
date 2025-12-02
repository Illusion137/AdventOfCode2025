#include "../utils.hpp"
#include <cstdint>
#include <print>
#include <string>

#define SAMPLE_1 1227775554
#define SAMPLE_2 4174379265

std::vector<std::pair<uint64_t, uint64_t>> get_ranges(const std::string& path){
    const std::string contents = read_file(path);
    const auto range_strings = split_string(contents, ",");
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    ranges.reserve(range_strings.size());

    for(const auto &range_string: range_strings){
        const auto split_range_string = split_string(range_string, "-");
        ranges.push_back(
            std::make_pair<uint64_t, uint64_t>(
                std::atoll(split_range_string[0].c_str()),
                std::atoll(split_range_string[1].c_str())
            )
        );
    }
    return ranges;
}

bool is_invalid_id(uint64_t id, uint32_t splits){
    const std::string str = std::to_string(id);
    if(str.size() % splits != 0) return false;
    uint32_t split_size = str.size() / splits;
    std::string first_str = str.substr(0, split_size);
    uint32_t same_strs = 1;
    for(uint32_t i = 1; i < splits; i++){
        if(first_str == str.substr(split_size * i, split_size))
            same_strs++;
    }
    return same_strs == splits;
}

uint64_t sol1(const std::string& path){
    uint64_t sum = 0;
    const auto &ranges = get_ranges(path);
    for(const auto &range: ranges){
        for(uint64_t id = range.first; id <= range.second; id++){
            if(is_invalid_id(id, 2)) sum += id;
        }
    }
    return sum;
}

uint64_t sol2(const std::string& path){
    uint64_t sum = 0;
    const auto &ranges = get_ranges(path);
    for(const auto &range: ranges){
        for(uint64_t id = range.first; id <= range.second; id++){
            const uint32_t digits = std::to_string(id).size();
            for(uint32_t i = 2; i <= digits; i++){
                if(is_invalid_id(id, i)) {
                    sum += id;
                    break;
                }
            }
        }
    }
    return sum;
}

int main(){
    std::println("Solution1-Sample: {}", sol1(SAMPLE_FILE));
    assert(sol1(SAMPLE_FILE) == SAMPLE_1);
    std::println("Solution1-Real  : {}", sol1(INPUT_FILE));
    std::println("");
    std::println("Solution2-Sample: {}", sol2(SAMPLE_FILE));
    assert(sol2(SAMPLE_FILE) == SAMPLE_2);
    std::println("Solution2-Real  : {}", sol2(INPUT_FILE));
    return 0;
}