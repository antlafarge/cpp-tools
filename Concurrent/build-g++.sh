#!/bin/bash

if [[ "$1" == "build" || $# -eq 0 ]]; then
    echo "========= Build ========="
    g++ \
        -Werror -Wall -Wextra -Wno-unknown-pragmas \
        -g \
        -pthread \
        -Isrc \
        -o concurrent.bin \
        test/test.cpp
fi

if [[ ($? -eq 0) && ("$1" == "run" || $# -eq 0) ]]; then
    echo "========= Run ==========="
    ./concurrent.bin
fi
