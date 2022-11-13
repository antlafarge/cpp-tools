#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/JSON

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -o test.bin src/*.cpp *.cpp
./test.bin
