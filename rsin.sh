#!/bin/bash
# Run single c-file
cd day$1

if (( $1 == 10 )); then
    cd ../build
    make 1> /dev/null
    if [ $? -eq 0 ]; then
        ./day10

        rm day10
        read -n 1 -s -r -p "";
        clear;
    fi
else
    g++ -O3 -std=c++23 -Wall day$1.cc -o __program__;
    if [ $? -eq 0 ]; then
        ./__program__

        rm __program__
        read -n 1 -s -r -p "";
        clear;
    fi
fi
