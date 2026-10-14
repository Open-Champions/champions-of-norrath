#define NORRATH_HOST_CONTRACT 1
#include "wait_dma.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" int norrath_host_dma_spin_override_valid = 0;
extern "C" int norrath_host_dma_spin_override = 0;

static unsigned script[8];
static int script_at;
static int prints;
static int pauses;
static unsigned* paused_channel;

extern "C" unsigned norrath_host_dma_chcr(unsigned* channel) {
    (void)channel;
    unsigned word = script[script_at];
    if (script_at < 7) script_at += 1;
    return word;
}
extern "C" int scePrintf(const char* text, ...) {
    if (std::strcmp(text, "libdma: sync timeout\n") != 0) return 9;
    prints += 1;
    return 0;
}
extern "C" void sceDmaPause(unsigned* channel) {
    pauses += 1;
    paused_channel = channel;
}

static void load(std::initializer_list<unsigned> words) {
    std::memset(script, 0, sizeof script);
    int index = 0;
    for (unsigned word : words) script[index++] = word;
    script_at = 0;
    prints = 0;
    pauses = 0;
    paused_channel = 0;
}

int main() {
    if (norrath_wait_dma_spins != 0x01000000) return 1;
    unsigned channel = 0;
    norrath_host_dma_spin_override_valid = 1;
    norrath_host_dma_spin_override = 4;
    load({0});
    WaitDma(&channel);
    if (prints != 0 || pauses != 0 || script_at != 1) return 2;

    load({0x100u, 0x100u, 0});
    WaitDma(&channel);
    if (prints != 0 || pauses != 0 || script_at != 3) return 3;

    norrath_host_dma_spin_override = 2;
    load({0x100u, 0x100u, 0});
    WaitDma(&channel);
    if (prints != 1 || pauses != 1 || paused_channel != &channel) return 4;
    if (script_at != 3) return 5;

    norrath_host_dma_spin_override = 0;
    load({0x100u, 0});
    WaitDma(&channel);
    if (prints != 1 || pauses != 1 || script_at != 2) return 6;
    std::puts("wait dma contract passed");
    return 0;
}
