// sprite_min.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <cassert>
#include <cstddef>
#include <iostream>
#include <chrono>

#include <stb_image.h>

#include <image_diff.h>

using namespace std::chrono;

int main() {
    auto old = steady_clock::now();

    auto now = steady_clock::now();

    std::cout << "Hello world" << std::endl;
}
