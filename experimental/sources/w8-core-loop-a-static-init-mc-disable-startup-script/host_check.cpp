// Host check for _GLOBAL_$I$mcDisableStartupScript. Not an EE plan input.
#include "static_init_mc_disable_startup_script.cpp"
#include <cstring>
#include <initializer_list>

extern "C" {
unsigned char tempPlayerStruct[4 * 23984];
}

static unsigned char expect[4 * 23984];

static void put16(unsigned char* p, int v) {
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
}

int main() {
    std::memset(tempPlayerStruct, 0x5a, sizeof tempPlayerStruct);
    std::memcpy(expect, tempPlayerStruct, sizeof expect);
    // independent description of the writes, by absolute offset
    for (int p = 0; p < 4; ++p)
        for (int g = 0; g < 3; ++g)
            for (int e = 0; e < 64; ++e) {
                unsigned char* x = expect + p * 23984 + 328 + g * 4616 + e * 72;
                for (int o : {36, 44, 52, 60}) put16(x + o, 0xffff);
                x[32] = x[33] = 0xff;
                for (int o = 16; o < 32; o += 2) put16(x + o, 0);
            }
    _GLOBAL_$I$mcDisableStartupScript();
    if (std::memcmp(tempPlayerStruct, expect, sizeof expect) != 0) return 1;
    // bytes the constructor does not own remain as they were
    if (tempPlayerStruct[0] != 0x5a || tempPlayerStruct[327] != 0x5a) return 2;
    if (tempPlayerStruct[328 + 34] != 0x5a || tempPlayerStruct[328 + 71] != 0x5a) return 3;
    // running it again with other contents rewrites the same fields
    std::memset(tempPlayerStruct, 0x11, sizeof tempPlayerStruct);
    _GLOBAL_$I$mcDisableStartupScript();
    if (tempPlayerStruct[328 + 16] != 0 || tempPlayerStruct[328 + 32] != 0xff || tempPlayerStruct[328 + 34] != 0x11) return 4;
    // the last entry of the last record is reached, and nothing past the array's group area
    unsigned char* last = tempPlayerStruct + 3 * 23984 + 328 + 2 * 4616 + 63 * 72;
    if (last[32] != 0xff || last[71] != 0x11) return 5;
    return 0;
}
