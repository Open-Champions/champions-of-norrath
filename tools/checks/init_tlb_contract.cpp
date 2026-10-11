#include <cstdio>
static int setups, copies, entries;
static unsigned ids[8];
extern "C" unsigned kernel_tlb_sysentry[20] = {};
extern "C" unsigned char tlbsrc[816] = {};
extern "C" unsigned _kExecArg = 0;
extern "C" void kernel_tlb_setup(unsigned, unsigned) { ++setups; }
extern "C" void kernel_tlb_copy(unsigned dest, const void* source, unsigned bytes) {
    if (dest != 0x80075000u || source != tlbsrc || bytes != 816) __builtin_trap();
    ++copies;
}
extern "C" unsigned kernel_tlb_get_entry(unsigned id) { ids[entries++] = id; return 70 + id; }
extern "C" int FlushCache(int) { return 0; }
extern "C" void InitTLBFunctions();
int main() {
    for (int i = 0; i < 20; ++i) kernel_tlb_sysentry[i] = 10 + i;
    InitTLBFunctions();
    if (copies != 1 || setups != 3 + 5 || entries != 6) return 1;
    if (ids[0] != 16 || ids[4] != 24 || ids[5] != 3) return 2;
    if (_kExecArg != 73) return 3;
    std::puts("tlb patch orchestration passed");
}
