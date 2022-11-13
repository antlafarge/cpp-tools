#!/bin/bash

# sudo apt install g++-10-powerpc64-linux-gnu qemu-user
# cd /mnt/c/Users/[username]/Documents/cpp-tools/Base64

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    powerpc64-linux-gnu-g++-10 -Werror -Wall -Wextra -Wno-unknown-pragmas -g -Isrc src/*.cpp test/test.cpp -o test-ppc64.bin
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu
    qemu-ppc64 ./test-ppc64.bin
fi
