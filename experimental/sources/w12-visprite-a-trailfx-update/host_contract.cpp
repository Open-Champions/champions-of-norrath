#define NORRATH_HOST_CONTRACT 1
#include "update.cpp"
#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

static std::vector<unsigned> cleared;
extern "C" void ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(void* t, void* s) {
    cleared.push_back(off_of(s));
    // emulate: the segment and its followers leave the list (the head word is repointed if needed)
    unsigned prev; std::memcpy(&prev, static_cast<char*>(s) + 4, 4);
    put(prev, 0x1f00);   // the predecessor now points at the end marker
}

// nodes at 0x1000 + 0x100 * i; header (embedded list) at trail + 28; end marker at 0x1f00
static void build(int n, float age0, float step) {
    std::memset(arena, 0, sizeof arena);
    const unsigned trail = 0x100;
    put(trail + 28, 0x1000);
    for (int i = 0; i < n; ++i) {
        unsigned a = 0x1000 + 0x100 * i;
        put(a, i + 1 < n ? a + 0x100 : 0x1f00);
        put(a + 4, i == 0 ? trail + 28 : a - 0x100);
        putf(a + 72, age0 + step * i);
    }
    put(0x1f00 + 4, n ? 0x1000 + 0x100 * (n - 1) : trail + 28);
    putf(trail + 24, 2.0f);
    cleared.clear();
}

int main() {
    const unsigned trail = 0x100;
    // times: 64-bit; only the low 32 bits of the difference are used
    long long t0 = 5000000000LL;
    std::memcpy(arena + trail + 16, &t0, 8);
    build(4, 0.0f, 0.5f);
    std::memcpy(arena + trail + 16, &t0, 8);
    // dt = 1000 ticks -> 1.0 s. ages 0, .5, 1, 1.5 become 1, 1.5, 2, 2.5
    int r = Update__9VITrailFxl(arena + trail, t0 + 1000);
    long long stored; std::memcpy(&stored, arena + trail + 16, 8);
    if (stored != t0 + 1000) return 1;
    if (getf(0x1000 + 72) != 1.0f || getf(0x1100 + 72) != 1.5f || getf(0x1200 + 72) != 2.0f) return 2;
    // node 2 expired and its predecessor (1.5) is not: kept. node 3 expired and node 2 expired: cleared from 3.
    if (cleared.size() != 1 || cleared[0] != 0x1300) return 3;
    if (getf(0x1300 + 72) != 2.5f) return 4;
    if (r != 1) return 5;
    // first node expired: cleared from the first node, which empties the list (stub repoints nothing at head)
    build(3, 3.0f, 0.0f);
    std::memcpy(arena + trail + 16, &t0, 8);
    Update__9VITrailFxl(arena + trail, t0 + 500);
    if (cleared.size() != 1 || cleared[0] != 0x1000) return 6;
    // the difference wraps to 32 bits: time - previous = 2^32 + 250 converts as 250
    build(2, 0.0f, 0.0f);
    long long t1 = 7;
    std::memcpy(arena + trail + 16, &t1, 8);
    Update__9VITrailFxl(arena + trail, t1 + (1LL << 32) + 250);
    if (getf(0x1000 + 72) != 250 * 0.001f) return 7;
    // no segments: nothing is touched and the result is 0
    build(0, 0, 0);
    std::memcpy(arena + trail + 16, &t0, 8);
    if (Update__9VITrailFxl(arena + trail, t0 + 5) != 0 || !cleared.empty()) return 8;
    return 0;
}
