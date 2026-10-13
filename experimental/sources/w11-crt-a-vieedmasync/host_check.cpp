// Host contract for VIEEDMASync__Fi. Not an EE plan input. The DMA entry points and the
// coprocessor wait are stubs that record what they were given.
#define NORRATH_HOST_CONTRACT
#include "vieedmasync.cpp"
static int waits, getchans, syncs, last_chan_arg, last_sync_mode, last_sync_timeout; static char chan_obj[8];
extern "C" void norrath_host_dma_wait() { ++waits; }
extern "C" void* sceDmaGetChan(int c) { ++getchans; last_chan_arg = c; return chan_obj; }
extern "C" int sceDmaSync(void* c, int m, int t) { ++syncs; last_sync_mode = m; last_sync_timeout = t; return c == chan_obj ? 0 : -1; }
int main() {
    VIEEDMASync__Fi(1);                       // first call: no wait
    if (waits != 0 || getchans || syncs) return 1;
    VIEEDMASync__Fi(1);                       // later calls wait
    if (waits != 1) return 2;
    VIEEDMASync__Fi(1);
    if (waits != 2 || getchans || syncs) return 3;
    VIEEDMASync__Fi(5);                       // other channels go through sceDmaSync
    if (waits != 2 || getchans != 1 || last_chan_arg != 5 || syncs != 1 || last_sync_mode != 0 || last_sync_timeout != 0) return 4;
    VIEEDMASync__Fi(0);
    if (last_chan_arg != 0 || syncs != 2 || waits != 2) return 5;
    return 0;
}
