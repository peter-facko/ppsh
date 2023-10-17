#!/usr/bin/env sh

cmake -G "Ninja Multi-Config" -S ./ -B build/ && \
cmake --build build/ --config Release
