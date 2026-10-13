// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "allocate.cpp"
#include <cstring>

static unsigned char block[40 * 8];
static unsigned requested, new_calls;
static bool fail;
extern "C" void* __builtin_vec_new(unsigned size) { requested = size; ++new_calls; return fail ? nullptr : block; }

int main() {
    alignas(8) unsigned char list[32];
    const unsigned char z[36] = {0};
    // Zero count: nothing allocated, nothing stored.
    std::memset(list, 0x66, sizeof list);
    unsigned zero = 0;
    std::memcpy(list + 4, &zero, 4);
    new_calls = 0; fail = false;
    unsigned char before[32];
    std::memcpy(before, list, sizeof list);
    if (Allocate__14VIQueryTriList(list) != 0 || new_calls != 0 || std::memcmp(list, before, sizeof list) != 0) return 1;
    // Three records: 120 bytes requested, nine floats zeroed in each, word 0 of each record untouched.
    std::memset(block, 0xa5, sizeof block);
    unsigned three = 3;
    std::memcpy(list + 4, &three, 4);
    if (Allocate__14VIQueryTriList(list) != 0 || requested != 120 || new_calls != 1) return 2;
    for (int r = 0; r < 3; ++r) {
        if (block[40 * r] != 0xa5 || block[40 * r + 1] != 0xa5 || block[40 * r + 2] != 0xa5 || block[40 * r + 3] != 0xa5) return 3;
        if (std::memcmp(block + 40 * r + 4, z, 36) != 0) return 4;
    }
    for (int i = 120; i < 320; ++i) if (block[i] != 0xa5) return 5;
    // The block pointer is stored as a 32-bit word at +8 (the low 32 bits of the host address here).
    unsigned stored;
    std::memcpy(&stored, list + 8, 4);
    if (stored != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block))) return 6;
    for (int i = 12; i < 32; ++i) if (list[i] != 0x66) return 9;
    // Allocation failure: -1 and a null block is stored (the stored word is zero).
    fail = true; std::memset(list + 8, 0x66, 8);
    if (Allocate__14VIQueryTriList(list) != -1) return 7;
    for (int i = 8; i < 12; ++i) if (list[i] != 0) return 8;
    return 0;
}
