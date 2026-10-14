// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "destroypool.cpp"
#include <cstdio>
#include <sys/mman.h>
#include <cstring>
#include <string>
static unsigned char* arena;
static const unsigned arena_size = 0x8000;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned* W(unsigned a) { return reinterpret_cast<unsigned*>(arena + a); }
static std::string trace;
static std::string n(unsigned v) { return std::to_string(v); }
// Elements are filled with 0x5a bytes, so a wrong element size, link offset or start word reads a wild
// index. The arena sits in the middle of an unreadable 4 GiB reservation, so the wild walk faults and the
// check crashes instead of passing.
int main() {
    unsigned char* base = static_cast<unsigned char*>(mmap(nullptr, 1ull << 32, PROT_NONE, MAP_PRIVATE | MAP_ANON | MAP_NORESERVE, -1, 0));
    if (base == MAP_FAILED) return 3;
    arena = base + 0x80000000u;
    if (mprotect(arena, 0x8000, PROT_READ | PROT_WRITE) != 0) return 4;
    bool ok = true;
    for (int variant = 0; variant < 4; ++variant) {
        std::memset(arena, 0x5a, arena_size);
        unsigned* p = W(0x100);
        p[2] = 0x1000;
        unsigned char* e = arena + 0x1000;
        auto link = [&](int i, int next) { std::memcpy(e + 12 * i + 4, &next, 4); };
        p[4] = variant == 0 ? 0xffffffffu : variant == 1 ? 2u : 5u;
        if (variant == 1) link(2, -1);
        if (variant >= 2) { link(5, 1); link(1, 3); link(3, variant == 3 ? 0 : -1); if (variant == 3) link(0, -1); }
        static unsigned char before[0x8000];
        std::memcpy(before, arena, 0x8000);
        if (DestroyPool__t6VIPool1ZP15VIParticleMotif(p) != 0) ok = false;
        if (std::memcmp(before, arena, 0x8000) != 0) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
