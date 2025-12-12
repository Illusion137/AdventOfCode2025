#include "../utils.hpp"

#define SAMPLE_1 2
#define SAMPLE_2 -1

uint64_t sol1(const std::string &path){
    uint64_t sum = 0;
    return sum;
}

uint64_t sol2(const std::string &path){
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