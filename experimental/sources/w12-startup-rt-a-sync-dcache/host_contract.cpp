// Host contract for SyncDCache. Not a build-plan input.
//   c++ -std=c++17 -DNORRATH_HOST_CONTRACT -o check host_contract.cpp && ./check
// The Status word and the three callees are scripted; the check records call order and arguments.
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" unsigned norrath_host_status_word;
unsigned norrath_host_status_word;
static char order[16]; static int n; static unsigned seen[2];
extern "C" int DIntr() { order[n++] = 'D'; return 0; }
extern "C" int EIntr() { order[n++] = 'E'; return 0; }
extern "C" void _sceSDC(unsigned b, unsigned e) { order[n++] = 'S'; seen[0] = b; seen[1] = e; }
#include "sync_dcache.cpp"
int main() {
    int bad = 0;
    struct C { unsigned sr; const char* want; } cases[] = {{0, "S"}, {0x10000, "DSE"}, {0xfffeffff, "S"}, {0x30001, "DSE"}};
    for (auto& c : cases) {
        n = 0; std::memset(order, 0, sizeof order); norrath_host_status_word = c.sr;
        SyncDCache(reinterpret_cast<void*>(0x1a0007fu), reinterpret_cast<void*>(0x1a000c1u));
        if (std::strcmp(order, c.want) != 0) { std::printf("order %x %s\n", c.sr, order); ++bad; }
        if (seen[0] != 0x1a00040u || seen[1] != 0x1a000c0u) { std::printf("mask %x %x\n", seen[0], seen[1]); ++bad; }
    }
    return bad;
}
