#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/Base64

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -g -Isrc src/*.cpp test.cpp -o test.bin

if [ $? -eq 0 ]
then
    ./test.bin
fi
