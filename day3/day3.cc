#include "../utils.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define SAMPLE_1 357
#define SAMPLE_2 3121910778619

#define SOL2_BATTERIES 12

typedef std::vector<uint8_t> Bank;
typedef std::vector<Bank> Banks;
typedef std::pair<uint8_t, uint32_t> Max;
typedef std::vector<std::vector<Max>> Maxes;

Banks get_banks(const std::string& path){
    const std::string contents = read_file(path);
    std::vector<std::string> lines = split_string(contents, "\n");
    Banks banks;
    banks.reserve(lines.size());
    for(const auto &line: lines){
        Bank joltages{};
        joltages.reserve(line.size());
        for(const char c: line){
            joltages.push_back(c - '0');
        }
        banks.push_back(std::move(joltages));
    }
    return banks;
}
uint64_t get_joltage(uint8_t left, uint8_t right){
    return (left * 10) + right;
}
Maxes get_maxes(const Bank &bank){
    Maxes maxes;
    maxes.reserve(bank.size());
    for(uint32_t i = 0; i < bank.size(); i++){
        std::vector<Max> max_heap;
        max_heap.reserve(bank.size());
        for(uint32_t j = i; j < bank.size(); j++){
            max_heap.push_back(std::make_pair<uint8_t, uint32_t>((uint8_t)bank[j], (uint32_t)j));
        }
        std::sort(max_heap.begin(), max_heap.end(), [](const auto& a, const auto& b) {
            return a.first == b.first ? a.second < b.second : a.first > b.first;
        });
        std::unordered_set<uint8_t> seen_set;
        std::vector<Max> filtered_max_heap;
        filtered_max_heap.reserve(max_heap.size());
        for(const auto max: max_heap){
            if(!seen_set.contains(max.first)) filtered_max_heap.push_back(max);
            seen_set.insert(max.first);
        }
        maxes.push_back(filtered_max_heap);
    }
    return maxes;
}

int64_t sol1(const std::string& path){
    int64_t sum = 0;
    const auto banks = get_banks(path);
    for(const auto &bank: banks){
        uint64_t max_joltage = 0;
        for(uint32_t i = 0; i < bank.size(); i++){
            for(uint32_t j = i + 1; j < bank.size(); j++){
                uint64_t temp_joltage = get_joltage(bank[i], bank[j]);
                if(temp_joltage > max_joltage){
                    max_joltage = temp_joltage;
                }
            }
        }
        sum += max_joltage;
    }
    return sum;
}

std::unordered_map<uint32_t, uint64_t> dp_cache{};
uint64_t sol2_dp(const uint8_t bank_size, const Maxes &maxes, uint64_t in_base_joltage, uint8_t left, uint8_t used){
    if(left >= bank_size) return 0;
    if(used == SOL2_BATTERIES - 1) {
        return in_base_joltage + maxes[left][0].first;
    }
    const uint32_t cache_key = ((uint16_t)left << 16) | used;
    if(dp_cache.contains(cache_key)) return dp_cache.at(cache_key);

    uint64_t max_joltage = 0;
    for(const auto &[digit, digit_left]: maxes[left]){
        const uint64_t base_joltage = in_base_joltage + (digit * std::pow(10, SOL2_BATTERIES - 1 - used));
        const auto rec_max_joltage = sol2_dp(bank_size, maxes, base_joltage, digit_left + 1, used + 1);
        if(rec_max_joltage > max_joltage){
            max_joltage = rec_max_joltage;
        }
    }
    dp_cache.insert({cache_key, max_joltage});
    return max_joltage;
}

int64_t sol2(const std::string& path){
    int64_t sum = 0;
    const auto banks = get_banks(path);
    dp_cache.reserve(1'000);
    for(const auto &bank: banks){
        const auto maxes = get_maxes(bank);
        const auto max = sol2_dp(bank.size(), maxes, 0, 0, 0);
        sum += max;
        dp_cache.clear();
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