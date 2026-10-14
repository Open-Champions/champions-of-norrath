/* Host contract. Not a plan input. A fake allocator hands out a chunk at a chosen
 * offset of an arena; the checks are the invariants of an aligned allocation. */
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>
#include "memalign_r.cpp"
alignas(64) static unsigned char arena[1 << 16];
static unsigned wanted_offset;          // chosen memory offset of the next fake allocation
static unsigned extra_bytes;
static unsigned last_request;
static std::vector<void*> frees;
static int locks, unlocks;
extern "C" void* _malloc_r(void*, unsigned n) {
    last_request = n;
    if (wanted_offset == 0) return nullptr;
    unsigned char* mem = arena + wanted_offset;
    unsigned chunk = ((n + 19u) & ~15u);
    if (chunk < 16) chunk = 16;
    chunk += extra_bytes;
    *reinterpret_cast<unsigned*>(mem - 4) = chunk | 1u;
    *reinterpret_cast<unsigned*>(mem - 8 + chunk + 4) = 0;
    return mem;
}
extern "C" void _free_r(void*, void* m) { frees.push_back(m); }
extern "C" void __malloc_lock(void*) { ++locks; }
extern "C" void __malloc_unlock(void*) { ++unlocks; }
int main() {
    int bad = 0;
    // Small alignment: straight malloc, no lock calls.
    wanted_offset = 4096;
    if (_memalign_r(nullptr, 16, 100) != arena + 4096 || last_request != 100 || locks != 0) { std::puts("small alignment wrong"); ++bad; }
    // Zero and 8 also go straight through.
    if (_memalign_r(nullptr, 0, 5) != arena + 4096 || last_request != 5) { std::puts("zero alignment wrong"); ++bad; }
    // Failure.
    wanted_offset = 0; locks = 0;
    if (_memalign_r(nullptr, 64, 100) != nullptr || locks != 0) { std::puts("failure wrong"); ++bad; }
    // Aligned results for many alignments, offsets and extra sizes.
    for (unsigned alignment : {32u, 64u, 128u, 256u, 1024u})   // powers of two: the mask arithmetic assumes it
        for (unsigned offset = 4096 + 8; offset < 4096 + 8 + 2 * 1024 + 8 && offset < 20000; offset += 8)
            for (unsigned extra : {0u, 16u, 32u}) {
                for (unsigned size : {0u, 1u, 12u, 100u, 300u}) {
                    std::memset(arena, 0, sizeof arena);
                    wanted_offset = offset; extra_bytes = extra; frees.clear(); locks = unlocks = 0;
                    unsigned char* got = static_cast<unsigned char*>(_memalign_r(nullptr, alignment, size));
                    const unsigned nb = (size + 19u < 31u) ? 16u : ((size + 19u) & ~15u);
                    if (last_request != nb + alignment + 16u) { std::puts("request size wrong"); ++bad; }
                    if (got == nullptr || (reinterpret_cast<uintptr_t>(got) % alignment) != 0) { std::printf("not aligned %u %u\n", alignment, offset); ++bad; continue; }
                    if (locks != 1 || unlocks != 1) { std::puts("lock pairing wrong"); ++bad; }
                    const unsigned head = *reinterpret_cast<unsigned*>(got - 4);
                    if ((head & ~3u) < nb) { std::puts("chunk too small"); ++bad; }
                    if ((reinterpret_cast<uintptr_t>(got) % alignment) == 0 && got != arena + offset && (frees.empty() || frees[0] != arena + offset)) {
                        std::puts("leading gap not released"); ++bad;
                    }
                }
            }
    if (bad) return 1;
    std::puts("_memalign_r contract passed");
    return 0;
}
