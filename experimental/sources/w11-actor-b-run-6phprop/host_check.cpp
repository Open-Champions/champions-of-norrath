// Host contract for msg_run__6PhProp; not an EE input. Build with the host compiler and -fno-builtin
// (sprintf and strcpy are called by name). Words are offsets from w11_actor_b_host_base; pointers inside the
// object (body, body position, position) are plain host pointers.
#define W11_ACTOR_B_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
int frameNm, levelFlags;
void* world = reinterpret_cast<void*>(0x1234);
unsigned char vt_4Prop[8];
static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char hullBytes[24];
static std::string calls;
static int waterResult, allocCount;
static float velocityZ, standHeight, clipFraction;
static Point3 clipHit;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
static std::string pt(const Point3& p) { return "(" + f(p.x) + "," + f(p.y) + "," + f(p.z) + ")"; }
static std::string ptp(const Point3* p) { return pt(*p); }
extern "C" {
int format_text(char* d, const char* fmt, ...) __asm__("sprintf");
int format_text(char* d, const char* fmt, ...) { std::strcpy(d, "foly_wtrsplash_sml1.anm"); calls += std::string("sprintf(") + fmt + ");"; return 0; }
char* copy_text(char* d, const char* s) __asm__("strcpy");
char* copy_text(char* d, const char* s) { std::strcpy(d, s); calls += "strcpy(" + off(d) + "," + off(s) + ");"; return d; }
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, Point3 p, float r, float* o) { calls += "water" + pt(p) + "," + f(r) + (o ? "O" : "") + ";"; return waterResult; }
char* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return reinterpret_cast<char*>(0x500); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) { calls += "snd(" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(h)) + "," + off(o) + "," + f(r) + ");"; }
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, Point3 p, float a, float b) { calls += "pert" + pt(p) + "," + f(a) + "," + f(b) + ";"; }
signed char* modelGetCollision__FP12_modelHeaderi(void*, int i) { calls += "coll" + std::to_string(i) + ";"; return reinterpret_cast<signed char*>(hullBytes); }
void matUntransform__FP6Point3P8Matrix33T0(Point3* o, void* m, Point3* in) { calls += "unt" + pt(*in) + ";"; o->x = in->x + 1.0f; o->y = in->y * 2.0f; o->z = in->z - 0.5f; (void)m; }
void bodyGetVelocity__FP5_bodyP6Point3T1(void*, Point3* p, Point3* o) { calls += "vel" + pt(*p) + ";"; o->x = 0.5f; o->y = -0.25f; o->z = velocityZ; }
void bodyApplyImpulse__FP5_bodyG6Point3T1f(void*, Point3* p, Point3* d, float a) { calls += "imp" + pt(*p) + pt(*d) + f(a) + ";"; }
void bodyAddForce__FP5_bodyP6Point3N31(void*, Point3* p, Point3* a, Point3* b, Point3* c) { calls += "addf" + pt(*p) + pt(*a) + pt(*b) + pt(*c) + ";"; }
void bodyApplyForce__FP5_bodyG6Point3T1f(void*, Point3* a, Point3* b, float c) { calls += "appf" + pt(*a) + pt(*b) + f(c) + ";"; }
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void*, Point3 p, float r) { calls += "stand" + pt(p) + "," + f(r) + ";"; return standHeight; }
float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void*, Point3 p, Point3* m, float r, int fl, Point3* o) {
    calls += "clip" + pt(p) + pt(*m) + "," + f(r) + "," + std::to_string(fl) + ";"; *o = clipHit; return clipFraction;
}
void fxFlyingFire__FP6Point3T0ffb(Point3* p, Point3* v, float a, float b, bool c) { calls += "fire" + pt(*p) + pt(*v) + f(a) + "," + f(b) + (c ? "T" : "F") + ";"; }
void objectUpdateInGrid__FP10GameObject(void* o) { calls += "upd" + off(o) + ";"; }
void objectAddToRunList__FP10GameObject(void* o) { calls += "run" + off(o) + ";"; }
void objectAddToDeleteList__FP10GameObject(void* o) { calls += "del" + off(o) + ";"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + std::to_string(n) + ";"; if (!allocCount) return nullptr; return arena + 0x8000 + 0x1000 * (allocCount - 1); }
void* ctor_StaticFire(void* s, Point3 p, float a, int b, float c) { calls += "sfire" + off(s) + pt(p) + "," + f(a) + "," + std::to_string(b) + "," + f(c) + ";"; return s; }
void* ctor_ItemProp(void* s, Point3 p, void* d, void* m) { calls += "item" + off(s) + pt(p) + "," + off(d) + "," + off(m) + ";"; return s; }
void* ctor_GameObject(void* s, Point3 p, void* m, char** n) { calls += "goctor" + off(s) + pt(p) + (m ? "M" : "") + (n ? "N" : "") + ";"; return s; }
void matScale__FP8Matrix34f(void*, float s) { calls += "mscale" + f(s) + ";"; }
void matTrans__FP8Matrix34fff(void*, float x, float y, float z) { calls += "mtrans" + f(x) + "," + f(y) + "," + f(z) + ";"; }
}
static void set_real(unsigned o, float v) { std::memcpy(arena + o, &v, 4); }
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void set_half(unsigned o, short v) { std::memcpy(arena + o, &v, 2); }
static float get_real(unsigned o) { float v; std::memcpy(&v, arena + o, 4); return v; }
static unsigned get_word(unsigned o) { unsigned v; std::memcpy(&v, arena + o, 4); return v; }
static short get_half(unsigned o) { short v; std::memcpy(&v, arena + o, 2); return v; }
static unsigned long long get_dword(unsigned o) { unsigned long long v; std::memcpy(&v, arena + o, 8); return v; }

static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(hullBytes, 0, sizeof hullBytes);
    w11_actor_b_host_base = arena;
    frameNm = 1; levelFlags = 0; waterResult = 0; allocCount = 1; velocityZ = 0; standHeight = 0; clipFraction = 1.0f; clipHit = Point3{0, 0, 0};
    set_real(400, 2.0f); set_real(404, 1.0f); set_real(408, 1.0f); set_real(412, 1.0f);
    set_half(270, 5);     // life counter: positive -> plain return
    set_real(16, 1.0f); set_real(20, 2.0f); set_real(24, 3.0f); set_real(28, 0.5f);
    calls.clear();
}
#define CHECK(cond, msg) do { if (!(cond)) { std::puts(calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)

int main() {
    // gravity: the float at +452 loses 0.18 / mass, or 0.054 / mass with the level bit
    reset(); set_real(452, 1.0f); msg_run__6PhProp(arena);
    CHECK(get_real(452) > 0.909f && get_real(452) < 0.9100001f, "gravity");
    reset(); set_real(452, 1.0f); levelFlags = 0x100; msg_run__6PhProp(arena);
    CHECK(get_real(452) > 0.972f && get_real(452) < 0.9731f, "gravity with the level bit");
    // water splash: only every eighth frame, only before the touched byte is set
    reset(); frameNm = 8; waterResult = 1; set_word(68, 7); msg_run__6PhProp(arena);
    CHECK(calls.rfind("water(1,2,3),12;sprintf(foly_wtrsplash_sml%d.anm);lump(fx.lmp,foly_wtrsplash_sml1.anm);snd(1280,0,0.2);pert(1,2,3),700,24;", 0) == 0, "splash");
    CHECK(arena[569] == 1 && get_word(68) == 7u * 1664525u + 1013904223u, "splash state");
    reset(); frameNm = 9; waterResult = 1; msg_run__6PhProp(arena);
    CHECK(calls.find("water") == std::string::npos, "not on frame 9");
    reset(); frameNm = 8; waterResult = 1; arena[569] = 1; msg_run__6PhProp(arena);
    CHECK(calls.find("water") == std::string::npos, "not after the touched byte");
    reset(); frameNm = 8; waterResult = 0; msg_run__6PhProp(arena);
    CHECK(calls.find("water") == 0 && arena[569] == 0 && calls.find("sprintf") == std::string::npos, "no water");
    // collision hull: one corner below the threshold
    reset(); set_word(232, 1); set_real(552, 0.0f); set_real(424, 0.0f); set_real(416, 1.0f); set_real(420, 1.0f);
    hullBytes[0] = 2; hullBytes[1] = 3; hullBytes[2] = 1; for (int i = 3; i < 24; ++i) hullBytes[i] = 100;   // other corners are far above
    set_real(492, 0.0f); set_real(496, 0.0f); set_real(500, -1.0f);   // plane height = -p.z + 424 -> corner 0: -4 < 2.0
    velocityZ = -2.0f; set_real(572, 0.75f); set_word(76, 0x100);
    msg_run__6PhProp(arena);
    CHECK(calls.find("coll0;unt(2,3,1);vel(4,7,0.5);") != std::string::npos, "corner transform");
    CHECK(calls.find("imp(4,7,0.5)(0,0,1)0.75;") != std::string::npos && get_word(560) == 1, "impulse on a downward velocity");
    CHECK(calls.find("addf(4,7,0.5)(0,0,") != std::string::npos, "lift force");
    // a non-zero +564 counter eats the corner
    reset(); set_word(232, 1); set_word(564, 2); hullBytes[2] = 4; for (int i = 3; i < 24; ++i) hullBytes[i] = 100;
    set_real(500, -1.0f); set_word(76, 0x100);
    msg_run__6PhProp(arena);
    CHECK(calls.find("vel") == std::string::npos, "counter skips the corner");
    // impact sound by flags (byte 568 clear: halfword 320)
    struct Snd { unsigned char b568, e29, e28; unsigned long long flags; unsigned short h320; const char* name; } sounds[] = {
        {0, 0, 0, 0, 8, "inv_drop_mdwp_wd.anm"}, {0, 0, 0, 0, 0x10, "inv_drop_mdwp_mtl.anm"}, {0, 0, 0, 0, 0, nullptr},
        {1, 0, 0, 0x800000, 0, "inv_drop_mdwp_wd.anm"}, {1, 0, 0, 0x40000, 0, "inv_drop_mdwp_mtl.anm"}, {1, 0, 0, 0x1, 0, nullptr},
        {1, 1, 3, 0x800000, 0, "inv_drop_sm_shield.anm"}, {1, 1, 3, 0x40000, 0, "inv_drop_sm_shield.anm"}, {1, 1, 3, 0x200000, 0, nullptr},
        {1, 1, 4, 0x800000, 0, "inv_drop_mdwp_wd.anm"}, {1, 1, 4, 0x200000, 0, "inv_drop_chain.anm"}, {1, 1, 4, 0x40000, 0, "inv_drop_mwdp_mtl.anm"},
        {1, 1, 4, 0x100000, 0, "inv_drop_leather.anm"}, {1, 1, 4, 0x80000, 0, "inv_drop_cloth.anm"}, {1, 1, 4, 0x1000000, 0, "inv_drop_potion.anm"},
        {1, 1, 4, 0x4000000, 0, "inv_drop_jewelry.anm"}, {1, 1, 4, 0x2000000, 0, "inv_drop_jewelry.anm"}, {1, 1, 4, 0x10000000, 0, "inv_drop_jewelry.anm"},
        {1, 1, 4, 0x8, 0, nullptr}};
    for (auto& s : sounds) {
        reset(); set_word(232, 1); set_word(76, 0x100); hullBytes[2] = 1; for (int i = 3; i < 24; ++i) hullBytes[i] = 100;
        set_real(500, -1.0f); velocityZ = -2.0f; arena[568] = s.b568; set_word(328, 0x2000); arena[0x2000 + 29] = s.e29; arena[0x2000 + 28] = s.e28;
        std::memcpy(arena + 336, &s.flags, 8); set_half(320, static_cast<short>(s.h320));
        msg_run__6PhProp(arena);
        const std::string want = s.name ? std::string("lump(fx.lmp,") + s.name + ");snd(1280,0,1);" : std::string();
        CHECK(s.name ? calls.find(want) != std::string::npos : calls.find("lump(fx.lmp,inv_drop") == std::string::npos, "impact sound");
    }
    // no sound when the byte 569 is set or the velocity is slow
    reset(); set_word(232, 1); set_word(76, 0x100); hullBytes[2] = 1; for (int i = 3; i < 24; ++i) hullBytes[i] = 100; set_real(500, -1.0f);
    velocityZ = -0.5f; set_half(320, 8); msg_run__6PhProp(arena);
    CHECK(calls.find("lump(fx.lmp,inv_drop") == std::string::npos && calls.find("imp") != std::string::npos, "slow impact: impulse without sound");
    reset(); set_word(232, 1); set_word(76, 0x100); hullBytes[2] = 1; for (int i = 3; i < 24; ++i) hullBytes[i] = 100; set_real(500, -1.0f);
    velocityZ = -2.0f; set_half(320, 8); arena[569] = 1; msg_run__6PhProp(arena);
    CHECK(calls.find("lump(fx.lmp,inv_drop") == std::string::npos, "no sound after the touched byte");
    // damping when halfword 320 has bit 5
    reset(); set_half(320, 0x20); set_real(456, 1.0f); set_real(404, 1.0f); set_real(460, -1.0f); set_real(408, 4.0f); set_real(464, 0.01f); msg_run__6PhProp(arena);
    CHECK(get_real(456) < 0.0476f && get_real(456) > 0.0474f && get_real(460) > -0.01188f && get_real(460) < -0.01187f, "velocity limit");
    CHECK(get_real(464) > 0.0094f && get_real(464) < 0.0096f, "small velocity untouched except the 0.95 damping");
    // motion: standing height, touched scaling, copying the body position
    reset(); standHeight = 1.5f; set_real(416, 5.0f); set_real(420, 6.0f); set_real(424, 1.505f); set_real(452, 0.0f);
    msg_run__6PhProp(arena);
    CHECK(get_real(552) == 1.5f && get_real(24) == 1.5f, "stand height pulls the object down");
    CHECK(calls.find("stand(5,6,1.505),0.5;") != std::string::npos, "stand height query uses the copied position");
    reset(); standHeight = 1.5f; set_real(424, 2.0f); set_real(452, 0.0f); msg_run__6PhProp(arena);
    CHECK(get_real(552) == 1.5f && get_real(24) == 2.0f, "stand height leaves a higher object");
    // clip motion: fast sideways motion
    reset(); set_real(444, 1.0f); set_real(448, 0.0f); clipFraction = 0.5f; clipHit = Point3{-1.0f, 0.0f, 0.0f}; standHeight = -100.0f; set_real(400, 1.0f);
    msg_run__6PhProp(arena);
    CHECK(calls.find("clip(0,0,0)(1,0,0),0.5,1;") != std::string::npos, "clip call");
    CHECK(get_real(16) == 0.5f && get_real(20) == 0.0f, "position advances by the clipped motion");
    // death: life counter below -220 builds a copy of the right kind and deletes the original
    reset(); set_half(270, -221); allocCount = 1; arena[84] = 0; msg_run__6PhProp(arena);
    CHECK(calls.find("mscale0;mtrans-0,-0,-0;alloc328;goctor32768(") != std::string::npos && calls.rfind("del0;") == calls.size() - 5, "plain copy");
    CHECK(get_word(0x8000 + 12) == 0x5809u && get_half(0x8000 + 320) == 1 && get_real(0x8000 + 28) == 4.0f, "plain copy fields");
    reset(); set_half(270, -221); arena[568] = 1; msg_run__6PhProp(arena);
    CHECK(calls.find("alloc400;item32768(0,0,0),328,272;del0;") != std::string::npos, "item copy");
    reset(); set_half(270, -221); arena[84] = 'a'; arena[100] = 'b'; arena[164] = 'c'; msg_run__6PhProp(arena);
    CHECK(calls.find("alloc328;goctor32768(") != std::string::npos && calls.find("strcpy(32852,84);strcpy(32868,100);strcpy(32932,164);") != std::string::npos, "named copy");
    CHECK(calls.find("lump(\x01,") == std::string::npos, "lump names come from the new object");
    reset(); set_half(270, -221); allocCount = 0; msg_run__6PhProp(arena);
    CHECK(calls.find("goctor") == std::string::npos && calls.rfind("del0;") == calls.size() - 5, "allocation failure still deletes");
    // the end part also needs quiet motion when the life counter is in range
    reset(); set_half(270, 0); set_real(456, 0.5f); msg_run__6PhProp(arena);
    CHECK(calls.find("del") == std::string::npos && get_half(270) == -1, "life 0 with fast spin returns");
    reset(); set_half(270, 0); msg_run__6PhProp(arena);
    CHECK(calls.rfind("del0;") == calls.size() - 5 && get_half(270) == -1, "life 0 with quiet motion ends");
    reset(); set_half(270, 1); msg_run__6PhProp(arena);
    CHECK(calls.find("del") == std::string::npos && get_half(270) == 0, "positive life counts down");
    // fire state: byte 570 is 1 and the object sits on the ground
    reset(); arena[570] = 1; standHeight = 3.0f; set_word(68, 0); msg_run__6PhProp(arena);
    CHECK(arena[570] == 2 && calls.find("alloc1192;sfire32768(") != std::string::npos, "fire state builds a StaticFire");
    { unsigned s = 1013904223u; std::string want = std::to_string(480 + 60 * ((s >> 16) % 40)); CHECK(calls.find("," + want + ",0.5;") != std::string::npos, "fire duration from the generator"); }
    reset(); arena[570] = 1; standHeight = 30.0f; set_real(424, 100.0f); msg_run__6PhProp(arena);
    CHECK(arena[570] == 1 && calls.find("alloc1192") == std::string::npos, "fire state needs the height window");
    reset(); arena[570] = 1; standHeight = 3.0f; set_real(456, 0.9f); msg_run__6PhProp(arena);
    CHECK(arena[570] == 1, "fire state needs low spin");
    return 0;
}
