#!/bin/bash

# sudo apt install g++-10-powerpc64-linux-gnu qemu-user
# cd /mnt/c/Users/[username]/Documents/cpp-tools/JSON

powerpc64-linux-gnu-g++-10 -o test-ppc64.bin src/*.cpp *.cpp
export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu
qemu-ppc64 ./test-ppc64.bin
