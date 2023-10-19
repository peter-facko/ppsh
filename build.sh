#!/usr/bin/env sh

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source-dir> <build-dir>"
    exit 1
fi

cmake -G "Ninja Multi-Config" -S "$1" -B "$2" && \
cmake --build "$2" --config Release
