#include "sce_dma_send.cpp"

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
    unsigned channel[13];
    for (unsigned& word : channel) word = 0x0cu;
    unsigned char packet[4] = {};
    checks = 0;
    waits = 0;
    sceDmaSend(channel, packet);
    if (checks != 1 || waits != 1 || waited != channel) return 1;
    if (seen_address != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(packet))) return 2;
    if (channel[12] != seen_address || channel[8] != 0u) return 3;
    if (channel[0] != 0x105u) return 4;
    if (channel[4] != 0x0cu) return 5;

    channel[0] = 0xffffffffu;
    channel[12] = 0xffffffffu;
    sceDmaSend(channel, reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x70001234u)));
    if (seen_address != 0x70001234u || channel[12] != 0xffffffffu) return 6;
    if (channel[8] != 0u || channel[0] != 0xfffffff7u) return 7;
    channel[12] = 0u;
    sceDmaSend(channel, reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x70001234u)));
    if (channel[12] != 0x80001234u) return 8;
    std::puts("dma send contract passed");
    return 0;
}
