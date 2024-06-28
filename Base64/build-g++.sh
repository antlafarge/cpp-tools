#!/bin/bash

# Usage : ./build-g++.sh
# Build only : ./build-g++.sh build
# Run only : ./build-g++.sh run

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    g++ \
        -Werror -Wall -Wextra -Wno-unknown-pragmas \
        -g \
        -Isrc \
        -o base64.bin \
        src/*.cpp test/test.cpp
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    ./base64.bin
fi
