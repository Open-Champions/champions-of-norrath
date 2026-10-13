// Host comparison only. Not an EE build input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include "gsm_option.cpp"

int main() {
    int failures = 0;
    alignas(16) unsigned char obj[2576];
    std::memset(obj, 0x5a, sizeof obj);
    unsigned char copy[2576];
    std::memcpy(copy, obj, sizeof obj);
    int v = 9;
    for (int opt : {0, 1, 2, 4, 5, 6, -1, 0x103}) {
        if (gsm_option(obj, opt, &v) != -1 || std::memcmp(obj, copy, sizeof obj) != 0) { std::printf("option %d must be inert\n", opt); ++failures; }
    }
    long long f = 7; std::memcpy(obj + 2464, &f, 8);
    std::memcpy(copy, obj, sizeof obj);
    if (gsm_option(obj, 3, nullptr) != 7 || std::memcmp(obj, copy, sizeof obj) != 0) { std::printf("null pointer must read only\n"); ++failures; }
    v = -2;
    if (gsm_option(obj, 3, &v) != 7) { std::printf("old value expected\n"); ++failures; }
    std::memcpy(&f, obj + 2464, 8);
    if (f != -2) { std::printf("must sign-extend into 64 bits: %lld\n", f); ++failures; }
    std::memset(copy, 0, 0);
    if (obj[2472] != 0x5a || obj[2463] != 0x5a) { std::printf("neighbour clobbered\n"); ++failures; }
    if (gsm_option(obj, 3, &v) != -2) { std::printf("second read wrong\n"); ++failures; }
    if (failures) return 2;
    std::printf("gsm_option cases passed\n");
    return 0;
}
