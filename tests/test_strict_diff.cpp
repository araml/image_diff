#include <cassert>
#include <iostream>

#include <error.h>
#include <image_diff.h>

static auto test_strict_diff_RGB() -> void {
    image<RGB> im1("images/red_box_no_alpha.png");
    image<RGB> im2("images/red_box_no_alpha.png");
    image<RGB> output(im1.size());

    strict_diff(im1, im2, output);


    for (auto &p : output) {
        ASSERT_EQ(p.r, p.g);
        ASSERT_EQ(p.g, p.b);
        ASSERT_EQ(p.b, 0);
    }
    ASSERT_EQ(true, true);

    //output.save("build/test_strict_diff.png");
}

static auto test_strict_diff_RGBA() -> void {
    image<RGBA> im1("images/red_box_with_alpha.png");
    image<RGBA> im2("images/red_box_with_alpha.png");
    image<RGBA> output(im1.size());

    strict_diff(im1, im2, output);


    for (auto &p : output) {
        ASSERT_EQ(p.r, p.g);
        ASSERT_EQ(p.g, p.b);
        ASSERT_EQ(p.b, 0);
    }

    ASSERT_EQ(true, true);
}

static auto test_slow_vs_fast_strict_diff() -> void {
    image<RGBA> im1("images/red_box_with_alpha.png");
    image<RGBA> im2("images/red_box_with_alpha.png");
    image<RGBA> out1(im1.size());
    image<RGBA> out2(im1.size());

    fast_diff(im1, im2, out1);
    slow_diff(im1, im2, out2);

    ASSERT_EQ(out1 == out2, true);
}


auto main() -> int {
    test_strict_diff_RGB();
    test_strict_diff_RGBA();
    test_slow_vs_fast_strict_diff();
}
