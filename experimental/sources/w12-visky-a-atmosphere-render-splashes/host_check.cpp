// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// All pointer words are 32-bit addresses mapped onto an arena (base 0x100000).
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "render_splashes.cpp"

// atmosphere 0x100, nodes 0x1000 (36 bytes each), billboards 0x2000 + 0x400 per batch, raster 0x6000,
// scene 0x8000 (its raster word is at +19356).
static const unsigned ATM = 0x100, NODES = 0x1000, BB = 0x2000, RAS = 0x6000, SCENE = 0x8000;
static std::string trace;
static int begin_result, begin_calls;
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string S(unsigned v) { return std::to_string(v); }

extern "C" {
void SetMaterial__8VIRasteri(unsigned char* r, int m) { trace += "material:" + S(addr(r)) + "," + std::to_string(m) + ";"; }
int BeginBillboards__8VIRasterPP11VIBillboardi(unsigned char* r, unsigned* cursor, int count) {
    trace += "begin:" + S(addr(r)) + "," + std::to_string(count) + ";";
    *cursor = arena_base + BB + 0x400 * begin_calls++;
    return begin_result;
}
void EndBillboards__8VIRaster(unsigned char* r) { trace += "end:" + S(addr(r)) + ";"; }
void Remove__7VILNode(unsigned char* n) { trace += "remove:" + S(addr(n)) + ";"; }
void AddTail__7VILListP7VILNode(unsigned char* l, unsigned char* n) { trace += "addtail:" + S(addr(l)) + "," + S(addr(n)) + ";"; }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void sf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static float rf(unsigned off) { float f; std::memcpy(&f, arena + off, 4); return f; }
// k visited nodes with the given ages, then a terminator node whose next word is zero.
static void arm(int count, const int* ages, int k) {
    std::memset(arena, 0x2e, sizeof arena);
    trace.clear(); begin_calls = 0; begin_result = 1;
    sw(SCENE + 19356, arena_base + RAS);
    sw(ATM + 20, arena_base + NODES); sw(ATM + 32, count); sw(ATM + 56, 7);
    for (int i = 0; i <= k; ++i) {
        const unsigned n = NODES + 36 * i;
        sw(n + 0, i < k ? arena_base + n + 36 : 0);
        sf(n + 16, 1.0f + i); sf(n + 20, 10.0f + i); sf(n + 24, 100.0f + i); sf(n + 28, 0.25f * (i + 1));
        sw(n + 32, i < k ? static_cast<unsigned>(ages[i]) : 77);
    }
}
static void go() { RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(arena + ATM, arena + SCENE, nullptr); }
int main() {
    // Three nodes, batch big enough for all: one Begin(…, 5), three billboards of 48 bytes, one End.
    const int ages[] = {0, 1, 2};
    arm(5, ages, 3); begin_result = 3; go();
    if (trace != "material:" + S(RAS) + ",7;begin:" + S(RAS) + ",5;end:" + S(RAS) + ";") return 1;
    for (int i = 0; i < 3; ++i) {
        const unsigned b = BB + 48 * i;
        if (rf(b + 0) != 1.0f + i || rf(b + 4) != 10.0f + i || rf(b + 8) != 100.0f + i) return 2;
        if (rw(b + 12) != static_cast<unsigned>((ages[i] + 1) << 2)) return 3;
        if (rw(b + 16) != 0x3da3d70au || rw(b + 20) != 0x3da3d70au) return 4;
        const float shade = 0.25f * (i + 1);
        if (rf(b + 32) != shade || rf(b + 36) != shade || rf(b + 40) != shade || rf(b + 44) != shade) return 5;
        // Untouched billboard fields keep their fill.
        if (arena[b + 24] != 0x2e || arena[b + 31] != 0x2e) return 6;
        if (rw(NODES + 36 * i + 32) != static_cast<unsigned>(ages[i] + 1)) return 7;
    }
    if (arena[BB + 48 * 3] != 0x2e) return 8;
    if (rw(ATM + 32) != 5) return 9;
    // A node reaching age 4 is removed and moved to the free list; the count drops by one.
    const int old[] = {3, 0, 3};
    arm(5, old, 3); begin_result = 3; go();
    if (trace != "material:" + S(RAS) + ",7;begin:" + S(RAS) + ",5;remove:" + S(NODES) + ";addtail:" + S(ATM + 36) + "," + S(NODES) + ";" +
                 "end:" + S(RAS) + ";remove:" + S(NODES + 72) + ";addtail:" + S(ATM + 36) + "," + S(NODES + 72) + ";") {
        return 10;
    }
    if (rw(ATM + 32) != 3 || rw(NODES + 32) != 4 || rw(NODES + 72 + 32) != 4 || rw(NODES + 36 + 32) != 1) return 11;
    // Batches of two: the third node starts a second batch with the remaining count.
    arm(5, ages, 3); begin_result = 2; go();
    if (trace != "material:" + S(RAS) + ",7;begin:" + S(RAS) + ",5;end:" + S(RAS) + ";begin:" + S(RAS) + ",3;end:" + S(RAS) + ";") return 12;
    if (rf(BB + 0x400 + 0) != 3.0f) return 13;
    // A batch larger than the node count is closed after the loop.
    arm(5, ages, 3); begin_result = 9; go();
    if (trace != "material:" + S(RAS) + ",7;begin:" + S(RAS) + ",5;end:" + S(RAS) + ";") return 14;
    // Only the terminator: no batch, nothing drawn.
    arm(5, ages, 0); go();
    if (trace != "material:" + S(RAS) + ",7;" || arena[BB] != 0x2e || rw(NODES + 32) != 77) return 15;
    return 0;
}
