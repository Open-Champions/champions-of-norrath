#include "dma_sync.cpp"

#include <cstdio>

static int waits;
static unsigned* waited;

extern "C" void WaitDma(unsigned* channel) {
    waits += 1;
    waited = channel;
}

int main() {
    unsigned channel[1] = {0x100u};
    waits = 0;
    if (sceDmaSync(channel, 1, 99) != 1 || waits != 0) return 1;
    channel[0] = 0;
    if (sceDmaSync(channel, 1, 99) != 0 || waits != 0) return 2;
    channel[0] = 0x200u;
    if (sceDmaSync(channel, 1, -1) != 0) return 3;
    channel[0] = 0x300u;
    if (sceDmaSync(channel, 1, 0) != 1) return 4;
    if (sceDmaSync(channel, 0, 5) != 0 || waits != 1 || waited != channel) return 5;
    if (sceDmaSync(channel, 2, 5) != 0 || waits != 2) return 6;
    if (sceDmaSync(channel, -4, 5) != 0 || waits != 3) return 7;
    std::puts("dma sync contract passed");
    return 0;
}
