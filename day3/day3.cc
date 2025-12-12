#include "../utils.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_set>

#define SAMPLE_1 357
#define SAMPLE_2 3121910778619

#define SOL2_BATTERIES 12

typedef std::vector<uint8_t> Bank;
typedef std::vector<Bank> Banks;
typedef std::pair<uint8_t, uint32_t> Max;
typedef std::vector<std::vector<Max>> Maxes;

Banks get_banks(const std::string& path){
    const std::vector<std::string> lines = read_file_lines(path);
    Banks banks;
    banks.reserve(lines.size());
    for(const auto &line: lines){
        Bank joltages{};
        joltages.reserve(line.size());
        for(const char c: line){
            joltages.emplace_back(c - '0');
        }
        banks.emplace_back(std::move(joltages));
    }
    return banks;
}
uint64_t get_joltage(uint8_t left, uint8_t right){
    return (left * 10) + right;
}
Maxes get_maxes(const Bank &bank){
    Maxes maxes;
    std::unordered_set<uint8_t> seen_set;
    maxes.reserve(bank.size());
    seen_set.reserve(bank.size());
    for(uint32_t i = 0; i < bank.size(); i++){
        std::vector<Max> max_heap;
        max_heap.reserve(bank.size() - i);
        for(uint32_t j = i; j < bank.size(); j++){
            if(!seen_set.contains(bank[j]))
                max_heap.emplace_back((uint8_t)bank[j], (uint32_t)j);
            else seen_set.insert(bank[j]);
        }
        std::sort(max_heap.begin(), max_heap.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
        maxes.emplace_back(std::move(max_heap));
        seen_set.clear();
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

uint64_t sol2_rec(const uint8_t bank_size, const Maxes &maxes, uint64_t in_base_joltage, uint8_t left, uint8_t used){
    if(left >= bank_size) return 0;
    if(used == SOL2_BATTERIES - 1) return in_base_joltage + maxes[left][0].first;

    for(const auto &[digit, digit_left]: maxes[left]){
        const uint64_t base_joltage = in_base_joltage + (digit * std::pow(10, SOL2_BATTERIES - 1 - used));
        const auto rec_max_joltage = sol2_rec(bank_size, maxes, base_joltage, digit_left + 1, used + 1);
        if(rec_max_joltage != 0) return rec_max_joltage;
    }
    return 0;
}

int64_t sol2(const std::string& path){
    int64_t sum = 0;
    const auto banks = get_banks(path);
    for(const auto &bank: banks){
        const auto maxes = get_maxes(bank);
        const auto max = sol2_rec(bank.size(), maxes, 0, 0, 0);
        sum += max;
    }
    return sum;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result, SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}