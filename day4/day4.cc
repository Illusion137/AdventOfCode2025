#include "../utils.hpp"
#include <cstdint>
#include <vector>

#define SAMPLE_1 13
#define SAMPLE_2 43

typedef std::vector<std::vector<char>> Diagram;

Diagram get_diagram(const std::string& path){
    return read_file_line_chars(path);
}

bool valid_adjacent_tile(const Diagram &diagram, uint32_t i, uint32_t j, uint32_t right, uint32_t down){
    return i + down >= 0 && i + down < diagram.size() && 
        j + right >= 0 && j + right < diagram[0].size() &&
        diagram[i + down][j + right] == '@';
}
uint8_t count_adjacent_tiles(const Diagram &diagram, uint32_t i, uint32_t j){
    uint8_t count = 0;
    if(valid_adjacent_tile(diagram, i, j, -1, -1)) count++;
    if(valid_adjacent_tile(diagram, i, j, -1, 0)) count++;
    if(valid_adjacent_tile(diagram, i, j, -1, 1)) count++;

    if(valid_adjacent_tile(diagram, i, j, 0, -1)) count++;
    if(valid_adjacent_tile(diagram, i, j, 0, 1)) count++;

    if(valid_adjacent_tile(diagram, i, j, 1, -1)) count++;
    if(valid_adjacent_tile(diagram, i, j, 1, 0)) count++;
    if(valid_adjacent_tile(diagram, i, j, 1, 1)) count++;
    return count;
}

int64_t sol1(const std::string& path){
    int64_t sum = 0;
    const Diagram diagram = get_diagram(path);
    for(uint32_t i = 0; i < diagram.size(); i++){
        for(uint32_t j = 0; j < diagram[0].size(); j++){
            if(diagram[i][j] != '@') continue;
            const uint8_t count = count_adjacent_tiles(diagram, i, j);
            if(count < 4) sum++;
        }
    }
    return sum;
}

int64_t sol2(const std::string& path){
    int64_t sum = 0;
    Diagram diagram = get_diagram(path);
    bool changed_any = true;
    while(changed_any){
        changed_any = false;
        for(uint32_t i = 0; i < diagram.size(); i++){
            for(uint32_t j = 0; j < diagram[0].size(); j++){
                if(diagram[i][j] != '@') continue;
                const uint8_t count = count_adjacent_tiles(diagram, i, j);
                if(count < 4) {
                    changed_any = true;
                    sum++;
                    diagram[i][j] = '.';
                }
            }
        }
    }


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