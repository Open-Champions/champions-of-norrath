#include "get_gparam.cpp"

#include <cstdio>
#include <cstring>

int main() {
    const unsigned char expected[16] = {
        1, 0, 2, 0, 1, 0, 3, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    unsigned char* param = sceGsGetGParam();
    if (param != gp_15) return 1;
    if (std::memcmp(param, expected, 16) != 0) return 2;
    std::puts("gs graph param contract passed");
    return 0;
}
