#include <cassert>
#include <iostream>

#include <error.h>
#include <image_diff.h>

static auto test_hello() -> void {
    ASSERT_EQ(true, true);
}

auto main() -> int {
    test_hello();
}
