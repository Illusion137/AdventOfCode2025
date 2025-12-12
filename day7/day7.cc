#include "../utils.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define SAMPLE_1 21
#define SAMPLE_2 40

#define SPLITTER '^'

std::pair<uint32_t, std::vector<std::string>> get_tachyon_lines(const std::string& path){
    const auto lines = read_file_lines(path);
    const auto start_index = lines[0].find('S');
    return {start_index, lines};
}

uint64_t sol1_rec(uint32_t beam_index, uint32_t line_no, std::vector<std::string> &lines, std::unordered_set<uint64_t> &visited){
    if(line_no == lines.size() - 1) return 0;
    if(beam_index >= lines[0].size()) return 0;
    if(beam_index < 0) return 0;
    for(uint32_t i = line_no; i < lines.size(); i++){
        const uint64_t visited_key = (uint64_t)i << 32 | beam_index;
        if(visited.contains(visited_key)) break;
        if(lines[i][beam_index] != SPLITTER) continue;
        visited.insert(visited_key);
        return 1 + sol1_rec(beam_index - 1, i, lines, visited)
            + sol1_rec(beam_index + 1, i, lines, visited);
    }
    return 0;
}

uint64_t sol1(const std::string& path){
    auto tachyon_lines = get_tachyon_lines(path);
    std::unordered_set<uint64_t> visited;
    return sol1_rec(tachyon_lines.first, 0, tachyon_lines.second, visited);
}

std::unordered_map<uint64_t, uint64_t> sol2_dp_cache;
uint64_t sol2_dp(uint32_t beam_index, uint32_t line_no, std::vector<std::string> &lines){
    const uint64_t cache_key = (uint64_t)line_no << 32 | beam_index;
    if(sol2_dp_cache.contains(cache_key)) return sol2_dp_cache.at(cache_key);
    if(beam_index >= lines[0].size()) return 0;
    if(beam_index < 0) return 0;
    for(uint32_t i = line_no; i < lines.size(); i++){
        if(lines[i][beam_index] != SPLITTER) continue;
        const uint64_t result = sol2_dp(beam_index - 1, i, lines)
            + sol2_dp(beam_index + 1, i, lines);
        sol2_dp_cache.insert({cache_key, result});
        return result;
    }
    sol2_dp_cache.insert({cache_key, 1});
    return 1;
}

uint64_t sol2(const std::string& path){
    auto tachyon_lines = get_tachyon_lines(path);
    sol2_dp_cache.clear();
    return sol2_dp(tachyon_lines.first, 0, tachyon_lines.second);
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