#!/bin/bash
cmake -DCMAKE_BUILD_TYPE:STRING="Release" -G Ninja -S . -B out
cmake --build out
