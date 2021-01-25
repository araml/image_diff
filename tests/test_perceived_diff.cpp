#include <error.h>
#include <image_diff.h>

static auto test_perceived_diff() -> void {
    image<RGBA> im1("images/donkey.png");
    image<RGBA> im2("images/donkey-2.png");
    image<RGBA> output(im1.size());

    YIQ_diff(im1, im2, output, 0.1);

    output.save("donkey_diff.png");
}

auto main() -> int {
    test_perceived_diff();
}
