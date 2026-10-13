#define NORRATH_HOST_CONTRACT 1
#include "world_init.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }

struct Ev { int kind; unsigned a; int b, c; };
static std::vector<Ev> ev;
static int fail_at;      // 1-based index of the event whose callee returns -1
static int calls;
static int result_for(int kind) { ++calls; return calls == fail_at ? -1 : 5; }

extern "C" int Init__t7VIArray1Z6VIZonei(void* a, int n) { ev.push_back({1, off_of(a), n, 0}); return result_for(1); }
extern "C" int Init__6VIZone(void* z) { ev.push_back({2, off_of(z), 0, 0}); return result_for(2); }
extern "C" int Init__11VIWorldTreeii(void* t, int a, int b) { ev.push_back({3, off_of(t), a, b}); return result_for(3); }
extern "C" int Init__t7VIArray1Z13VIWorldRegioni(void* a, int n) { ev.push_back({4, off_of(a), n, 0}); return result_for(4); }
extern "C" int SetProfiles__7VIWorld(VIWorld* w) { ev.push_back({5, off_of(w), 0, 0}); return result_for(5); }

int main() {
    const unsigned world = 0x100, zones = 0x1000, tree_ptr = 0x2000, entries = 0x3000, box = 0x400, vec = 0x440;
    auto reset = [&]() {
        std::memset(arena, 0x5a, sizeof arena); ev.clear(); calls = 0; fail_at = 0;
        put(world + 76, zones); put(world + 64, tree_ptr); put(zones + 96, entries); put(tree_ptr, 0x12345678);
        for (int i = 0; i < 6; ++i) putf(box + 4 * i, 10.0f + i);
        for (int i = 0; i < 3; ++i) putf(vec + 4 * i, 100.0f + i);
    };
    VIWorld* w = reinterpret_cast<VIWorld*>(arena + world);
    const VIBBox* b = reinterpret_cast<const VIBBox*>(arena + box);
    const VIVect3* v = reinterpret_cast<const VIVect3*>(arena + vec);
    // one zone: corners are copied into the zone and its first entry
    reset();
    if (Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 1, 3, 4, 5, 7.5f, v, b) != 0) return 1;
    if (ev.size() != 5) return 2;
    if (ev[0].kind != 1 || ev[0].a != world + 72 || ev[0].b != 1) return 3;
    if (ev[1].kind != 2 || ev[1].a != zones) return 4;
    if (ev[2].kind != 3 || ev[2].a != world + 52 || ev[2].b != 0 || ev[2].c != 1) return 5;
    if (ev[3].kind != 4 || ev[3].a != world + 80 || ev[3].b != (3 * 5) * (5 * 4)) return 6;
    if (ev[4].kind != 5 || ev[4].a != world) return 7;
    for (int i = 0; i < 3; ++i) {
        if (getf(zones + 44 + 4 * i) != 10.0f + i || getf(zones + 56 + 4 * i) != 13.0f + i) return 8;
        if (getf(entries + 4 + 4 * i) != 10.0f + i || getf(entries + 16 + 4 * i) != 13.0f + i) return 9;
        if (getf(world + 4 * i) != 10.0f + i || getf(world + 12 + 4 * i) != 13.0f + i) return 10;
        if (getf(world + 36 + 4 * i) != 100.0f + i) return 11;
    }
    if (getf(world + 32) != 7.5f || get(world + 24) != 3 || get(world + 28) != 4 || get(world + 48) != 5) return 12;
    if (get(tree_ptr) != 0) return 13;
    // three zones: each initialised in order, nothing copied into zone 0
    reset();
    if (Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 3, 1, 1, 2, 0.0f, v, b) != 0 || ev.size() != 7) return 14;
    for (int i = 0; i < 3; ++i) if (ev[1 + i].kind != 2 || ev[1 + i].a != zones + 592 * i) return 15;
    if (get(zones + 44) != 0x5a5a5a5au) return 16;
    // zero zones: no element calls
    reset();
    Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 0, 1, 1, 1, 0.0f, v, b);
    if (ev.size() != 4 || ev[1].kind != 3) return 17;
    // each failure point returns -1 and stops
    for (int fail = 1; fail <= 5; ++fail) {
        reset(); fail_at = fail;
        if (Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 1, 1, 1, 1, 1.0f, v, b) != -1) return 18;
        if (ev.size() != (size_t)fail) return 19;
    }
    // overflowing product wraps like the original's 32-bit multiply
    reset();
    Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 0, 65536, 1, 65536, 0.0f, v, b);
    if (ev.size() != 4 || ev[2].kind != 4 || ev[2].b != 0) return 20;
    reset();
    Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(w, 0, 7, 11, 13, 0.0f, v, b);
    if (ev[2].b != (7 * 13) * (13 * 11)) return 21;
    return 0;
}
