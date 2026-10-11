/* Observed WaitDma at 0x002d2b50, 112 bytes. If bit 8 of the first channel
 * word is clear, it returns. Otherwise it counts down from 0x01000000. Each
 * step reloads that word. Once the counter goes negative it prints
 * "libdma: sync timeout\n" and calls sceDmaPause, then keeps polling until
 * bit 8 clears. The pause and the print can repeat.
 */
extern "C" int scePrintf(const char* text, ...);
extern "C" void sceDmaPause(unsigned* channel);

static constexpr int norrath_wait_dma_spins = 0x01000000;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" int norrath_host_dma_spin_override_valid;
extern "C" int norrath_host_dma_spin_override;
extern "C" unsigned norrath_host_dma_chcr(unsigned* channel);
#define NORRATH_CHCR(channel) norrath_host_dma_chcr(channel)
static int norrath_spin_start() {
    if (norrath_host_dma_spin_override_valid != 0)
        return norrath_host_dma_spin_override;
    return norrath_wait_dma_spins;
}
#else
#define NORRATH_CHCR(channel) ((channel)[0])
static int norrath_spin_start() { return norrath_wait_dma_spins; }
#endif

extern "C" void WaitDma(unsigned* channel) {
    if ((NORRATH_CHCR(channel) & 0x100u) == 0u)
        return;
    int spins = norrath_spin_start();
    for (;;) {
        spins -= 1;
        if (spins < 0) {
            scePrintf("libdma: sync timeout\n");
            sceDmaPause(channel);
        }
        if ((NORRATH_CHCR(channel) & 0x100u) == 0u)
            return;
        spins -= 1;
    }
}
