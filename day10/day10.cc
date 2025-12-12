#include "../utils.hpp"
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <z3++.h>

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

uint64_t sol2_solve_schematic(const Schematic &schematic){
    z3::context context;
    auto solver = z3::solver(context);

    std::vector<z3::expr> button_variables;
    button_variables.reserve(schematic.buttons.size());
    for(uint32_t i = 0; i < schematic.buttons.size(); i++){
        button_variables.push_back(context.int_const(("a" + std::to_string(i)).c_str()));
        solver.add(button_variables[i] >= 0);
    } 

// vvars = [bvars[j] for j,button in enumerate(buttons) if i in button]
// solver.add(Sum(vvars) == v)
    for(uint32_t i = 0; i < schematic.joltages.size(); i++){
        z3::expr sum_expr = context.int_val(0);
        for(uint32_t j = 0; j < schematic.buttons.size(); j++){
            // for(uint32_t k = 0; i < schematic.joltages.size(); k++){
            if(schematic.buttons[j][i]){
                sum_expr = sum_expr + button_variables[j];
                // break;
            }
            // }
        }
        solver.add(sum_expr == schematic.joltages[i]);
    }

    int32_t min = INT32_MAX;
    while(solver.check() == z3::sat) {
        int32_t local_min = 0;
        auto model = solver.get_model();
        for(uint32_t i = 0; i < model.size(); i++){
            local_min += model.get_const_interp(model[i]).get_numeral_int();
        }

        z3::expr button_variables_sum = context.int_val(0);
        for(const auto &button_var: button_variables){
            button_variables_sum = button_variables_sum + button_var;
        }
        solver.add(button_variables_sum < local_min);
        min = std::min(min, local_min);
    }

    return min;
}

uint64_t sol2(const std::string &path){
    uint64_t sum = 0;
    const auto schematics = get_schematics(path);
    for(const auto &schematic: schematics){
        sum += sol2_solve_schematic(schematic);
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