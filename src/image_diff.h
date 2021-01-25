#pragma once

#include <stddef.h>

#include <image.h>

using uchar = unsigned char;

constexpr float max_YIQ_delta = 35215.;

template <typename Color>
auto inf_norm(const Color &p1, const Color &p2) -> uchar {
    uchar r = std::abs(p1.r - p2.r);
    uchar g = std::abs(p1.g - p2.g);
    uchar b = std::abs(p1.b - p2.b);

    if (r <= g)
        return g > b ? g : b;
    else
        return r > b ? r : b;
}

template <typename T>
auto fast_diff(image<T> &base, image<T> &compare, image<T> output) -> void {
    size_t length = base.width * base.height;
    T* base_data = base.data;
    T* comp_data = compare.data;
    T* out_data = output.data;

    for (size_t i = 0; i < length; i++) {
        uchar v = inf_norm(base_data[i], comp_data[i]);
        if constexpr (pixel_size_v<T> == 4) { // RGBA
            out_data[i] = {v, v, v, 255};
        } else {
            out_data[i] = {v, v, v};
        }
    }
}

template <typename T>
auto slow_diff(image<T> &base, image<T> &compare, image<T> &output) -> void {
    for (size_t i = 0; i < base.height; i++) {
        for (size_t k = 0; k < base.width; k++) {
            if (i >= compare.height || k >= compare.width) {
                if constexpr (pixel_size_v<T> == 3) {
                    output[i][k] = RGB{255, 0, 0};
                } else {
                    output[i][k] = RGBA{255, 0, 0, 255};
                }
            } else {
                uchar v = inf_norm(base[i][k], compare[i][k]);
                if constexpr (pixel_size_v<T> == 3) {
                    output[i][k] = RGB{v, v, v};
                } else {
                    output[i][k] = RGBA{v, v, v, 255};
                }
            }
        }
    }
}

template <typename T>
auto strict_diff(image<T> &base, image<T> &compare, image<T> &output) {
    if (base.size() == compare.size()) {
        fast_diff(base, compare, output);
    }
}

/*
auto generate_diff(image &base, image &compare,
                   image &output, float threshold) -> bool {
    size_t total_size = width * height;

    float max_delta = max_YIQ_delta * threshold * threshold;

    bool is_there_diff = false;

    for (size_t i = 0; i < total_size; i++) {
        if (output[i] = base[i] - dst[i])
            is_there_diff = true;
    }

    return true;

}*/
