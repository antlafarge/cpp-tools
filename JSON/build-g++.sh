#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/JSON

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas src/*.cpp test.cpp -o test.bin

if [ $? -eq 0 ]
then
    ./test.bin
fi
