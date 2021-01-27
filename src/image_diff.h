#pragma once

#include <stddef.h>

#include <error.h>
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
auto fast_diff(image<T> &base, image<T> &compare, image<T> &output) -> void {
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
    for (int i = 0; i < base.height; i++) {
        for (int k = 0; k < base.width; k++) {
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

template<typename T>
constexpr auto red_pixel() -> T {
    if constexpr (pixel_size_v<T> == 3) {
        return T{255, 0, 0};
    } else {
        return T{255, 0, 0, 255};
    }
}

template <typename T>
auto YIQ_diff(image<T> &base, image<T> &compare,
                   image<T> &output, float threshold = 0.1) -> bool {

    float max_delta = max_YIQ_delta * threshold * threshold;

    size_t differences = 0;
    for (int i = 0; i < base.height; i ++) {
        for (int k = 0; k < base.width; k++) {
            if (base[i][k] == compare[i][k])
                continue;

            YIQ b_yiq = base[i][k];
            YIQ c_yiq = compare[i][k];

            YIQ result = b_yiq - c_yiq;
            float delta = 0.5053 * result.y * result.y +
                          0.299  * result.i * result.i +
                          0.1957 * result.q * result.q;
            if (delta > max_delta) {
                output[i][k] = red_pixel<T>();
                differences++;
            }
        }
    }

    LOG("Different pixels:", differences);

    return differences > 0;

}
