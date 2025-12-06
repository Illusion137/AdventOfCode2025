#include "../utils.hpp"
#include <cctype>
#include <cstdint>
#include <numeric>
#include <vector>
#include <ranges>

#define SAMPLE_1 4277556
#define SAMPLE_2 3263827

struct Equation {
    std::vector<uint64_t> numbers;
    bool addition;
};

std::vector<Equation> get_equations(const std::string& path){
    std::vector<Equation> equations;
    auto lines = read_file_lines(path);
    std::vector<std::vector<std::string>> lines_splits;
    lines_splits.reserve(lines.size());
    for(const auto &line: lines){
        auto split = split_string(line, " ");
        const auto split_filter = std::views::filter(split, [](std::string &str){
            str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
            return str.size() > 0;
        }) | std::ranges::to<std::vector>();
        lines_splits.emplace_back(split_filter);
    }

    for(uint32_t i = 0; i < lines_splits[0].size(); i++){
        Equation equation;
        equation.numbers.reserve(lines_splits.size() - 1);
        for(uint32_t j = 0; j < lines_splits.size() - 1; j++){
            equation.numbers.emplace_back(atoll(lines_splits[j][i].c_str()));
        }
        equation.addition = lines_splits[lines_splits.size() - 1][i][0] == '+';
        equations.emplace_back(std::move(equation));
    }
    return equations;
}
std::vector<Equation> get_cephalopods_equations(const std::string& path){
    std::vector<Equation> equations;
    auto lines = read_file_lines(path);

    while(true){
        Equation equation;
        equation.numbers.reserve(lines.size() - 1);

        auto &last_line = lines[lines.size() - 1];
        equation.addition = last_line[0] == '+';
        uint32_t width = 1;
        for(width = 1; width < last_line.size(); width++){
            if(!std::isspace(last_line[width])) break;
        }
        last_line = last_line.substr(width);

        std::vector<std::string> numbers_strings;
        for(uint32_t i = 0; i < lines.size() - 1; i++){
            auto &line = lines[i];
            numbers_strings.emplace_back(line.substr(0, last_line.size() != 0 ? width - 1 : width).c_str());
            line = line.substr(width);
        }

        for(uint32_t i = 0; i < numbers_strings[0].size(); i++){
            std::string column = "";
            for(uint32_t j = 0; j < numbers_strings.size(); j++){
                column += numbers_strings[j][i];
            }
            equation.numbers.emplace_back(std::atoll(column.c_str()));
        }

        equations.emplace_back(std::move(equation));
        if(last_line.size() == 0) break;
    }
    return equations;
}

uint64_t run_equation(const Equation &equation){
    if(equation.addition) return std::accumulate(equation.numbers.begin(), equation.numbers.end(), 0);
    uint64_t product = 1;
    for(const auto &number: equation.numbers){
        product *= number;
    }
    return product;
}

uint64_t sol1(const std::string& path){
    uint64_t sum = 0;
    const auto equations = get_equations(path);
    for(const auto &equation: equations){
        sum += run_equation(equation);
    }
    return sum;
}

uint64_t sol2(const std::string& path){
    uint64_t sum = 0;
    const auto equations = get_cephalopods_equations(path);
    for(const auto &equation: equations){
        sum += run_equation(equation);
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
    std::println("");

    const auto sol2_sample_result = sol2(SAMPLE_FILE);
    std::println("Solution2-Sample: {}", sol2_sample_result);
    assert(sol2_sample_result == SAMPLE_2);

    const auto sol2_real_result = sol2(INPUT_FILE);
    std::println("Solution2-Real  : {}", sol2_real_result);
    time_end();
    return 0;
}