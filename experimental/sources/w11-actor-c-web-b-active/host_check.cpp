// Host contract for bActive__3Web; not an EE input. Includes the source.
#include "b_active.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    for (unsigned v : {0u, 1u, 2u, 0x80000000u, 0xffffffffu, 0x100u}) {
        unsigned char web[64];
        std::memset(web, 0xff, sizeof web);
        std::memcpy(web + 44, &v, 4);
        if (bActive__3Web(web) != (v != 0u ? 1 : 0)) return 1;
    }
    unsigned char web[64] = {};
    web[43] = web[48] = 0xff;  // neighbours do not count
    return bActive__3Web(web) != 0;
}
