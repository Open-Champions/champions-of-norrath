#include "act_align.cpp"

#include <cstdio>

int main() {
    const unsigned char want[6] = {0x00, 0x01, 0xff, 0xff, 0xff, 0xff};
    if (sizeof actAlignData != 6 || sizeof actSendData != 8) return 1;
    for (int i = 0; i < 6; ++i) if (actAlignData[i] != want[i]) return 2;
    for (unsigned char b : actSendData) if (b != 0) return 3;
    std::puts("act align data passed");
    return 0;
}
