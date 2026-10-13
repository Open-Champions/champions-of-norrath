#define NORRATH_HOST_CONTRACT 1
#include "render_by_portals.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }

struct Ev { int kind; unsigned a, b, c; };
static std::vector<Ev> ev;
static int leaf_result, leaf_x, leaf_y, frustum_result, pretrans_result, push_result, room_result, pop_result;

extern "C" int FindLeaf__C7VIWorldRC7VIVect3RiT2(VIWorld* w, const void* p, int* x, int* y) {
    ev.push_back({1, off_of(w), off_of(p), 0});
    if (leaf_result >= 0) { *x = leaf_x; *y = leaf_y; }
    return leaf_result;
}
extern "C" int RenderByFrustum__7VIWorldR15VISceneRendVars(VIWorld* w, VISceneRendVars* v) { ev.push_back({2, off_of(w), off_of(v), 0}); return frustum_result; }
extern "C" int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void* z, unsigned r, unsigned c) { ev.push_back({3, off_of(z), r, c}); return pretrans_result; }
extern "C" int PushFrustum__11VIClipStackR8VICamera(unsigned s, unsigned c) { ev.push_back({4, s, c, 0}); return push_result; }
extern "C" void GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(unsigned s, void* a, void* b, void* c, void* d) {
    ev.push_back({5, s, off_of(a), off_of(d)});
    if (off_of(b) != off_of(a) + 4 || off_of(c) != off_of(a) + 8 || off_of(d) != off_of(a) + 12) ev.push_back({99, 0, 0, 0});
}
extern "C" int RenderRoom__6VIZoneiR15VISceneRendVars(void* z, int room, VISceneRendVars* v) { ev.push_back({6, off_of(z), (unsigned)room, off_of(v)}); return room_result; }
extern "C" int Pop__11VIClipStack(unsigned s) { ev.push_back({7, s, 0, 0}); return pop_result; }

int main() {
    const unsigned world = 0x100, vars = 0x400, camera = 0x600, zones = 0x1000, table = 0x3000, rooms = 0x4000;
    auto reset = [&](unsigned w52, unsigned w60, unsigned flag) {
        std::memset(arena, 0, sizeof arena); ev.clear();
        put(world + 52, w52); put(world + 60, w60); put(world + 76, zones);
        put(vars + 12, camera); put(vars + 16, 0x7777); put(vars + 20, 0x2222); put(vars + 48, 0xffff);
        put(zones + 592 * 3 + 80, table); put(zones + 592 * 3 + 96, rooms);
        put(table + 8 * 2, 5);                    // room index for y = 2
        put(rooms + 120 * 5, flag);
        leaf_result = 0; leaf_x = 3; leaf_y = 2; frustum_result = 0; pretrans_result = 0; push_result = 0; room_result = 0; pop_result = 0;
    };
    VIWorld* w = reinterpret_cast<VIWorld*>(arena + world);
    VISceneRendVars* v = reinterpret_cast<VISceneRendVars*>(arena + vars);
    // empty tree
    reset(0, 0, 0);
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != 0 || !ev.empty() || get(vars + 8) != (unsigned)(unsigned long)w) return 1;
    // FindLeaf failure
    reset(1, 0, 0); leaf_result = -4;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 1) return 2;
    if (ev[0].a != world || ev[0].b != camera + 28) return 3;
    // flag set: delegate to RenderByFrustum
    reset(0, 1, 0x1); frustum_result = 9;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != 0 || ev.size() != 2 || ev[1].kind != 2 || ev[1].a != world || ev[1].b != vars) return 4;
    reset(0, 1, 0xff); frustum_result = -1;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1) return 5;
    // flag clear: full path
    reset(1, 1, 0xfe);
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != 0) return 6;
    if (ev.size() != 6) return 7;
    if (ev[1].kind != 3 || ev[1].a != zones + 592 * 3 || ev[1].b != 0x7777 || ev[1].c != camera) return 8;
    if (ev[2].kind != 4 || ev[2].a != 0x2222 || ev[2].b != camera) return 9;
    if (ev[3].kind != 5 || ev[3].a != 0x2222 || ev[3].b != vars + 24 || ev[3].c != vars + 36) return 10;
    if (ev[4].kind != 6 || ev[4].a != zones + 592 * 3 || ev[4].b != 5 || ev[4].c != vars) return 11;
    if (ev[5].kind != 7 || ev[5].a != 0x2222) return 12;
    if (get(vars + 48) != 3) return 13;
    // failures at each step
    reset(1, 1, 0); pretrans_result = -1;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 2) return 14;
    reset(1, 1, 0); push_result = -1;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 3) return 15;
    reset(1, 1, 0); room_result = -1;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 5) return 16;
    reset(1, 1, 0); pop_result = -3;
    if (RenderByPortals__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 6) return 17;
    for (auto& e : ev) if (e.kind == 99) return 18;
    return 0;
}
