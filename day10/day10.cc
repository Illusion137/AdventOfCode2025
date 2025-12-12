#include "../utils.hpp"
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define SAMPLE_1 7
#define SAMPLE_2 33

#define LIGHT_DIAGRAM_ON '#'

struct Schematic {
    std::bitset<10> light_diagram;
    std::vector<std::bitset<10>> buttons;
    std::vector<uint16_t> joltages;
};

std::bitset<10> get_button_set(const std::string &buttons_string){
    std::bitset<10> buttons;
    const auto no_paren_string = buttons_string.substr(1, buttons_string.size() - 2);
    const auto split_digits = split_string(no_paren_string, ",");
    for(const auto &split_digit: split_digits){
        buttons.set(std::atoi(split_digit.c_str()));
    }
    return buttons;
}

std::vector<Schematic> get_schematics(const std::string &path){
    const auto lines = read_file_lines(path);
    std::vector<Schematic> data;
    data.reserve(lines.size());
    for(const auto &line: lines){
        const auto splits = split_string(line, " ");
        std::bitset<10> light_diagram;
        std::vector<std::bitset<10>> buttons;
        std::vector<uint16_t> joltages;

        for(uint32_t i = 1; i < splits[0].size() - 1; i++){
            light_diagram.set(i - 1, splits[0][i] == LIGHT_DIAGRAM_ON);
        }

        buttons.reserve(splits.size() - 2);
        for(uint32_t i = 1; i < splits.size() - 1; i++){
            buttons.emplace_back(get_button_set(splits[i]));
        }

        const auto no_bracket_joltage_string = splits[splits.size() - 1].substr(1, splits[splits.size() - 1].size() - 2);
        const auto joltage_string_splits = split_string(no_bracket_joltage_string, ",");
        joltages.reserve(joltage_string_splits.size());
        for(const auto &joltage_string: joltage_string_splits){
            joltages.emplace_back(std::atoi(joltage_string.c_str()));
        }

        data.emplace_back(std::move(light_diagram), std::move(buttons), std::move(joltages));
    }
    return data;
}

uint64_t get_cache_key_v1(const std::bitset<10> &light_diagram, uint32_t presses){
    return (light_diagram.to_ullong() << 32) | presses;
}
std::unordered_map<uint64_t, uint32_t> sol1_dp_cache;
uint16_t sol1_dp(const Schematic &schematic, std::bitset<10> light_diagram, uint32_t presses){
    const uint64_t cache_key = get_cache_key_v1(light_diagram, presses);
    if(sol1_dp_cache.contains(cache_key)) return sol1_dp_cache.at(cache_key);
    if(schematic.light_diagram == light_diagram) return presses;
    if(presses >= 10) return UINT16_MAX;
    uint32_t min_presses = UINT16_MAX;
    for(uint32_t i = 0; i < schematic.buttons.size(); i++){
        const auto toggle_diagram = light_diagram ^ schematic.buttons[i];
        const uint32_t temp_solution = sol1_dp(schematic, toggle_diagram, presses + 1);
        min_presses = std::min(min_presses, temp_solution);
    }
    sol1_dp_cache.insert({cache_key, min_presses});
    return min_presses;
}

uint64_t sol1(const std::string &path){
    uint64_t sum = 0;
    const auto schematics = get_schematics(path);
    for(const auto &schematic: schematics){
        sol1_dp_cache.clear();
        sum += sol1_dp(schematic, std::bitset<10>{0}, 0);
    }
    return sum;
}

uint64_t hash_joltages(const std::vector<uint16_t> &joltages){
    // std::size_t seed = joltages.size();
    // for(auto x : joltages) {
    //     x = ((x >> 16) ^ x) * 0x45d9f3b;
    //     x = ((x >> 16) ^ x) * 0x45d9f3b;
    //     x = (x >> 16) ^ x;
    //     seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    // }
    // return seed;
    uint64_t h = 0;
    for (uint16_t x : joltages) {
        h = h * 1315423911u + x;
    }
    return h;
}

uint16_t sol2_schematic(const Schematic &schematic){
    std::queue<std::vector<uint16_t>> joltages_queue;
    std::unordered_map<uint64_t, uint16_t> joltages_cache;
    joltages_cache.reserve(100000);

    joltages_queue.push(std::vector<uint16_t>(schematic.joltages.size(), 0));
    joltages_cache[hash_joltages(joltages_queue.front())] = 0;

    while(!joltages_queue.empty()){
        const auto current_joltages = joltages_queue.front();
        joltages_queue.pop();

        const uint16_t current_joltage_presses = joltages_cache[hash_joltages(current_joltages)];
        if(schematic.joltages == current_joltages) {
            std::println("{}", current_joltage_presses);
            return current_joltage_presses;
        }

        for(const auto &button: schematic.buttons){
            std::vector<uint16_t> next = current_joltages;

            for(int i = 0; i < schematic.joltages.size(); i++) {
                if(!button[i]) continue;
                next[i]++;
                if(next[i] > schematic.joltages[i]) goto nope;
            }
            goto insert_cache;
            nope: continue;

            insert_cache:
            uint64_t cache_key = hash_joltages(next);
            if (joltages_cache.contains(cache_key)) continue;
            joltages_cache[cache_key] = current_joltage_presses + 1;
            joltages_queue.push(std::move(next));
        }
    }
    return -1;
}

static std::unordered_map<uint64_t, uint16_t> memo;

// Reverse DFS (goal → zero)
uint16_t dfs_rev(
    const std::vector<uint16_t> &target,
    const std::vector<std::bitset<10>> &buttons,
    const std::vector<uint16_t> &state
) {
    uint64_t key = hash_joltages(state);
    if (auto it = memo.find(key); it != memo.end())
        return it->second;

    // Check if reached zero state
    bool all_zero = true;
    for (uint16_t x : state)
        if (x != 0) { all_zero = false; break; }
    if (all_zero) {
        memo[key] = 0;
        return 0;
    }

    uint16_t best = std::numeric_limits<uint16_t>::max();

    // Try subtracting each button
    for (const auto &btn : buttons) {
        std::vector<uint16_t> next = state;
        bool valid = true;

        for (size_t i = 0; i < state.size(); i++) {
            if (btn[i]) {
                if (next[i] == 0) { // cannot go negative
                    valid = false;
                    break;
                }
                next[i]--;
            }
        }

        if (!valid) continue;

        uint16_t r = dfs_rev(target, buttons, next);
        if (r != std::numeric_limits<uint16_t>::max()) {
            if (r + 1 < best) best = r + 1;
        }
    }

    memo[key] = best;
    return best;
}

using State = std::array<uint16_t,10>;

inline uint64_t hash_state(const State &s, size_t n) {
    uint64_t h = 0x9e3779b97f4a7c15ULL;
    for(size_t i = 0; i < n; i++) {
        uint64_t x = s[i];
        x ^= x >> 33;
        x *= 0xff51afd7ed558ccdULL;
        x ^= x >> 33;
        x *= 0xc4ceb9fe1a85ec53ULL;
        x ^= x >> 33;
        h ^= x + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
    }
    return h;
}

uint16_t solve_schematic(const Schematic &schematic) {
    const size_t n = schematic.joltages.size();
    State start{};
    for(size_t i = 0; i < n; i++) start[i] = schematic.joltages[i];

    std::queue<std::pair<State,uint16_t>> q;
    std::unordered_set<uint64_t> visited;
    q.push({start,0});
    visited.insert(hash_state(start,n));

    while(!q.empty()) {
        auto [cur, presses] = q.front(); q.pop();

        // Check goal: all zeros
        bool done = true;
        for(size_t i = 0; i < n; i++) if(cur[i] != 0) { done = false; break; }
        if(done) return presses;

        // Try subtracting each button
        for(const auto &btn : schematic.buttons) {
            State next = cur;
            bool valid = true;
            for(size_t i = 0; i < n; i++) {
                if(btn[i]) {
                    if(next[i] == 0) { valid = false; break; }
                    next[i]--;
                }
            }
            if(!valid) continue;

            uint64_t h = hash_state(next,n);
            if(!visited.contains(h)) {
                visited.insert(h);
                q.push({next, static_cast<uint16_t>(presses+1)});
            }
        }
    }
    return std::numeric_limits<uint16_t>::max(); // unreachable
}


uint64_t sol2(const std::string &path){
    uint64_t sum = 0;
    const auto schematics = get_schematics(path);
    for(const auto &schematic: schematics){
        memo.clear();
        sum += solve_schematic(schematic);
        std::println("{}", sum);
    }
    return sum;
}

int main(){
    time_start();
    // const auto sol1_sample_result = sol1(SAMPLE_FILE);
    // std::println("Solution1-Sample: {}", sol1_sample_result);
    // assert(sol1_sample_result, SAMPLE_1);

    // const auto sol1_real_result = sol1(INPUT_FILE);
    // std::println("Solution1-Real  : {}", sol1_real_result);
    // std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}