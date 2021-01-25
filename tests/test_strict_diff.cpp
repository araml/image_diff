#include <cassert>
#include <iostream>

#include <error.h>
#include <image_diff.h>

static auto test_strict_diff() -> void {
    image<RGB> im1("images/red_box_no_alpha.png");
    image<RGB> im2("images/red_box_no_alpha.png");
    image<RGB> output(im1.size());

    strict_diff(im1, im2, output);


    for (auto &p : output) {
        ASSERT_EQ(p.r, p.g);
        ASSERT_EQ(p.g, p.b);
        ASSERT_EQ(p.b, 0);
    }

    output.save("build/test_strict_diff.png");

    ASSERT_EQ(true, true);
}

auto main() -> int {
    test_strict_diff();
}
