// Host contract for doJumpEffect__Fi. Not an EE plan input.
// Build: c++ -std=c++17 -w host_check.cpp
// Object addresses stored in memory are offsets into one host arena; the callees record calls.
#define NORRATH_HOST_ARENA
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "do_jump_effect.cpp"

static unsigned char arena[1 << 20];
extern "C" unsigned char* norrath_host_base = arena;
static std::string ev;
static std::string num(long v) { return std::to_string(v); }
static unsigned W(const void* p) { return (unsigned)((const unsigned char*)p - arena); }
struct Particle { Point3 position, velocity; };
static std::vector<Particle> particles;
static int toggleValues[2], toggleCalls, points = 3, yawValue;
static float zoomValue;
static unsigned char* shockModel;
static Point3 shadowOut(int index) { return Point3{(float)index, 2.0f * index, 3.0f * index}; }

extern "C" {
signed char hudNoDraw, jumpHidden;
float jumpGateTime;
int jumpGateHurtDest, nmPlayersAlive, eRandState, jumping, frameNm;
unsigned gameCam;
int playersAlive[4];
alignas(8) unsigned char player[23984 * 3];
Point3 jumpGatePos;
unsigned char jumpParticleJump[16];

void hudLetterBox__Fv() { ev += "box;"; }
int GetToggle__6Camera(void*) { return toggleValues[toggleCalls++ < 1 ? 0 : 1]; }
void engineSetBlur__Fi(int a) { ev += "blur" + num(a) + ";"; }
int GetYaw__6Camera(void*) { return yawValue; }
void SetYaw__6Camerai(void*, int yaw) { ev += "yaw" + num(yaw) + ";"; }
float GetZoomDistance__6Camera(void*) { return zoomValue; }
void SetZoomDistance__6Cameraf(void*, float d) { ev += "zoom" + num((long)(d * 1000)) + ";"; }
void SetNoTargetUpdates__6Camerai(void*, int f) { ev += "notarget" + num(f) + ";"; }
void P_StartLightning__FG6Point3T0iiffUi(Point3* from, Point3* to, int a, int b, float c, float d, unsigned color) {
    ev += "bolt(" + num((long)from->x) + "," + num((long)from->y) + "," + num((long)from->z) + ")(" + num((long)to->x) + ")" + num(a) + "," + num(b) + "," + num((long)(c * 10)) + "," + num((long)(d * 10)) + ",0x" + [&] { char t[16]; std::snprintf(t, sizeof t, "%x", color); return std::string(t); }() + ";";
}
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(void* model, void* anim, Point3* position, short yaw, void* fx, float a, float b, float c, unsigned color) {
    ev += "shock(" + num(W(model)) + "," + num(W(anim)) + "," + num((long)position->x) + "," + num(yaw) + "," + num(W(fx)) + "," + num((long)(a * 10)) + "," + num((long)b) + "," + num((long)c) + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* p, Point3* v, float) { particles.push_back({*p, *v}); }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return points; }
int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, Point3*, short, int index, Point3* out, float*, void*, float) {
    ev += "shadow" + num(index) + ";";
    *out = shadowOut(index);
    return 0;
}
}
extern "C" unsigned char* norrath_host_slot_call_pointer(unsigned function, unsigned char* self) {
    ev += "vcall(" + num(function) + "," + num(W(self)) + ");";
    return shockModel;
}

static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static float getf(unsigned at) { float v; std::memcpy(&v, arena + at, 4); return v; }
static short get16(unsigned at) { short v; std::memcpy(&v, arena + at, 2); return v; }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static const unsigned OBJ = 0x2000;
static int fail(int n, const char* m) { std::printf("fail %d: %s\n  events: %s\n  particles: %zu\n", n, m, ev.c_str(), particles.size()); return n; }
#define EXPECT(n, cond) do { if (!(cond)) return fail(n, #cond); } while (0)

static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }

static void world() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
    ev.clear(); particles.clear();
    hudNoDraw = 0; jumpHidden = 0; jumpGateTime = 2.0f; jumpGateHurtDest = 0; nmPlayersAlive = 1; playersAlive[0] = 1;
    eRandState = 1; jumping = 0; frameNm = 777; gameCam = 0x100;
    toggleValues[0] = toggleValues[1] = 0; toggleCalls = 0; yawValue = 100; zoomValue = 50.0f; points = 3;
    jumpGatePos = Point3{100.0f, 0.0f, 0.0f};
    for (int p = 0; p < 3; p++) {
        unsigned at = OBJ + 0x1000 * p;
        put32(at - 0 + 0, 0);
        unsigned word = at;
        std::memcpy(player + 23984 * p + 21872, &word, 4);
        putf(at + 16, 10.0f * (p + 1)); putf(at + 20, 0.0f); putf(at + 24, 0.0f);
        put32(at + 72, 0x800); put32(at + 1332, 0x900 + p);
        put32(at + 224, 0); put32(at + 228, 0);
    }
    // virtual entry at +128 of the table at 0x800: adjust 8, function word 4660
    arena[0x800 + 128] = 8; put32(0x800 + 132, 4660);
    shockModel = arena + 0x9999;
}

int main() {
    // Setup part
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 0; jumpGateTime = 0.5f; yawValue = 100; zoomValue = 50.0f;
    toggleValues[0] = 0;
    doJumpEffect__Fi(2);
    // blur = (int)(0.5*24+80) = 92; yaw += (int)(0.5*186*2) = 186; zoom = 50 - (int)(0.5*2*(0.25*1.654)) = 50 - 0
    EXPECT(1, hudNoDraw == 1);
    EXPECT(2, ev == "box;blur92;yaw286;zoom50000;notarget5;");
    // toggle nonzero and not 2: factor 1.0
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 0; jumpGateTime = 1.0f; toggleValues[0] = 1; toggleValues[1] = 1;
    doJumpEffect__Fi(3);
    EXPECT(3, ev == "box;blur104;yaw658;zoom46000;notarget5;");   // yaw 100+(int)(558); zoom 50-(int)(3*1.653974)=50-4
    // toggle 2: the factor stays 1.2 (0x3f999999)
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 0; jumpGateTime = 1.0f; toggleValues[0] = 1; toggleValues[1] = 2;
    doJumpEffect__Fi(3);
    EXPECT(4, ev == "box;blur104;yaw658;zoom45000;notarget5;");   // 3*1.2*1.653974 = 5.95 -> 5
    // hurt-dest part: choose a state whose successor has (state >> 16) % 10 == 0
    unsigned s = 1;
    while (((lcg(s) >> 16) % 10u) != 0u) s++;
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 1; playersAlive[0] = 1; eRandState = (int)s; jumpGateTime = 0.0f;
    doJumpEffect__Fi(0);
    EXPECT(5, (unsigned)eRandState == lcg(s));
    EXPECT(6, ev.find("bolt(" + num(20) + ",0,50)(100)492,4,77,43,0x5e808080;vcall(4660," + num(OBJ + 0x1000 + 8) + ");shock(" + num(0x9999) + "," + num(OBJ + 0x1000 + 288) + ",20,0," + num(OBJ + 0x1000 + 824) + ",15,0,0);") != std::string::npos);
    EXPECT(7, get16(OBJ + 0x1000 + 154) == 64 && get16(OBJ + 0x1000 + 148) == 10 && get16(OBJ + 0x1000 + 140) == 10 && get16(OBJ + 0x1000 + 150) == 10);
    EXPECT(8, get16(OBJ + 0x1000 + 142) == 10 && get16(OBJ + 0x1000 + 152) == 128 && get16(OBJ + 0x1000 + 144) == 128 && get16(OBJ + 0x1000 + 146) == 64);
    // hidden: the shorts are still set but no effect is started
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 1; playersAlive[0] = 1; eRandState = (int)s; jumpHidden = 1; jumpGateTime = 0.0f;
    doJumpEffect__Fi(0);
    EXPECT(9, ev.find("bolt") == std::string::npos && get16(OBJ + 0x1000 + 154) == 64);
    // not chosen: nothing is changed on the object
    unsigned t = 1;
    while (((lcg(t) >> 16) % 10u) == 0u) t++;
    world(); jumpGateHurtDest = 1; nmPlayersAlive = 1; playersAlive[0] = 1; eRandState = (int)t; jumpGateTime = 0.0f;
    doJumpEffect__Fi(0);
    EXPECT(10, ev.find("bolt") == std::string::npos && get16(OBJ + 0x1000 + 154) == 0 && (unsigned)eRandState == lcg(t));
    // no hurt destination: jumping == 30 marks the objects, then particles
    world(); jumpGateHurtDest = 0; jumping = 30; nmPlayersAlive = 2; playersAlive[0] = 0; playersAlive[1] = 2; jumpGateTime = 0.0f;
    doJumpEffect__Fi(0);
    EXPECT(11, get32(OBJ + 252) == 777 && get32(OBJ + 936) == 120 && get32(OBJ + 0x2000 + 252) == 777 && get32(OBJ + 0x2000 + 936) == 120);
    EXPECT(12, get32(OBJ + 0x1000 + 252) == 0);
    EXPECT(13, particles.size() == 100);   // 50 per player, no shadow particles (count 0)
    // first particle of player 0: four generator steps from the seed
    {
        unsigned s1 = lcg(1), s2 = lcg(s1), s3 = lcg(s2), s4 = lcg(s3);
        const float unit = 1.52587890625e-05f;
        float f = (float)(s1 >> 16) * unit;
        Point3 start{10.0f + 0.0f, 0.0f, 0.0f + 50.0f};
        Point3 spot{start.x + (100.0f - start.x) * f, start.y + (0.0f - start.y) * f, start.z + (0.0f - start.z) * f};
        EXPECT(14, particles[0].position.x == spot.x && particles[0].position.y == spot.y && particles[0].position.z == spot.z);
        EXPECT(15, particles[0].velocity.x == ((float)(s2 >> 16) * unit - 0.5f) * 5.0f);
        EXPECT(16, particles[0].velocity.y == ((float)(s3 >> 16) * unit - 0.5f) * 5.0f);
        EXPECT(17, particles[0].velocity.z == ((float)(s4 >> 16) * unit - 0.5f) * 5.0f);
        EXPECT(18, (unsigned)eRandState != 1u);
    }
    world(); jumpGateHurtDest = 0; jumping = 29; jumpGateTime = 0.0f; doJumpEffect__Fi(0);
    EXPECT(19, get32(OBJ + 0x1000 + 252) == 0 && particles.size() == 50);
    // shadow particles: count = (int)(time * 35)
    world(); jumpGateHurtDest = 0; jumping = 0; jumpGateTime = 0.1f; points = 5; doJumpEffect__Fi(0);   // (int)(3.5) = 3
    EXPECT(20, particles.size() == 50 + 3);
    {
        unsigned seed = 1;
        for (int i = 0; i < 50; i++) seed = lcg(lcg(lcg(lcg(seed))));
        unsigned d = lcg(seed);
        int index = (int)(d >> 16) % 5;
        EXPECT(21, ev.find("shadow" + num(index) + ";") != std::string::npos);
        Point3 out = shadowOut(index);
        EXPECT(22, particles[50].position.x == out.x && particles[50].position.y == out.y && particles[50].position.z == out.z);
        unsigned a1 = lcg(d), a2 = lcg(a1), a3 = lcg(a2);
        const float unit = 1.52587890625e-05f;
        EXPECT(23, particles[50].velocity.x == ((float)(a1 >> 16) * unit - 0.5f) * 5.0f && particles[50].velocity.z == ((float)(a3 >> 16) * unit - 0.5f) * 5.0f);
        (void)a2;
    }
    return 0;
}
