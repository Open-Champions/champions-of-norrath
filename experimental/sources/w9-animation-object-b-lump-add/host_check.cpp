// Host contract for lumpAdd__FPvPCc. Not an EE plan input.
// Build: c++ -std=c++17 host_check.cpp ../w9-animation-object-b-lumps/lumps.cpp
// The null-lump store (a write to address 0) is not exercised on the host.
#include <cstdio>
#include <cstring>
#include "lump_add.cpp"

int nmLumps;
static void* init_arg;
static int init_calls, init_result, count_seen_by_init;
static int order, init_order;
extern "C" int initLump__FPv(void* lump) { ++init_calls; init_arg = lump; count_seen_by_init = nmLumps; init_order = ++order; return init_result; }
extern "C" int lumpAdd__FPvPCc(void*, const char*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    std::memset(lumps, 0x5a, sizeof lumps);
    int lump_a, lump_b;
    nmLumps = 0; init_result = 1234;
    int r = lumpAdd__FPvPCc(&lump_a, "config.lmp");
    expect(r == 1234 && init_calls == 1 && init_arg == &lump_a, "initializer called with the lump and its result returned");
    expect(count_seen_by_init == 0 && nmLumps == 1, "count is read after the initializer and incremented once");
    expect(std::strcmp(lumps[0].name, "config.lmp") == 0 && lumps[0].lump == &lump_a, "name and lump stored in entry 0");
    expect(lumps[0].word20 == 0 && lumps[0].word24 == 0 && lumps[0].word28 == 0 && lumps[0].word32 == 0 && lumps[0].word36 == 0, "the five words are zeroed");
    expect(lumps[1].name[0] == 0x5a && lumps[1].word36 == 0x5a5a5a5a, "the next entry is untouched");
    nmLumps = 7; init_result = -1;
    r = lumpAdd__FPvPCc(&lump_b, "frontend.lmp");
    expect(r == -1 && nmLumps == 8 && std::strcmp(lumps[7].name, "frontend.lmp") == 0 && lumps[7].lump == &lump_b, "entry index follows the live count");
    expect(lumps[6].lump != &lump_b && lumps[8].name[0] == 0x5a, "neighbours untouched");
    if (failures) return 1;
    std::printf("lumpAdd contract passed\n");
    return 0;
}
