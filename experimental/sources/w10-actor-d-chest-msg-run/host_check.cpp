// Host contract for msg_run__5Chest; not an EE input. Pointer words are offsets from a host base
// (W10_ACTOR_D_HOST). Every callee is a logging stub; eRand returns a scripted sequence.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
extern "C" { int frameNm; float worldDifficulty; unsigned char norrath_w10_actor_d_view_7d7720[80], norrath_w10_actor_d_view_7d7770[80]; }
#include "msg_run.cpp"
static std::string calls; static unsigned char* objectUnderTest; static bool allocFails;
static int rolls[64], rollIndex; static int rollCount;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string p(const Point3* q) { return "(" + f(q->x) + "," + f(q->y) + "," + f(q->z) + ")"; }
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* s, Point3* pos, void* a, short* b) {
    calls += "anim(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(reinterpret_cast<unsigned char*>(pos) - objectUnderTest) + "," + std::to_string(a == nullptr && b == nullptr) + ");";
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const Point3* pos, short type, void* st, int a, Point3* out, Point3* second, float scale) {
    calls += "attach(" + std::to_string(static_cast<unsigned char*>(model) - w10_actor_d_host_base) + "," + p(pos) + "," + std::to_string(type) + "," + std::to_string(st == nullptr) + "," + std::to_string(a) + "," + std::to_string(second == nullptr) + "," + f(scale) + ");";
    *out = Point3{1.0f, 2.0f, 3.0f};
}
int eRand__10GameObject(void*) { return rolls[rollIndex++ % rollCount]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* pos, Point3* vel, float v) {
    calls += std::string("particle(") + (d == norrath_w10_actor_d_view_7d7720 ? "fuse" : d == norrath_w10_actor_d_view_7d7770 ? "smoke" : "?") + "," + p(pos) + "," + p(vel) + "," + f(v) + ");";
}
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return w10_actor_d_host_base + 7; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls += "seq(" + std::to_string(static_cast<unsigned char*>(h) - w10_actor_d_host_base) + "," + std::to_string(o == objectUnderTest) + "," + f(v) + ");";
}
void makeExplosion__FG6Point3f(const Point3* q, float s) { calls += "expl(" + p(q) + "," + f(s) + ");"; }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* o, const Point3* q, int a, float b, float c, int d, int e, int g) {
    calls += "rad(" + std::to_string(o == objectUnderTest) + "," + p(q) + "," + std::to_string(a) + "," + f(b) + "," + f(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(g) + ");";
}
void floorAdd__FG6Point3sfii(const Point3* q, short a, float b, int c, int d) {
    calls += "floor(" + p(q) + "," + std::to_string(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");";
}
static unsigned char lightBlock[188];
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; return allocFails ? nullptr : lightBlock; }
void lightEffect_construct(void* s, const Point3* q, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, void* o, int fl) {
    calls += "light(" + std::to_string(s == lightBlock) + "," + p(q) + "," + f(f0) + "," + f(f1) + "," + f(f2) + "," + f(f3) + "," + f(f4) + "," + f(f5) + "," + f(f6) + "," + f(f7) + "," + f(f8) + "," + f(f9) + "," + f(f10) + "," + f(f11) + "," + std::to_string(o == nullptr) + "," + std::to_string(fl) + ");";
}
void objectDoMyActions__FP10GameObjecti(void* o, int v) { calls += "act(" + std::to_string(o == objectUnderTest) + "," + std::to_string(v) + ");"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rm;"; }
static void destructor(void* s, int flags) { calls += "dtor(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(flags) + ");"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
int main() {
    alignas(16) static unsigned char arena[8192];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[3] = destructor;
    unsigned char* object = arena + 2048; objectUnderTest = object;
    auto setup = [&](signed char opened, unsigned short half98, int frame, int stamp, float difficulty) {
        std::memset(arena, 0, sizeof arena);
        unsigned m = 300, model = 900, table = 4096; std::memcpy(object + 36, &m, 4); std::memcpy(arena + m + 12, &model, 4);
        std::memcpy(object + 72, &table, 4); short adj = 4; unsigned fn = 3; std::memcpy(arena + table + 8, &adj, 2); std::memcpy(arena + table + 12, &fn, 4);
        float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
        short type = 9; std::memcpy(object + 56, &type, 2);
        object[88] = static_cast<unsigned char>(opened); std::memcpy(object + 98, &half98, 2);
        std::memcpy(object + 80, &stamp, 4); frameNm = frame; worldDifficulty = difficulty; calls.clear(); allocFails = false; rollIndex = 0;
        rollCount = 6; int r[6] = {32768, 32768, 49152, 32768, 32768, 16384}; std::memcpy(rolls, r, sizeof r);
    };
    // closed chest: flag bit 0x80 of the halfword at +98
    setup(0, 0x80, 0, 0, 0);
    msg_run__5Chest(object);
    expect(calls == "anim(92,16,1);act(1,1);rm;", "closed and flagged");
    { int w132; std::memcpy(&w132, object + 132, 4); expect(w132 == 2, "state word set to 2"); }
    setup(0, 0x7f, 0, 0, 0); msg_run__5Chest(object);
    expect(calls == "anim(92,16,1);", "closed, flag clear");
    setup(0, 0xff7f, 0, 0, 0); msg_run__5Chest(object);
    expect(calls == "anim(92,16,1);", "closed, only low seven bits set");
    // opened chest, not yet 90 frames old: only the two particles. First ball (0, 0, 0.5), second (0, 0, -0.5) made non-negative
    setup(1, 0, 100, 10, 0); msg_run__5Chest(object);
    expect(calls == "anim(92,16,1);attach(900,(10,20,30),9,1,0,1,1);particle(fuse,(1,2,3),(0,0,12.5),0);particle(smoke,(1,2,3),(0,0,15.8208),0);", "opened, young");
    // exactly 90 frames: not greater
    setup(1, 0, 100, 10, 0); msg_run__5Chest(object);
    expect(calls.find("seq(") == std::string::npos, "90 frames does not explode");
    setup(-1, 0, 101, 10, 3.5f); msg_run__5Chest(object);
    expect(calls.find("lump(fx.lmp,spel_we_explarrow_hit.anm);seq(7,1,1);expl((10,20,44.7),2);rad(1,(10,20,30),21,40,180,264192,0,0);") != std::string::npos, "explosion part one");
    expect(calls.find("floor((10,20,31),-32768,48,9,0);alloc(188);light(1,(10,20,30),128,100,78,-32,0,0,0,0,168,60,1,0.35,1,0);dtor(4,3);") != std::string::npos, "explosion part two");
    setup(1, 0, 101, 10, 3.5f); allocFails = true; msg_run__5Chest(object);
    expect(calls.find("alloc(188);dtor(4,3);") != std::string::npos && calls.find("light(") == std::string::npos, "failed light allocation still deletes");
    return failures ? 1 : 0;
}
