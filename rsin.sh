#!/bin/zsh
# Run single c-file

g++ -std=c++23 -Wall $1 -o __program__;

if [ $? -eq 0 ]; then
    start=$(gdate +%s%3N)
    ./__program__;
    end=$(gdate +%s%3N)
    
    runtime=$(echo "$end - $start" | bc -l)
    rm ./__program__;
    echo "\033[0;34mFINISHED RUNNING in $runtime ms : $1\033[0m";
    read -n 1 -s -r -p "";
    clear;
fi