#!/bin/bash

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    g++ \
        -Werror -Wall -Wextra -Wno-unknown-pragmas \
        -g \
        -Isrc -I../Base64/src \
        -o uuid.bin \
        ../Base64/src/*.cpp src/*.cpp test/test.cpp
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    ./uuid.bin
fi
