#!/bin/bash

# Usage : ./build-g++-ppc64.sh
# Build only : ./build-g++-ppc64.sh build
# Run only : ./build-g++-ppc64.sh run

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    powerpc64-linux-gnu-g++ \
        -Werror -Wall -Wextra -Wno-unknown-pragmas \
        -g \
        -Isrc \
        -o base64-ppc64.bin \
        src/*.cpp test/test.cpp
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    export QEMU_LD_PREFIX=/usr/powerpc64-linux-gnu
    qemu-ppc64 ./base64-ppc64.bin
fi
