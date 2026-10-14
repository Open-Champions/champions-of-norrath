#include "dma_get_chan.cpp"

#include <cstdint>
#include <cstdio>

int main() {
    const unsigned expect[10] = {
        0x10008000u, 0x10009000u, 0x1000a000u, 0x1000b000u, 0x1000b400u,
        0x1000c000u, 0x1000c400u, 0x1000c800u, 0x1000d000u, 0x1000d400u
    };
    for (int channel = 0; channel < 10; ++channel) {
        unsigned* got = sceDmaGetChan(channel);
        if (reinterpret_cast<std::uintptr_t>(got) != expect[channel]) return channel + 1;
    }
    if (sceDmaGetChan(10) != 0 || sceDmaGetChan(11) != 0 || sceDmaGetChan(-1) != 0) return 20;
    dch[2] = 0x01020304u;
    if (reinterpret_cast<std::uintptr_t>(sceDmaGetChan(2)) != 0x01020304u) return 21;
    std::puts("dma get channel contract passed");
    return 0;
}
