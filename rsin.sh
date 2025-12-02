#!/bin/zsh
# Run single c-file
cd day$1

g++ -O3 -std=c++23 -Wall day$1.cc -o __program__;

if [ $? -eq 0 ]; then
    start=$(gdate +%s%N)
    ./__program__;
    end=$(gdate +%s%N)
    dtime=$(($end - $start))
    divisor=1000000
    ttime=$(($dtime / $divisor))
    runtime=$(echo "$ttime" | bc -l)
    rm ./__program__;
    echo "\033[0;34mFINISHED RUNNING in $runtime ms : day$1.cc\033[0m";
    read -n 1 -s -r -p "";
    clear;
fi