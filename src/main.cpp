// sprite_min.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <cassert>
#include <cstddef>
#include <iostream>
#include <chrono>
#include <unordered_map>

#include <image_diff.h>

constexpr char help[] = "Usage: (TBD) [OPTION]... "
                        "base_image compare_image [output]\n\n"
                        "The options are as follow: \n"
                        "-j, --jobs       numbers of threads that will run for "
                        "the current comparator\n"
                        "-s, --strict     selects a faster comparator which "
                        "will show non perceived differences\n"
                        "-t, --threshold  selects the image threshold for the "
                        "perceived diff comparator\n"
                        "    --help       displays this help and exit\n"
                        "    --version    outputs version information and exit\n";


auto main(int argc, char *argv[]) -> int {
    if (argc < 4) {
        std::cout << help;
        return 0;
    }
    image_info base_info = query_image_info(argv[2]);
    image_info compare_info = query_image_info(argv[3]);
    if (argc == 5 && (std::string(argv[1]) == "-s" ||
        std::string(argv[1]) == "--strict")) {
        if (base_info.channels == 3 && compare_info.channels == 3) {
            image<RGB> base(argv[2]);
            image<RGB> compare(argv[3]);
            image<RGB> output(base.size());
            strict_diff(base, compare, output);
            output.save(argv[4]);
        } else {
            image<RGBA> base(argv[2]);
            image<RGBA> compare(argv[3]);
            image<RGBA> output(base.size());
            strict_diff(base, compare, output);
            output.save(argv[4]);
        }
    } else if (argc == 4) {
        if (base_info.channels == 3 && compare_info.channels == 3) {
            image<RGB> base(argv[1]);
            image<RGB> compare(argv[2]);
            image<RGB> output(base.size());
            YIQ_diff(base, compare, output);
            output.save(argv[3]);
        } else {
            image<RGBA> base(argv[1]);
            image<RGBA> compare(argv[2]);
            image<RGBA> output(base.size());
            YIQ_diff(base, compare, output);
            output.save(argv[3]);
        }

    }

    return 0;
}
