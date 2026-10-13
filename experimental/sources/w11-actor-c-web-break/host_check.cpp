// Host contract for breakWeb__3Web; not an EE input. Pointer words are offsets from a host base.
#define W11_ACTOR_C_HOST 1
#include "break_web.cpp"
#include <cstring>
#include <initializer_list>
unsigned char* w11_actor_c_host_base;
int main() {
    static unsigned char base[4096];
    w11_actor_c_host_base = base;
    int failures = 0;
    for (unsigned w12 : {0u, 5u})
        for (unsigned w28 : {0u, 512u})
            for (unsigned flags : {0u, 0x20u, 0x1fu, 0xffffffdfu})
                for (unsigned w44 : {0u, 1u, 9u}) {
                    std::memset(base, 0xa5, sizeof base);
                    unsigned char* web = base + 64;
                    unsigned char* held = base + 512;   // object the web points at
                    unsigned char* p = base + 1024;     // its pointer at +12
                    *reinterpret_cast<unsigned*>(web + 12) = w12;
                    *reinterpret_cast<unsigned*>(web + 28) = w28;
                    *reinterpret_cast<unsigned*>(web + 44) = w44;
                    *reinterpret_cast<unsigned*>(held + 12) = 1024;
                    *reinterpret_cast<unsigned*>(p + 12) = flags;
                    *reinterpret_cast<unsigned*>(p + 756) = 0x11223344u;
                    unsigned char ref[4096];
                    std::memcpy(ref, base, sizeof ref);
                    breakWeb__3Web(web);
                    bool hit = w12 != 0 && w28 != 0 && (flags & 0x20u) != 0;
                    if (hit) *reinterpret_cast<unsigned*>(ref + 1024 + 756) = 0;
                    *reinterpret_cast<unsigned*>(ref + 64 + 44) = 0;
                    if (std::memcmp(ref, base, sizeof ref) != 0) ++failures;
                }
    return failures != 0;
}
