#!/bin/bash

# sudo apt install g++-10-powerpc64-linux-gnu qemu-user
# cd /mnt/c/Users/[username]/Documents/cpp-tools/JSON

powerpc64-linux-gnu-g++-10 src/*.cpp test.cpp -o test-ppc64.bin
export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu

if [ $? -eq 0 ]
then
    qemu-ppc64 ./test-ppc64.bin
fi
