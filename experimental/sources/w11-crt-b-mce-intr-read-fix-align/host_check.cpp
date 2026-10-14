// Host comparison only. Not an EE build input. The uncached-bit OR and the 32-bit destination
// words are replaced by the host-contract mapping.
#include <cstdio>
#include <cstring>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
extern "C" {
int semaid_timerfunc;
}
static std::vector<int> events;
static unsigned char buffers[2][256];
extern "C" {
int iSignalSema(int s) { events.push_back(s); return 4000 + s; }
unsigned char* norrath_host_destination(unsigned word) { return buffers[word - 1]; }
}
#include "mce_intr_read_fix_align.cpp"
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
alignas(8) static unsigned char block[160];
static void put(unsigned off, int v) { std::memcpy(block + off, &v, 4); }
static void reset(int a, int b) {
    std::memset(block, 0, sizeof block);
    std::memset(buffers, 0xee, sizeof buffers);
    put(0, a); put(4, b); put(8, 1); put(12, 2);
    for (int i = 0; i < 64; ++i) { block[16 + i] = static_cast<unsigned char>(0x10 + i); block[80 + i] = static_cast<unsigned char>(0x90 + i); }
    events.clear(); semaid_timerfunc = 21;
}
int main() {
    reset(5, 3);
    check(mceIntrReadFixAlign(block) == 4021 && events == std::vector<int>{21}, "result and semaphore");
    bool ok = true;
    for (int i = 0; i < 5; ++i) ok = ok && buffers[0][i] == 0x10 + i;
    for (int i = 0; i < 3; ++i) ok = ok && buffers[1][i] == 0x90 + i;
    ok = ok && buffers[0][5] == 0xee && buffers[1][3] == 0xee;
    check(ok, "both copies and no overrun");
    reset(0, 4);
    mceIntrReadFixAlign(block);
    check(buffers[0][0] == 0xee && buffers[1][3] == 0x93 && buffers[1][4] == 0xee, "A zero copies only B");
    reset(4, 0);
    mceIntrReadFixAlign(block);
    check(buffers[0][3] == 0x13 && buffers[0][4] == 0xee && buffers[1][0] == 0xee, "B zero copies only A");
    reset(-3, -7);
    mceIntrReadFixAlign(block);
    check(buffers[0][0] == 0xee && buffers[1][0] == 0xee && events.size() == 1, "negative counts copy nothing");
    reset(64, 64);
    mceIntrReadFixAlign(block);
    check(buffers[0][63] == 0x10 + 63 && buffers[1][63] == 0x90 + 63, "full blocks");
    if (failures) return 2;
    std::puts("mceIntrReadFixAlign cases passed");
    return 0;
}
