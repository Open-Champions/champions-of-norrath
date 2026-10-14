// Host contract for setState__10CloudGianti; not an EE input. Pointer words are offsets from a host
// base (W10_ACTOR_D_HOST); the callees are logging stubs.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
extern "C" { char bossMeterName[128]; int bossMeterCurrent, bossMeterMax; }
#include "set_state.cpp"
static std::string calls; static unsigned char* objectUnderTest; static int processingResult, processingRewrite, nextRoll, allocFails;
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - w10_actor_d_host_base); }
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
void soundStopSound__Fi(int v) { calls += "stop(" + std::to_string(v) + ");"; }
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc(" + std::to_string(*s) + ");"; if (processingRewrite >= 0) *s = processingRewrite; return processingResult; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* st, void* h, float r, int fl) {
    calls += "trans(" + std::to_string(static_cast<unsigned char*>(st) - objectUnderTest) + "," + off(h) + "," + f(r) + "," + std::to_string(fl) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int fl) {
    calls += "shot(" + off(h) + "," + f(a) + "," + f(b) + "," + std::to_string(fl) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
Point3* getSmartRandomDestination__8Creaturei(Point3* r, void*, int n) { calls += "dest(" + std::to_string(n) + ");"; *r = Point3{7.0f, 8.0f, 9.0f}; return r; }
int eRand__10GameObject(void*) { calls += "rand;"; return nextRoll; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, const Point3* p, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* n) {
    calls += "drop(" + f(p->x) + "," + std::to_string(a) + "," + f(f0) + "," + f(f1) + "," + f(f2) + "," + f(f3) + "," + f(f4) + "," + f(f5) + "," + std::to_string(n == nullptr) + ");";
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, const Point3*, short t, void*, int a, Point3* out, Point3* second, float s) {
    calls += "attach(" + off(m) + "," + std::to_string(t) + "," + std::to_string(a) + "," + std::to_string(second != nullptr) + "," + f(s) + ");"; out->x = 55.0f;
}
static unsigned char propBlock[600];
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; return allocFails ? nullptr : propBlock; }
void phProp_construct(void* s, const Point3* p, char* a, char* b, char* c, int d, int e, void* desc, float fl) {
    calls += "prop(" + std::to_string(s == propBlock) + "," + f(p->x) + "," + off(a) + "," + off(b) + "," + off(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(desc == nullptr) + "," + f(fl) + ");";
}
static void death(void* s, int) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + ");"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
template <typename T> static T get(unsigned o) { T v; std::memcpy(&v, objectUnderTest + o, sizeof v); return v; }
template <typename T> static void put(unsigned o, T v) { std::memcpy(objectUnderTest + o, &v, sizeof v); }
int main() {
    alignas(16) static unsigned char arena[8192];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[2] = death;
    unsigned char* object = arena + 2048; objectUnderTest = object;
    const unsigned m = 300;
    auto setup = [&](short current, int w976, int w980, int w984, unsigned flags) {
        std::memset(arena, 0, sizeof arena); put<unsigned>(36, m);
        for (unsigned i = 0; i < 64; ++i) put<unsigned>(0, 0), std::memcpy(arena + m + i * 4, &(const unsigned&)(i * 4 + 1000), 4);  // block word at offset k is 1000 + k
        unsigned table = 4096; put<unsigned>(72, table); short adj = 8; unsigned fn = 2; std::memcpy(arena + table + 192, &adj, 2); std::memcpy(arena + table + 196, &fn, 4);
        put<short>(572, current); put<int>(976, w976); put<int>(980, w980); put<int>(984, w984); put<unsigned>(12, flags);
        put<int>(808, 1000); put<int>(1308, 77); put<float>(16, 1.0f); put<short>(56, 5); put<float>(224, 3.5f);
        put<float>(564, 90.9f); put<float>(560, 40.5f);
        calls.clear(); processingResult = 0; processingRewrite = -1; nextRoll = 0; allocFails = 0;
        std::memset(bossMeterName, 'z', sizeof bossMeterName); bossMeterCurrent = bossMeterMax = -5;
    };
    auto run = [&](int s) { calls.clear(); setState__10CloudGianti(object, s); };
    setup(0, 0, 0, 0, 0); nextRoll = 250;
    run(0);
    expect(calls == "stop(77);proc(0);rand;trans(288,1132,0.3,3);" && get<int>(256) == 1000 + 10 && get<unsigned short>(572) == 0 && get<int>(1308) == -1, "state 0");
    setup(0, 0, 0, 0, 0); processingResult = 1; processingRewrite = 5; put<short>(572, 9);
    run(3);
    expect(calls == "stop(77);proc(3);" && get<short>(572) == 9, "processing result nonzero returns before the state is stored");
    setup(0, 0, 0, 0, 0); processingRewrite = 5; run(1);
    expect(calls == "stop(77);proc(1);trans(288,1140,0.1,3);" && get<short>(572) == 5, "rewritten state selects the case and is stored");
    setup(0, 0, 1, 0, 0); nextRoll = 241; run(1);
    expect(calls == "stop(77);proc(1);reset;dest(200);rand;trans(288,1076,0.3,2051);sync;" && get<int>(256) == 1000 + 1 + 300 && get<float>(272) == 7.0f && get<float>(280) == 9.0f, "state 1 with +980 set");
    setup(0, 0, 0, 0, 0); run(1);
    expect(calls.find("trans(288,1172,0.3,2051);sync;") != std::string::npos, "state 1 with +980 clear");
    setup(4, 0, 0, 0, 0); run(3);
    expect(calls == "stop(77);proc(3);reset;trans(288,1172,0.1,2051);sync;" && bossMeterName[0] == 'z', "state 3 from another state, +976 clear");
    for (short cur : {3, 6, 14, 15}) { setup(cur, 0, 1, 0, 0); run(3); expect(calls == "stop(77);proc(3);reset;", "state 3 skips the transition from 3, 6, 14, 15"); }
    setup(15, 1, 0, 0, 0); run(3);
    expect(calls == "stop(77);proc(3);reset;" && std::strcmp(bossMeterName, "CloudGiantKing") == 0 && bossMeterMax == 90 && bossMeterCurrent == 40, "state 3 with +976 set copies the name and fills the meter");
    setup(0, 0, 0, 0, 0); run(5); expect(calls == "stop(77);proc(5);trans(288,1140,0.1,3);", "state 5");
    setup(0, 0, 0, 0, 0); nextRoll = 3; run(6); expect(calls == "stop(77);proc(6);rand;shot(1108,0.1,0.1,2056);", "state 6 odd roll");
    setup(0, 0, 0, 0, 0); nextRoll = 4; run(6); expect(calls == "stop(77);proc(6);rand;shot(1100,0.1,0.1,2056);", "state 6 even roll");
    setup(0, 0, 0, 0, 0); nextRoll = 3; run(8); expect(calls == "stop(77);proc(8);rand;shot(1124,0.1,0.1,16);", "state 8");
    setup(0, 0, 0, 0, 0); nextRoll = 4; run(10);
    expect(calls == "stop(77);proc(10);rand;trans(288,1084,0.3,6);drop(1,5,36,2,2.4,0.6,4,0.75,1);slot(8);", "state 10, bit 2 clear, +984 clear");
    expect(get<int>(980) == 0 && get<int>(1304) == 0 && get<unsigned>(12) == 0, "state 10 fields");
    setup(0, 0, 0, 1, 7); run(10);
    expect(calls.find("drop(") != std::string::npos && calls.find("attach") == std::string::npos && get<unsigned>(12) == 7 && get<int>(984) == 1 && calls.find("slot(8);") != std::string::npos, "state 10 with bit 2 set skips the attachment");
    setup(0, 0, 0, 1, 5); run(10);
    expect(calls.find("attach(1012,-16379,0,1,1);alloc(584);prop(1,55,1000,1192,1200,1,16385,1,3.5);slot(8);") != std::string::npos && get<unsigned>(12) == 4 && get<int>(984) == 0, "state 10 builds the attachment");
    setup(0, 0, 0, 1, 5); allocFails = 1; run(10);
    expect(calls.find("alloc(584);slot(8);") != std::string::npos && get<int>(984) == 0, "state 10 with a failed allocation");
    setup(0, 0, 0, 0, 0); nextRoll = 47; run(13);
    expect(calls == "stop(77);proc(13);trans(288,1044,0.1,3);shot(1036,0.1,0.1,8);rand;" && get<int>(968) == 1240 && get<int>(256) == 7 && objectUnderTest[964] == 0, "state 13");
    setup(0, 0, 0, 0, 0); run(14); expect(calls.find("shot(1156,0.1,0.1,2056);") != std::string::npos, "state 14");
    setup(0, 0, 1, 0, 0); run(15); expect(calls.find("shot(1060,") != std::string::npos, "state 15 with +980");
    setup(0, 0, 0, 0, 0); run(15); expect(calls.find("shot(1164,") != std::string::npos, "state 15 without");
    setup(0, 0, 0, 0, 0); run(16); expect(calls.find("shot(1148,") != std::string::npos, "state 16");
    setup(0, 0, 0, 0, 0); run(17); expect(calls.find("shot(1068,") != std::string::npos, "state 17");
    setup(0, 0, 0, 0, 0); run(18); expect(calls.find("shot(1052,") != std::string::npos, "state 18");
    for (int s : {2, 4, 7, 9, 11, 12, 19, 40, 65537}) { setup(0, 0, 0, 0, 0); run(s); expect(calls == "stop(77);proc(" + std::to_string(s) + ");" && get<unsigned short>(572) == static_cast<unsigned short>(s), "states without a case only store the state"); }
    return failures ? 1 : 0;
}
