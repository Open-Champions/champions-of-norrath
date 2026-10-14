// Host contract for __8LostSoulG6Point3T1iP10_vagHeaderii; not an EE input. 32-bit words are offsets from a host arena; every callee
// logs its name and key arguments. The authored function was also compared with the original in a private R5900 run (40 scenarios,
// every original word but one nop covered; mutants of the frame offsets, the trail constants, the light constants and the final
// deactivation test are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
static Point3 got_position, trail_position;
static void *got_owner, *got_map;
static unsigned char light_at_add[64];
static void* deactivated;
static int trail_args[8]; static float trail_float; static int trail_id_seen;
extern "C" {
int frameNm;
unsigned char vt_LostSoul[16] __asm__("_vt$8LostSoul");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void projectile_construct(void* self, Point3 position, void* owner, void* map) {
    calls += "ctor;"; got_position = position; got_owner = owner; got_map = map;
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, &position, 12);
}
void lightAdd__FP5Light(void* light) { calls += "light;"; std::memcpy(light_at_add, light, sizeof light_at_add); }
void lightDeactivate__FP5Light(void* light) { calls += "deact;"; deactivated = light; }
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls += "trail;"; trail_args[0] = a; trail_args[1] = b; trail_args[2] = d; trail_args[3] = e; trail_args[4] = f; trail_args[5] = g; trail_args[6] = h; trail_float = c;
    return 41;
}
void P_ExtendTrail__FiG6Point3(int id, Point3 position) { calls += "extend;"; trail_id_seen = id; trail_position = position; }
}
#include "ctor.cpp"

static int run(int b, int c, bool expect_deactivate) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); deactivated = nullptr; frameNm = 1000;
    unsigned char* o = arena + 0x100;
    Point3 p = {1.5f, -2.0f, 300.0f}, v = {6.0f, -12.0f, 3.0f};
    __8LostSoulG6Point3T1iP10_vagHeaderii(o, p, v, 0x12345, arena + 0x400, b, c);
    std::string want = "ctor;light;run;trail;extend;";
    if (expect_deactivate) want += "deact;";
    if (calls != want) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f || got_position.z != 300.0f || got_owner != nullptr || got_map != nullptr) return 2;
    if (word(o, 72) != to_word(vt_LostSoul)) return 3;
    const unsigned char* l = light_at_add;
    if (l[8] != 0 || l[9] != 0) return 4;
    const short want_h[8] = {172, 252, 500, -32, 172, 252, 500, -32};
    const int offs[8] = {10, 12, 14, 16, 18, 20, 22, 24};
    for (int i = 0; i < 8; ++i) if (*reinterpret_cast<const short*>(l + offs[i]) != want_h[i]) return 5;
    if (*reinterpret_cast<const float*>(l + 28) != 120.0f || *reinterpret_cast<const float*>(l + 36) != 120.0f) return 6;
    if (*reinterpret_cast<const unsigned*>(l + 32) != 0x3891a2b3u || *reinterpret_cast<const unsigned*>(l + 40) != 0x3891a2b3u) return 7;
    if (*reinterpret_cast<const float*>(l + 44) != 0.0f) return 8;
    if (real(o, 148) != 1.5f || real(o, 152) != -2.0f || real(o, 156) != 300.0f) return 9;
    if (real(o, 28) != 0.0f || half(o, 56) != 0x2345 || real(o, 88) != 6.0f || real(o, 92) != -12.0f || real(o, 96) != 3.0f) return 10;
    if (word(o, 168) != static_cast<unsigned>(c) || word(o, 164) != static_cast<unsigned>(b) || word(o, 12) != 0x4001u) return 11;
    if (word(o, 84) != 1000u + (b == 1 ? 60u : 600u)) return 12;
    if (trail_args[0] != 4 || trail_args[1] != 750 || trail_args[2] != 4 || trail_args[3] != 107 || trail_args[4] != 100 || trail_args[5] != 234 || trail_args[6] != 171) return 13;
    if (trail_float != bits(0x3ea4dd2fu) || word(o, 172) != 41u || trail_id_seen != 41) return 14;
    if (trail_position.x != 1.5f || trail_position.y != -2.0f || trail_position.z != 300.0f) return 15;
    if (expect_deactivate && deactivated != o + 100) return 16;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    if ((r = run(1, 0, true))) return r;
    if ((r = run(1, 5, false))) return 20 + r;
    if ((r = run(0, 0, false))) return 40 + r;
    return (r = run(2, 0, false)) ? 60 + r : 0;
}
