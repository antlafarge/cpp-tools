#!/bin/bash

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    powerpc64-linux-gnu-g++-10 \
        -Werror -Wall -Wextra -Wno-unknown-pragmas \
        -g \
        -Isrc -I../Base64/src \
        -o test-ppc64.bin \
        ../Base64/src/*.cpp src/*.cpp test/test.cpp
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu
    qemu-ppc64 ./test-ppc64.bin
fi
