#include "words.cpp"

#include <cstdio>

int main() {
    if (sizeof _sceCd_scmdsdata != 1032 || sizeof eenull != 40 || sizeof batchBuffer != 4096 || sizeof gCd != 96)
        return 1;
    for (unsigned char byte : _sceCd_scmdsdata) if (byte != 0) return 2;
    const unsigned expected[5] = {0x3c1d0008u, 0x0060f809u, 0x27bd1fc0u, 0x2403fff8u, 0x0000000cu};
    for (int i = 0; i < 5; ++i) if (eenull[i] != expected[i]) return 3;
    for (int i = 5; i < 10; ++i) if (eenull[i] != 0) return 4;
    for (unsigned char byte : batchBuffer) if (byte != 0) return 5;
    for (unsigned char byte : gCd) if (byte != 0) return 6;
    if (cancelStreamWait != 0 || decompressThreadid != 0 || dmaGIF != 0) return 7;
    std::puts("startup data words passed");
    return 0;
}
