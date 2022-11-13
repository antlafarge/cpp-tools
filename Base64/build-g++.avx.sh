#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/Base64

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -g -march=native -Isrc/avx -Isrc src/avx/*.cpp src/Base64.helpers.cpp test.cpp -o test.avx.bin

if [ $? -eq 0 ]
then
    ./test.avx.bin
fi
