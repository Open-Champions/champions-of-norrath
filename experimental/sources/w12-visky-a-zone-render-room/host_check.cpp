// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// All pointer words are 32-bit addresses mapped onto an arena (base 0x100000).
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "render_room.cpp"

// Layout: zone 0x100; records 0x2000 (120 bytes each); scene 0x4000; camera 0x4800; raster 0x5000;
// clip stack 0x5800; vars 0x6000; actor nodes 0x7000 + 64k.
static const unsigned ZONE = 0x100, RECS = 0x2000, SCENE = 0x4000, CAM = 0x4800, RAS = 0x5000, CLIP = 0x5800, VARS = 0x6000, NODES = 0x7000;

static std::string trace;
static int r_cull, r_portals, r_sprite, r_add, r_actor_fail_at, actors_seen;
static float r_dist;
static unsigned char element_seen[12], color_seen[16], matrix_seen[64];
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

extern "C" {
int CullByTop__11VIClipStackRC6VIBBox(unsigned char* clip, const unsigned char* box) { trace += "cull:" + std::to_string(addr(clip)) + "," + std::to_string(addr(box)) + ";"; return r_cull; }
int RenderThroughPortals__6VIZoneiR15VISceneRendVars(unsigned char* z, int room, unsigned char* v) { trace += "portals:" + std::to_string(addr(z)) + "," + std::to_string(room) + "," + std::to_string(addr(v)) + ";"; return r_portals; }
void sceVu0CopyMatrix(unsigned char* dst, const unsigned char* src) { trace += "copy:" + std::to_string(addr(dst)) + "," + std::to_string(addr(src)) + ";"; std::memcpy(dst, src, 64); }
int SetModelView__8VIRasterRC10VIMatrix44(unsigned char* ras, const unsigned char* m) { trace += "modelview:" + std::to_string(addr(ras)) + "," + std::to_string(addr(m)) + ";"; std::memcpy(matrix_seen, m, 64); return 0; }
int SetStaticLighting__8VIRasterRC10VIColor32F(unsigned char* ras, const float* c) { trace += "light:" + std::to_string(addr(ras)) + ";"; std::memcpy(color_seen, c, 16); return 0; }
int RasterSprite__7VISceneR8VICameraiUi(unsigned char* s, unsigned char* c, int sprite, unsigned flag) {
    trace += "sprite:" + std::to_string(addr(s)) + "," + std::to_string(addr(c)) + "," + std::to_string(sprite) + "," + std::to_string(flag) + ";";
    return r_sprite;
}
int Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem(unsigned char* vec, const unsigned* e) {
    trace += "add:" + std::to_string(addr(vec)) + ";"; std::memcpy(element_seen, e, 12); return r_add;
}
float DistanceSquared__C6VIBBoxRC7VIVect3(const unsigned char* box, const unsigned char* p) { trace += "dist:" + std::to_string(addr(box)) + "," + std::to_string(addr(p)) + ";"; return r_dist; }
int RasterActor__7VISceneR8VICameraiR11VIClipStack(unsigned char* s, unsigned char* c, int actor, unsigned char* clip) {
    trace += "actor:" + std::to_string(addr(s)) + "," + std::to_string(addr(c)) + "," + std::to_string(actor) + "," + std::to_string(addr(clip)) + ";";
    return ++actors_seen == r_actor_fail_at ? -1 : 0;
}
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void arm(unsigned flags, unsigned sprite, unsigned last, unsigned frame, float limit, int nodes) {
    std::memset(arena, 0x2e, sizeof arena);
    r_cull = 0; r_portals = 0; r_sprite = 1; r_add = 0; r_dist = 50.0f; r_actor_fail_at = 0; actors_seen = 0;
    trace.clear();
    sw(ZONE + 96, arena_base + RECS);
    sw(VARS + 4, arena_base + SCENE); sw(VARS + 12, arena_base + CAM); sw(VARS + 16, arena_base + RAS);
    sw(VARS + 20, arena_base + CLIP); sw(VARS + 48, 0xabcd0123u);
    std::memcpy(arena + VARS + 52, &limit, 4);
    sw(SCENE + 1348, frame);
    const unsigned rec = RECS + 120 * 2;
    sw(rec + 0, flags); sw(rec + 28, sprite); sw(rec + 36, last);
    sw(rec + 104, nodes ? arena_base + NODES : 0xffffffffu);
    for (int k = 0; k < nodes; ++k) { sw(NODES + 64 * k + 4, 500 + k); sw(NODES + 64 * k + 16, k + 1 < nodes ? arena_base + NODES + 64 * (k + 1) : 0xffffffffu); }
    for (int i = 0; i < 16; ++i) { float f = 1.5f * i; std::memcpy(arena + CAM + 48 + 4 * i, &f, 4); }
}
static int run() { return RenderRoom__6VIZoneiR15VISceneRendVars(arena + ZONE, 2, arena + VARS); }
static std::string S(unsigned a) { return std::to_string(a); }

int main() {
    // Full path: sprite drawn and added, then two actors.
    arm(0, 9, 76, 77, 10.0f, 2);
    if (run() != 0) return 1;
    std::string want = "cull:" + S(CLIP) + "," + S(RECS + 244) + ";portals:" + S(ZONE) + ",2," + S(VARS) + ";copy:" + S(ZONE + 144) + "," + S(CAM + 48) + ";" +
                       "modelview:" + S(RAS) + "," + S(ZONE + 144) + ";light:" + S(RAS) + ";sprite:" + S(SCENE) + "," + S(CAM) + ",9,1;add:" + S(SCENE + 1112) + ";" +
                       "dist:" + S(RECS + 244) + "," + S(CAM + 28) + ";actor:" + S(SCENE) + "," + S(CAM) + ",500," + S(CLIP) + ";actor:" + S(SCENE) + "," + S(CAM) + ",501," + S(CLIP) + ";";
    if (trace != want) return 2;
    if (rw(RECS + 240 + 36) != 77) return 3;
    // Matrix: copied from camera+48, then the last row replaced by (0, 0, 0, 1.0).
    unsigned char expect_m[64];
    for (int i = 0; i < 16; ++i) { float f = 1.5f * i; std::memcpy(expect_m + 4 * i, &f, 4); }
    std::memset(expect_m + 48, 0, 12); { float one = 1.0f; std::memcpy(expect_m + 60, &one, 4); }
    if (std::memcmp(matrix_seen, expect_m, 64) != 0 || std::memcmp(arena + ZONE + 144, expect_m, 64) != 0) return 4;
    { float c[4] = {0, 0, 0, 1.0f}; if (std::memcmp(color_seen, c, 16) != 0) return 5; }
    { unsigned e[3] = {0xabcd0123u, 0xffffffffu, 9}; if (std::memcmp(element_seen, e, 12) != 0) return 6; }
    // Cull rejects: only the cull call, result 0, record untouched.
    arm(0, 9, 76, 77, 10.0f, 2); r_cull = 1;
    if (run() != 0 || trace != "cull:" + S(CLIP) + "," + S(RECS + 244) + ";" || rw(RECS + 240 + 36) != 76) return 7;
    // Portal failure: -1 after two calls.
    arm(0, 9, 76, 77, 10.0f, 2); r_portals = -3;
    if (run() != -1 || trace.find("sprite") != std::string::npos || trace.find("dist") != std::string::npos) return 8;
    // Already drawn this frame, or flag bit 2: skip the sprite entirely.
    arm(0, 9, 77, 77, 10.0f, 0);
    if (run() != 0 || trace.find("copy") != std::string::npos || trace.find("dist") == std::string::npos) return 9;
    arm(2, 9, 76, 77, 10.0f, 0);
    if (run() != 0 || trace.find("copy") != std::string::npos || rw(RECS + 240 + 36) != 76) return 10;
    arm(1, 9, 76, 77, 10.0f, 0);
    if (run() != 0 || trace.find("copy") == std::string::npos) return 11;
    // Sprite id -1: the frame is still recorded, but nothing is drawn.
    arm(0, 0xffffffffu, 76, 77, 10.0f, 0);
    if (run() != 0 || trace.find("copy") != std::string::npos || rw(RECS + 240 + 36) != 77) return 12;
    // Sprite id 0 is a valid id.
    arm(0, 0, 76, 77, 10.0f, 0);
    if (run() != 0 || trace.find("sprite:" + S(SCENE) + "," + S(CAM) + ",0,1;") == std::string::npos) return 13;
    // RasterSprite returning 0: no Add. Add failing: -1 and no distance test.
    arm(0, 9, 76, 77, 10.0f, 0); r_sprite = 0;
    if (run() != 0 || trace.find("add:") != std::string::npos || trace.find("dist") == std::string::npos) return 14;
    arm(0, 9, 76, 77, 10.0f, 2); r_add = -1;
    if (run() != -1 || trace.find("dist") != std::string::npos) return 15;
    // Distance gate: below the squared limit walks the actors, at or above does not.
    arm(0, 0xffffffffu, 77, 77, 10.0f, 2); r_dist = 99.5f;
    if (run() != 0 || actors_seen != 2) return 16;
    arm(0, 0xffffffffu, 77, 77, 10.0f, 2); r_dist = 100.0f;
    if (run() != 0 || actors_seen != 0) return 17;
    arm(0, 0xffffffffu, 77, 77, -10.0f, 2); r_dist = 99.0f;
    if (run() != 0 || actors_seen != 2) return 18;
    // An actor failure stops the walk with -1; an empty list is fine.
    arm(0, 0xffffffffu, 77, 77, 10.0f, 3); r_actor_fail_at = 2;
    if (run() != -1 || actors_seen != 2) return 19;
    arm(0, 0xffffffffu, 77, 77, 10.0f, 0);
    if (run() != 0 || actors_seen != 0) return 20;
    return 0;
}
