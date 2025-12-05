#include "../utils.hpp"
#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>

#define SAMPLE_1 3
#define SAMPLE_2 14

typedef std::pair<uint64_t, uint64_t> Range;

struct IngredientInfo {
    std::vector<Range> ingredient_id_ranges;
    std::vector<uint64_t> available_ingredient_ids;
};

IngredientInfo get_ingredient_info(const std::string& path){
    IngredientInfo info;
    const std::string contents = read_file(path);
    const auto ranges_ids_split = split_string(contents, "\n\n");
    const auto id_ranges_lines = split_string(ranges_ids_split[0], "\n");
    const auto ids_lines = split_string(ranges_ids_split[1], "\n");

    info.ingredient_id_ranges.reserve(id_ranges_lines.size());
    info.available_ingredient_ids.reserve(ids_lines.size());

    for(const auto &line: id_ranges_lines){
        const auto range_split = split_string(line, "-");
        info.ingredient_id_ranges.emplace_back(atoll(range_split[0].c_str()), atoll(range_split[1].c_str()));
    }
    for(const auto &line: ids_lines){
        info.available_ingredient_ids.emplace_back(atoll(line.c_str()));
    }

    return info;
}

inline bool in_range(uint64_t number, const Range &range){
    return number >= range.first && number <= range.second;
}

int64_t sol1(const std::string& path){
    const auto info = get_ingredient_info(path);
    return std::ranges::count_if(info.available_ingredient_ids, [&info](const auto id){
        return std::ranges::any_of(info.ingredient_id_ranges, [id](const auto &range){
            return in_range(id, range);
        });
    });
}

inline uint64_t range_magnitude(const Range &range){
    if(range.first == range.second && range.first == 0) return 0;
    return range.second - range.first + 1;
}
Range overlap_range(const Range &range1, const Range &range2){
    const uint64_t min = std::max(range1.first, range2.first);
    const uint64_t max = std::min(range1.second, range2.second);
    if(min > max) return {0, 0};
    const uint64_t min2 = std::min(range1.first, range2.first);
    const uint64_t max2 = std::max(range1.second, range2.second);
    return {min2, max2};
}

int64_t sol2(const std::string& path){
    int64_t sum = 0;
    auto info = get_ingredient_info(path);
    auto &ranges = info.ingredient_id_ranges;
    std::sort(ranges.begin(), ranges.end());
    for(uint32_t i = 0; i + 1 < ranges.size(); i++){
        const uint32_t j = i + 1;
        const auto overlap = overlap_range(ranges[i], ranges[j]);
        if(overlap.first == overlap.second) {
            sum += range_magnitude(ranges[i]);
            continue;
        }
        ranges[i] = overlap;
        ranges.erase(ranges.begin() + j);
        i--;
    }
    sum += range_magnitude(ranges[ranges.size() - 1]);

    return sum;
}

// 352509891817881
int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    assert(sol1_sample_result == SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result == SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}