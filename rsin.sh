#!/bin/zsh
# Run single c-file
cd day$1

g++ -O3 -std=c++23 -Wall day$1.cc -o __program__;

if [ $? -eq 0 ]; then
    TIMEFMT='%U'
    TIMEFORMAT='%U'

    (time ./__program__) 2>time.tmp;
    cat time.tmp | awk '{print int($1*1000)}' > user_ms.tmp
    ms_text=$(cat user_ms.tmp)
    echo "\033[0;34mFINISHED RUNNING in $ms_text ms : day$1.cc\033[0m";

    rm time.tmp
    rm user_ms.tmp

    rm __program__
    read -n 1 -s -r -p "";
    clear;
fi