#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/JSON

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -pthread -o test.bin *.cpp
if [ "$1" == "-run" ]
then
    ./test.bin
fi
