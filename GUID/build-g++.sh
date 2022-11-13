#!/bin/bash

# sudo apt install g++
# cd /mnt/c/Users/[username]/Documents/cpp-tools/GUID

g++ -Werror -Wall -Wextra -Wno-unknown-pragmas -g -I../Base64/src ../Base64/src/*.cpp *.cpp -o test.bin

if [ $? -eq 0 ]
then
    ./test.bin
fi
