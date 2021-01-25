#include <error.h>
#include <image.h>


static auto test_ptr_constructor() -> void {
    image<RGB> im("images/red_box_no_alpha.png");
    image<RGB> im2(im.data, im.width, im.height);

    ASSERT_EQ(im2.width, im.width);
    ASSERT_EQ(im2.height, im.height);

    for (int i = 0; i < im.height; i++) {
        for (int k = 0; k < im.width; k++) {
            ASSERT_EQ(im[i][k], im2[i][k]);
        }
    }
}

auto main() -> int {
    test_ptr_constructor();
}
