// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_read.cpp
#include <cstring>
extern "C" {
unsigned char norrath_libpad_port_info_view[256];
static unsigned char dma_block[2][2][128];
static int dintr_result, dintr_calls, eintr_calls, block_calls, last_port = -1, last_slot = -1;
static int dintr_before_block;
int DIntr() { ++dintr_calls; return dintr_result; }
int EIntr() { ++eintr_calls; return 0; }
unsigned char* scePadGetDmaStr(int port, int slot) {
    ++block_calls; last_port = port; last_slot = slot;
    dintr_before_block = dintr_calls;
    return dma_block[port][slot];
}
int scePadRead(int port, int slot, void* buffer);
}
static void set_open(int port, int slot, unsigned value) {
    unsigned* info = reinterpret_cast<unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    info[4] = value;
}
static void reset() { dintr_calls = eintr_calls = block_calls = 0; dintr_result = 1; last_port = last_slot = -1; }
int main() {
    unsigned char out[160];
    for (int p = 0; p < 2; ++p) for (int s = 0; s < 2; ++s)
        for (int i = 0; i < 128; ++i) dma_block[p][s][i] = static_cast<unsigned char>(p * 64 + s * 32 + i + 1);
    // not open: nothing is touched
    std::memset(out, 0xee, sizeof out);
    reset();
    if (scePadRead(0, 0, out) != 0 || dintr_calls != 0 || block_calls != 0 || out[0] != 0xee) return 1;
    // each port/slot uses its own 28-byte/112-byte info entry
    set_open(1, 1, 1);
    reset();
    if (scePadRead(0, 1, out) != 0 || scePadRead(1, 0, out) != 0 || block_calls != 0) return 2;
    // the count is the word at +96 of the 128-byte block
    unsigned count = 32;
    std::memcpy(dma_block[1][1] + 96, &count, 4);
    reset();
    std::memset(out, 0xee, sizeof out);
    if (scePadRead(1, 1, out) != 32 || last_port != 1 || last_slot != 1) return 3;
    if (std::memcmp(out, dma_block[1][1], 32) != 0 || out[32] != 0xee) return 4;
    if (dintr_calls != 1 || dintr_before_block != 1 || eintr_calls != 1) return 5;
    // interrupts are re-enabled only when DIntr returned exactly 1
    reset(); dintr_result = 0;
    scePadRead(1, 1, out);
    if (eintr_calls != 0 || dintr_calls != 1) return 6;
    reset(); dintr_result = 2;
    scePadRead(1, 1, out);
    if (eintr_calls != 0) return 7;
    // a count of 100 copies 100 bytes including the count word itself
    count = 100;
    std::memcpy(dma_block[1][1] + 96, &count, 4);
    reset();
    std::memset(out, 0xee, sizeof out);
    if (scePadRead(1, 1, out) != 100 || std::memcmp(out, dma_block[1][1], 100) != 0 || out[100] != 0xee) return 8;
    // zero count copies nothing but still counts as open and returns 0
    count = 0;
    std::memcpy(dma_block[1][1] + 96, &count, 4);
    std::memset(out, 0xee, sizeof out);
    reset();
    if (scePadRead(1, 1, out) != 0 || out[0] != 0xee || block_calls != 1 || eintr_calls != 1) return 9;
    return 0;
}
