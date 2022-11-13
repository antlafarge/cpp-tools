#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/Base64

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -g -Isrc src/*.cpp test/test.cpp -o test.bin
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    ./test.bin
fi
