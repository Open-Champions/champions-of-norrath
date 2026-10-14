// Host contract for canGrab__11SpiderQueen; not an EE input. Pointer words are offsets from a host base.
#define W11_ACTOR_C_HOST 1
#include "can_grab.cpp"
#include <cstring>
#include <initializer_list>
unsigned char* w11_actor_c_host_base;
int main() {
    static unsigned char base[4096];
    w11_actor_c_host_base = base;
    int failures = 0;
    for (unsigned state : {0u, 15u, 16u, 17u, 18u, 0xfffffff0u, 13u})
        for (unsigned target : {0u, 1024u})
            for (unsigned flags : {0u, 0x20u, 0x1fu, 0xffffffffu})
                for (float f : {0.0f, -0.0f, 1.0f, -3.5f}) {
                    std::memset(base, 0x11, sizeof base);
                    unsigned char* queen = base + 64;
                    unsigned char* block = base + 1024;
                    unsigned char* q = base + 2048;
                    *reinterpret_cast<unsigned*>(queen + 964) = state;
                    *reinterpret_cast<unsigned*>(queen + 576) = target;
                    *reinterpret_cast<unsigned*>(block + 12) = 2048;
                    *reinterpret_cast<unsigned*>(q + 12) = flags;
                    std::memcpy(q + 756, &f, 4);
                    unsigned char ref[4096];
                    std::memcpy(ref, base, sizeof ref);
                    int want = (state - 16u >= 2u) && target != 0 && (flags & 0x20u) != 0 && f == 0.0f;
                    if (canGrab__11SpiderQueen(queen) != want) ++failures;
                    if (std::memcmp(ref, base, sizeof ref) != 0) ++failures;
                }
    return failures != 0;
}
