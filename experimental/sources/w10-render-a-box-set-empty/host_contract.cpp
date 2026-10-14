// Host contract for boxSetEmpty__FP3Box. Not an EE plan input.
#include "box_set_empty.cpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char box[32];
    for (unsigned i = 0; i != 32; ++i) box[i] = 0xa5;
    boxSetEmpty__FP3Box(box);
    const unsigned want[8] = {0x4b189680u, 0x4b189680u, 0x4b189680u,
                              0xcb189680u, 0xcb189680u, 0xcb189680u, 0xa5a5a5a5u, 0xa5a5a5a5u};
    for (unsigned i = 0; i != 8; ++i)
        if (hw(box + 4 * i) != want[i]) { std::printf("word %u got %08x\n", i, hw(box + 4 * i)); return 1; }
    std::puts("boxSetEmpty contract passed");
    return 0;
}
