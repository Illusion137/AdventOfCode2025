#include "../utils.hpp"
#include <vector>

#define SAMPLE_1 2
#define SAMPLE_2 -1

typedef std::vector<std::string> Shape;
struct Region {
    uint16_t x;
    uint16_t y;
    std::array<uint16_t, 6> shape_quantities;
};
struct PresentsInfo {
    std::vector<Shape> shapes;
    std::vector<Region> regions;
};

PresentsInfo get_presents_info(const std::string &path){
    const auto contents = read_file(path);
    const auto splits = split_string(contents, "\n\n");
    PresentsInfo info;
    info.shapes.reserve(splits.size() - 1);
    for(uint32_t i = 0; i < splits.size() - 1; i++){
        auto shape_lines = split_string(splits[i], "\n");
        shape_lines.erase(shape_lines.begin());
        info.shapes.emplace_back(std::move(shape_lines));
    }
    const auto regions_splits = split_string(splits[splits.size() - 1], "\n");
    info.regions.reserve(regions_splits.size());
    for(const auto &region_string: regions_splits){
        const auto dimensions_quanities_split = split_string(region_string, ": ");
        const auto dimensions_split = split_string(dimensions_quanities_split[0], "x");
        const auto quantites_split = split_string(dimensions_quanities_split[1], " ");
        std::array<uint16_t, 6> shape_quantities;
        sassert(quantites_split.size(), 6);
        for(uint32_t i = 0; i < 6; i++){
            shape_quantities[i] = std::atoi(quantites_split[i].c_str());
        }
        info.regions.emplace_back(std::atoi(dimensions_split[0].c_str()), std::atoi(dimensions_split[1].c_str()), std::move(shape_quantities));
    }
    return info;
}

uint64_t sol1(const std::string &path){
    const auto presents_info = get_presents_info(path);
    uint64_t sum = 0;
    return sum;
}

uint64_t sol2(const std::string &path){
    const auto presents_info = get_presents_info(path);
    uint64_t sum = 0;
    return sum;
}

int main(){
    time_start();
    const auto sol1_sample_result = sol1(SAMPLE_FILE);
    std::println("Solution1-Sample: {}", sol1_sample_result);
    sassert(sol1_sample_result, SAMPLE_1);

    const auto sol1_real_result = sol1(INPUT_FILE);
    std::println("Solution1-Real  : {}", sol1_real_result);
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    sassert(sol2_sample_result, SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}