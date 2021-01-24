#!/bin/bash
rm -rf build
mkdir build
cd build
#cmake -DBUILD_TESTS=ON .. #-G Ninja ..
cmake -DBUILD_TESTS=ON -G Ninja ..
#cmake --build . -j16
ninja

