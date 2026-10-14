#define NORRATH_HOST_CONTRACT 1
#include "dma_reset.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned dch[10] = {};
extern "C" unsigned* norrath_host_dma_channel(int index);
extern "C" unsigned norrath_host_d_ctrl = 0;
extern "C" unsigned norrath_host_d_stat = 0;
extern "C" int norrath_host_d_stat_read_valid = 0;
extern "C" unsigned norrath_host_d_stat_read = 0;

static unsigned channels[10][40];
static int put_calls;
static unsigned char put_bytes[20];
static int clear_calls;
static int clear_count;

extern "C" unsigned* norrath_host_dma_channel(int index) { return channels[index]; }
extern "C" void memclr(unsigned char* dest, int count) {
    clear_calls += 1;
    clear_count = count;
    for (int i = 0; i < count; ++i) dest[i] = 0;
}
extern "C" void sceDmaPutEnv(void* env) {
    put_calls += 1;
    std::memcpy(put_bytes, env, 20);
}

static void fill() {
    for (int index = 0; index < 10; ++index)
        for (int word = 0; word < 40; ++word) channels[index][word] = 0x111u;
    put_calls = 0;
    clear_calls = 0;
    std::memset(put_bytes, 0x5a, sizeof put_bytes);
}

static int channel_cleared(int index) {
    return channels[index][32] == 0 && channels[index][0] == 0 && channels[index][12] == 0
        && channels[index][4] == 0 && channels[index][20] == 0 && channels[index][16] == 0;
}

int main() {
    const unsigned expect_isclr[10] = {1u, 1u, 1u, 1u, 1u, 0u, 0u, 0u, 1u, 1u};
    for (int index = 0; index < 10; ++index) {
        if (isclr[index] != expect_isclr[index]) return 1;
    }
    fill();
    norrath_host_d_ctrl = 0x10u;
    norrath_host_d_stat_read_valid = 0;
    if (sceDmaReset(0) != 0) return 2;
    if (norrath_host_d_ctrl != 0x10u || norrath_host_d_stat != 0u) return 3;
    for (int index = 0; index < 10; ++index) {
        if (expect_isclr[index] != 0 && !channel_cleared(index)) return 10 + index;
        if (expect_isclr[index] == 0 && channels[index][0] != 0x111u) return 20 + index;
    }
    if (clear_calls != 1 || clear_count != 20 || put_calls != 1) return 4;
    for (unsigned char byte : put_bytes) {
        if (byte != 0) return 5;
    }

    fill();
    norrath_host_d_ctrl = 0x21u;
    norrath_host_d_stat_read_valid = 1;
    norrath_host_d_stat_read = 0x1234ff1fu;
    if (sceDmaReset(1) != 1) return 6;
    if (norrath_host_d_stat != 0x12140000u) return 7;
    if (norrath_host_d_ctrl != 0x21u) return 8;

    fill();
    norrath_host_d_ctrl = 0u;
    norrath_host_d_stat_read_valid = 0;
    if (sceDmaReset(1) != 0 || norrath_host_d_ctrl != 1u) return 9;
    std::puts("dma reset contract passed");
    return 0;
}
