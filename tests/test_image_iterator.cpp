#include <image.h>
#include <error.h>

static auto test_image_iterator_RGB() -> void {
    image<RGB> im("images/red_box_no_alpha.png");

    for (auto &p : im) {
        ASSERT_EQ(p, (RGB{255, 0, 0}));
    }
}

static auto test_image_iterator_RGBA() -> void {
    image<RGBA> im("images/red_box_with_alpha.png");

    for (auto &p : im) {
        ASSERT_EQ(p, (RGBA{255, 0, 0, 255}));
    }
}

auto main() -> int {
    test_image_iterator_RGB();
    test_image_iterator_RGBA();
}
