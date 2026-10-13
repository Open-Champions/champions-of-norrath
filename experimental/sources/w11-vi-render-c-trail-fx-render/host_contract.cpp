#define NORRATH_HOST_CONTRACT 1
unsigned norrath_trail_free_def[4] __asm__("VITrailFx$sFreeTrailFx");
unsigned norrath_trail_active_def[4] __asm__("VITrailFx$sActiveTrailFx");
unsigned norrath_trail_inactive_def[4] __asm__("VITrailFx$sInactiveTrailFx");
#include "trail_fx_render.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }

struct Ev { int kind; unsigned a; long b; };
static std::vector<Ev> ev;
static int update_result[8];     // by node offset / 0x40

extern "C" int Update__9VITrailFxl(void* t, long time) { ev.push_back({1, off_of(t), time}); return update_result[off_of(t) / 0x40 - 0x40]; }
extern "C" void Render__9VITrailFxR8VIRasterR8VICamera(void* t, VIRaster* r, VICamera* c) {
    ev.push_back({2, off_of(t), (long)((unsigned long)r * 1000 + (unsigned long)c)});
}
extern "C" void Remove__7VILNode(void* n) { ev.push_back({3, off_of(n), 0}); }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) {
    ev.push_back({4, off_of(n), l == norrath_trail_free_def ? 1 : 0});
}

int main() {
    std::memset(arena, 0, sizeof arena);
    // nodes at 0x1000, 0x1040, 0x1080 (active); 0x2000... wait: keep node index = offset / 0x40 - 0x40
    // active: A0 = 0x1000 -> 0x1040 -> 0x1080 -> sentinel 0x10c0 (next word 0)
    put(0x1000, 0x1040); put(0x1040, 0x1080); put(0x1080, 0x10c0); put(0x10c0, 0);
    // inactive: 0x1100 -> 0x1140 -> 0x1180 -> sentinel 0x11c0
    put(0x1100, 0x1140); put(0x1140, 0x1180); put(0x1180, 0x11c0); put(0x11c0, 0);
    norrath_trail_active_def[0] = 0x1000;
    norrath_trail_inactive_def[0] = 0x1100;
    // result by index: 0x1000/0x40-0x40 = 0
    update_result[0] = 5; update_result[1] = 0; update_result[2] = -3;     // active nodes
    update_result[4] = 1; update_result[5] = 0; update_result[6] = -1;     // inactive nodes
    VIRaster* r = reinterpret_cast<VIRaster*>(7); VICamera* c = reinterpret_cast<VICamera*>(9);
    VITrailFxRender__FlR8VIRasterR8VICamera(1234, r, c);
    struct { int kind; unsigned a; long b; } want[] = {
        {1, 0x1000, 1234}, {2, 0x1000, 7009},
        {1, 0x1040, 1234},
        {1, 0x1080, 1234},
        {1, 0x1100, 1234}, {2, 0x1100, 7009},
        {1, 0x1140, 1234}, {3, 0x1140, 0}, {4, 0x1140, 1},
        {1, 0x1180, 1234}, {3, 0x1180, 0}, {4, 0x1180, 1},
    };
    if (ev.size() != sizeof want / sizeof want[0]) return 1;
    for (unsigned i = 0; i < ev.size(); ++i)
        if (ev[i].kind != want[i].kind || ev[i].a != want[i].a || ev[i].b != want[i].b) return 2;
    // empty lists
    ev.clear();
    norrath_trail_active_def[0] = 0x10c0; norrath_trail_inactive_def[0] = 0x11c0;
    VITrailFxRender__FlR8VIRasterR8VICamera(1, r, c);
    if (!ev.empty()) return 3;
    return 0;
}
