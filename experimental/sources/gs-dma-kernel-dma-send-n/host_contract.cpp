#include "dma_send_n.cpp"

#include <cstdint>
#include <cstdio>

static int checks;
static int waits;
static unsigned seen_address;
static unsigned* waited;

extern "C" unsigned CheckAddress(unsigned address) {
    checks += 1;
    seen_address = address;
    if ((address >> 28) == 7u)
        return (address & 0x0fffffffu) | 0x80000000u;
    return address;
}
extern "C" void WaitDma(unsigned* channel) {
    waits += 1;
    waited = channel;
}

int main() {
    unsigned channel[9];
    for (unsigned& word : channel) word = 0x0cu;
    unsigned char packet[4] = {};
    checks = 0;
    waits = 0;
    sceDmaSendN(channel, packet, 6);
    if (checks != 1 || waits != 1 || waited != channel) return 1;
    if (seen_address != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(packet))) return 2;
    if (channel[4] != seen_address || channel[8] != 6u) return 3;
    if (channel[0] != 0x101u) return 4;

    channel[0] = 0xffffffffu;
    channel[4] = 0xffffffffu;
    sceDmaSendN(channel, reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x70001234u)), 1);
    if (seen_address != 0x70001234u || channel[4] != 0xffffffffu) return 5;
    if (channel[8] != 1u || channel[0] != 0xfffffff3u) return 6;
    std::puts("dma send normal contract passed");
    return 0;
}
