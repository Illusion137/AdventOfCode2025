#include "../utils.hpp"
#include <cmath>
#include <print>

#define DIAL_START 50
#define MAX 100.0

typedef int32_t Turn;
std::vector<Turn> get_turns(const std::string& path){
    const std::string contents = read_file(path);
    const auto lines = split_string(contents, "\n");
    std::vector<Turn> turns {};
    turns.reserve(lines.size());
    for(const auto &line: lines){
        const bool left = line[0] == 'L';
        const int32_t amount = std::atoi(line.substr(1).c_str());
        turns.emplace_back(left ? -amount : amount);
    }
    return turns;
}

// 0-99
int32_t calculate_dial(int32_t current, Turn turn){
    current += turn;
    current %= (int)MAX;
    if(current < 0){
        current *= -1;
        current = MAX - current;
    }
    return current;
}

int32_t calculate_dial(int32_t current, Turn turn, int32_t &zeros){
    int next_dial = current + turn;
    if(next_dial < 0){
        zeros = std::abs(
            std::ceil(next_dial / MAX) -
            std::ceil(current / MAX)
        );
    }
    else {
        zeros = std::abs(
            std::floor(next_dial / MAX) -
            std::floor(current / MAX)
        );
    };

    if(zeros == 0 && calculate_dial(current, turn) == 0) zeros = 1;
    return calculate_dial(current, turn);
}

int32_t sol1(const std::vector<Turn> &turns){
    int32_t sum = 0;
    int32_t dial = DIAL_START;
    for(const auto &turn: turns){
        dial = calculate_dial(dial, turn);
        if(dial == 0) sum++;
    }
    return sum;
}

int32_t sol2(const std::vector<Turn> &turns){
    int32_t sum = 0;
    int32_t dial = DIAL_START;
    for(const auto &turn: turns){
        int32_t zeros = 0;
        dial = calculate_dial(dial, turn, zeros);
        sum += zeros;
    }
    return sum;
}

int main(){
    std::println("Solution1-Sample: {}", sol1(get_turns("day1/sampleInput.txt")));
    std::println("Solution1-Real  : {}", sol1(get_turns("day1/input.txt")));
    std::println("");
    std::println("Solution2-Sample: {}", sol2(get_turns("day1/sampleInput.txt")));
    std::println("Solution2-Real  : {}", sol2(get_turns("day1/input2.txt")));
    return 0;
}