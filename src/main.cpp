// sprite_min.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <cassert>
#include <cstddef>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <future>

#include <image_diff.h>

using namespace std::chrono;

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
            //image<RGBA> base(argv[2]);
            //image<RGBA> compare(argv[3]);
            LOG("Async test");
            std::future<image<RGBA>> bf = std::async(
                std::launch::async, [argv] { return image<RGBA>{argv[2]}; });
            std::future<image<RGBA>> cf = std::async(
                std::launch::async, [argv] { return image<RGBA>{argv[3]}; });
            std::future<image<RGBA>> of =
                std::async(std::launch::async, [base_info] {
                    return image<RGBA>{
                        image_size{base_info.width, base_info.height}};
                });

            image<RGBA> base = bf.get();
            image<RGBA> compare = cf.get();
            image<RGBA> output = of.get();
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
            auto old_total = steady_clock::now();
            LOG("Async test");
            std::future<image<RGBA>> bf = std::async(
                std::launch::async, [argv] {
                    auto old = steady_clock::now();
                    image<RGBA> im{argv[1]};
                    auto now = steady_clock::now();
                    LOG("Time to load compare:", duration_cast<nanoseconds>(now - old).count() * 1e-6);
                    return im;
                });
            std::future<image<RGBA>> cf = std::async(
                std::launch::async, [argv] {
                    auto old = steady_clock::now();
                    image<RGBA> im{argv[2]};
                    auto now = steady_clock::now();
                    LOG("Time to load compare:", duration_cast<nanoseconds>(now - old).count() * 1e-6);
                    return im;
                });
            std::future<image<RGBA>> of =
                std::async(std::launch::async, [base_info] {
                    return image<RGBA>{
                        image_size{base_info.width, base_info.height}};
                });

            image<RGBA> base = bf.get();
            image<RGBA> compare = cf.get();
            image<RGBA> output = of.get();

            auto old_diff = steady_clock::now();
            YIQ_diff(base, compare, output);
            auto now_diff = steady_clock::now();
            LOG("Time to diff:",
                duration_cast<nanoseconds>(now_diff - old_diff).count() * 1e-6);

            auto old = steady_clock::now();
            output.save(argv[3]);
            auto now = steady_clock::now();
            LOG("Time to save:", duration_cast<nanoseconds>(now - old).count() * 1e-6);
            LOG("Total time:", duration_cast<nanoseconds>(now - old_total).count() * 1e-6);
        }

    }

    return 0;
}
