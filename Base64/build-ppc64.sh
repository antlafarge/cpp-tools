#!/bin/bash

# sudo apt install g++-10-powerpc64-linux-gnu qemu-user
# cd /mnt/c/Users/[username]/Documents/cpp-tools/Base64

powerpc64-linux-gnu-g++-10 -Werror -Wall -Wextra -Wno-unknown-pragmas -g -Isrc src/*.cpp test.cpp -o test-ppc64.bin

if [ $? -eq 0 ]
then
    export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu
    qemu-ppc64 ./test-ppc64.bin
fi
