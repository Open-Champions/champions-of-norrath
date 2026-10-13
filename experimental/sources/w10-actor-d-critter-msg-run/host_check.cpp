// Host contract for msg_run__7Critter; not an EE input. Pointer words are offsets from a host base
// (W10_ACTOR_D_HOST); callees are logging stubs; the soft-double routines use host doubles.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
extern "C" { int frameNm; int nmPlayers; unsigned world; unsigned char player[4 * 23984]; }
#include "msg_run.cpp"
static std::string calls; static unsigned char* objectUnderTest;
static int lineResult, everyResult, activationResult, steerResult, routeResult, farResult, rollValue; static float groundHeight; static short lerpResult;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - objectUnderTest); }
static unsigned long long bitsOf(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
static double dbl(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
int gameLineUpdatePosition__FP17gameLinePathState(void* s) { calls += "line(" + off(s) + ");"; return lineResult; }
short lerpShortRot__Fssfff(short a, short b, float c, float d, float e) { calls += "lerp(" + std::to_string(a) + "," + std::to_string(b) + "," + f(c) + "," + f(d) + "," + f(e) + ");"; return lerpResult; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_d_host_base) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int n) { calls += "act(" + std::to_string(n) + ");"; return activationResult; }
void creatureAdvanceAnimation__8Creaturef(void*, float v) { calls += "adv(" + f(v) + ");"; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void move2__8CreatureG6Point3(void*, const Point3* p) { calls += "move2(" + f(p->x) + f(p->y) + f(p->z) + ");"; }
int steerToPosition__8CreatureG6Point3(void*, const Point3* p) { calls += "steer(" + f(p->x) + ");"; return steerResult; }
int steerAndRouteToPosition__8CreatureG6Point3(void*, const Point3* p) { calls += "route(" + f(p->x) + ");"; return routeResult; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int fl) {
    calls += "shot(" + off(s) + "," + std::to_string(static_cast<unsigned char*>(h) - w10_actor_d_host_base) + "," + f(a) + "," + f(b) + "," + std::to_string(fl) + ");";
}
int eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
int bFarFromHome__8Creature(void*) { calls += "far;"; return farResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
Point3* getSmartRandomDestination__8Creaturei(Point3* r, void*, int n) { calls += "dest(" + std::to_string(n) + ");"; *r = Point3{5.0f, 6.0f, 7.0f}; return r; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, const Point3* p, float v) { calls += "height(" + std::to_string(static_cast<unsigned char*>(w) - w10_actor_d_host_base) + "," + f(p->x) + "," + f(v) + ");"; return groundHeight; }
void flee2__8CreatureP10GameObject(void*, void* o) { calls += "flee(" + std::to_string(static_cast<unsigned char*>(o) - w10_actor_d_host_base) + ");"; }
void listDestroy__FP8ListHead(void* h) { calls += "ldestroy(" + off(h) + ");"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
unsigned long long fptodp(float v) { return bitsOf(static_cast<double>(v)); }
unsigned long long dpadd(unsigned long long a, unsigned long long b) { return bitsOf(dbl(a) + dbl(b)); }
unsigned long long dpsub(unsigned long long a, unsigned long long b) { calls += "dpsub;"; return bitsOf(dbl(a) - dbl(b)); }
unsigned long long dpmul(unsigned long long a, unsigned long long b) { return bitsOf(dbl(a) * dbl(b)); }
int dpcmp(unsigned long long a, unsigned long long b) { return dbl(a) < dbl(b) ? -1 : dbl(a) > dbl(b) ? 1 : 0; }
float dptofp(unsigned long long v) { return static_cast<float>(dbl(v)); }
static void setState(void* s, int v) { calls += "state(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(v) + ");"; }
static void slot192(void*, int) { calls += "slot192;"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
template <typename T> static T get(unsigned o) { T v; std::memcpy(&v, objectUnderTest + o, sizeof v); return v; }
template <typename T> static void put(unsigned o, T v) { std::memcpy(objectUnderTest + o, &v, sizeof v); }
int main() {
    alignas(16) static unsigned char arena[0x40000];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[1] = setState; w10_actor_d_host_functions[2] = slot192;
    unsigned char* object = arena + 0x10000; objectUnderTest = object;
    auto setup = [&](short state) {
        std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
        put<short>(572, state); unsigned table = 4096; put<unsigned>(72, table);
        short adj = 0; unsigned fn = 1; std::memcpy(arena + table + 184, &adj, 2); std::memcpy(arena + table + 188, &fn, 4);
        fn = 2; std::memcpy(arena + table + 192, &adj, 2); std::memcpy(arena + table + 196, &fn, 4);
        put<unsigned>(1036, 900u); put<unsigned>(1048, 7000u); put<unsigned>(1052, 0);
        put<int>(808, 100); put<int>(1028, 30); put<int>(1032, 50); put<int>(256, 200);
        put<float>(16, 1.0f); put<float>(20, 2.0f); put<float>(24, 3.0f); put<float>(28, 4.0f);
        calls.clear(); lineResult = 0; everyResult = 0; activationResult = 1; steerResult = 0; routeResult = 0; farResult = 0; rollValue = 0; groundHeight = 0; lerpResult = 0;
        frameNm = 0; nmPlayers = 0; world = 20000u;
    };
    const std::string head = "every(900);act(600);adv(1);";
    // gating
    setup(1); everyResult = 1; msg_run__7Critter(object); expect(calls == "every(900);", "everyFrame stops the run");
    setup(1); activationResult = 0; msg_run__7Critter(object); expect(calls == "every(900);act(600);", "activation check stops the run");
    for (short s : {2, 3, 4, 5, 6, 8, 9, 11, 17, 100, -1}) { setup(s); msg_run__7Critter(object); expect(calls == head, "states without a case"); }
    // 12: moving along a line
    setup(12); lineResult = 0; msg_run__7Critter(object); expect(calls == "line(960);adv(1);" && get<int>(236) == 1, "state 12, line not moving");
    setup(12); lineResult = 1; put<unsigned char>(977, 1); put<unsigned short>(1000, 777); put<short>(56, 5); put<float>(180, 0.5f); put<float>(184, -1.0f); put<float>(972, 6.0f);
    msg_run__7Critter(object);
    expect(get<unsigned short>(56) == 777 && calls == "line(960);adv(1);grid;", "state 12, byte +977 copies the heading");
    expect(get<float>(16) == 1.0f + 30.0f * 0x1.111110p-6f && get<float>(20) == 2.0f + -60.0f * 0x1.111110p-6f, "state 12 x and y");
    { const double speed = static_cast<double>(__builtin_sqrtf(60.0f * 60.0f + 30.0f * 30.0f)); const float want = static_cast<float>(3.0 + 6.0 * speed * 0x1.1111111111111p-6);
      expect(get<float>(24) == want && get<float>(60) == want, "state 12 z through soft doubles"); }
    setup(12); lineResult = 1; put<unsigned char>(977, 0); put<unsigned short>(1000, 40); put<short>(56, 10); put<float>(1016, 8.0f); lerpResult = 123; msg_run__7Critter(object);
    expect(calls.find("lerp(10,40,2,100,-1);") != std::string::npos && get<short>(56) == 123, "state 12 lerps the heading");
    setup(12); lineResult = 1; put<unsigned char>(977, 0); put<float>(972, -2.0f); put<float>(180, 0.0f); put<float>(184, 0.0f); msg_run__7Critter(object);
    expect(get<float>(24) == 3.0f, "state 12 with zero speed leaves z");
    // 0
    setup(0); put<unsigned>(1048, 0); put<int>(1056, 1); put<int>(256, 99); msg_run__7Critter(object);
    expect(calls == head + "move2(000);state(0,1);", "state 0 reroute without a header: setState(1) when +256 < +808");
    setup(0); put<unsigned>(1048, 0); put<int>(1056, 0); msg_run__7Critter(object); expect(calls == head + "move2(000);", "state 0, +1056 zero");
    setup(0); put<unsigned>(1048, 0); put<int>(1056, 1); farResult = 1; msg_run__7Critter(object); expect(calls == head + "move2(000);far;state(0,1);", "state 0, far from home");
    setup(0); put<unsigned>(1048, 0); put<int>(1056, 1); farResult = 0; msg_run__7Critter(object); expect(calls == head + "move2(000);far;", "state 0, near home");
    setup(0); put<unsigned>(1048, 0); put<int>(1056, 1); put<unsigned short>(294, 8); msg_run__7Critter(object); expect(calls == head + "move2(000);", "state 0, bit 8 of +294");
    setup(0); rollValue = 7; msg_run__7Critter(object);
    expect(calls == head + "move2(000);shot(288,7000,0.1,0.1,2056);rand;" && get<int>(1032) == 100 + 30 + 7, "state 0 one-shot");
    setup(0); put<unsigned>(1052, 8000u); rollValue = 4; msg_run__7Critter(object); expect(calls == head + "move2(000);rand;shot(288,8000,0.1,0.1,2056);rand;", "state 0 picks the second header on an even roll");
    setup(0); put<unsigned>(1052, 8000u); rollValue = 5; msg_run__7Critter(object); expect(calls.find("rand;shot(288,7000,") != std::string::npos, "odd roll keeps the first header");
    setup(0); put<int>(1032, 100); put<int>(1056, 1); put<int>(256, 500); msg_run__7Critter(object); expect(calls == head + "move2(000);far;", "state 0, 1032 not below 808");
    // 1
    setup(1); put<float>(272, 11.0f); routeResult = 1; put<int>(1044, 5); msg_run__7Critter(object); expect(calls == head + "route(11);state(0,0);", "state 1 reached, +1044 set");
    setup(1); put<float>(272, 11.0f); put<unsigned>(1076, 2); routeResult = 1; steerResult = 0; put<int>(256, 500); msg_run__7Critter(object); expect(calls == head + "steer(11);", "state 1 bit 2 uses steerToPosition and does nothing when not reached");
    setup(1); put<unsigned>(1076, 2); steerResult = 3; rollValue = 125; msg_run__7Critter(object);
    expect(calls == head + "steer(0);reset;dest(200);rand;" && get<float>(272) == 5.0f && get<int>(256) == 100 + 5 + 180, "state 1 reached, no +1044: new destination");
    setup(1); put<int>(256, 50); msg_run__7Critter(object); expect(calls.find("route(0);reset;") != std::string::npos, "state 1 timed out");
    setup(1); put<unsigned>(1076, 4); put<int>(256, 500); put<float>(776, 33.0f); put<float>(24, 100.0f); groundHeight = 88.5f; msg_run__7Critter(object);
    expect(calls == head + "route(0);height(20000,33,4);" && get<float>(272) == 33.0f, "state 1 ground check copies the point");
    setup(1); put<unsigned>(1076, 4); put<int>(256, 500); put<float>(776, 33.0f); put<float>(24, 100.0f); groundHeight = 89.0f; msg_run__7Critter(object);
    expect(get<float>(272) == 0.0f, "state 1 ground check at the boundary does not copy");
    // 7
    setup(7); put<int>(1044, 1); msg_run__7Critter(object); expect(calls == head + "state(0,0);", "state 7, empty list, +1044 set");
    setup(7); msg_run__7Critter(object); expect(calls == head + "state(0,1);", "state 7, empty list, +1044 clear");
    setup(7); { unsigned node = 9000; put<unsigned>(576, node); unsigned data = 9500; std::memcpy(arena + node + 12, &data, 4); } put<int>(256, 500); msg_run__7Critter(object);
    expect(calls == head + "flee(9500);", "state 7 flee, not timed out");
    setup(7); { unsigned node = 9000; put<unsigned>(576, node); unsigned data = 9500; std::memcpy(arena + node + 12, &data, 4); } put<int>(256, 50); put<int>(1044, 1); msg_run__7Critter(object);
    expect(calls == head + "flee(9500);ldestroy(576);state(0,0);" && get<int>(576) == 0, "state 7 flee, timed out");
    // 10
    setup(10); msg_run__7Critter(object); expect(calls == head + "rmrun;slot192;", "state 10");
    // 13 / 15 proximity
    auto placePlayer = [&](int i, float x, float y, float z) {
        unsigned objWord = 0x20000 + 0x100 * i; unsigned char* po = arena + objWord;
        std::memcpy(po + 16, &x, 4); std::memcpy(po + 20, &y, 4); std::memcpy(po + 24, &z, 4); std::memcpy(player + 21872 + 23984 * i, &objWord, 4);
    };
    setup(13); put<unsigned char>(569, 3); frameNm = 0x13; nmPlayers = 2; put<int>(1084, 5); put<float>(16, 10.0f); put<float>(20, 20.0f); put<float>(24, 30.0f);
    placePlayer(0, 100.0f, 0, 0); placePlayer(1, 13.0f, 24.0f, 30.0f); put<unsigned>(1048, 0); put<int>(236, 1); msg_run__7Critter(object);
    expect(calls == head, "state 13: second player at exactly 5.0 is not within (strict)");
    setup(13); put<unsigned char>(569, 3); frameNm = 0x13; nmPlayers = 2; put<int>(1084, 5); put<float>(16, 10.0f); put<float>(20, 20.0f); put<float>(24, 30.0f);
    placePlayer(0, 100.0f, 0, 0); placePlayer(1, 13.0f, 24.0f, 30.5f); put<unsigned>(1048, 0); put<int>(236, 1); msg_run__7Critter(object);
    expect(calls == head, "state 13: just outside");
    setup(13); put<unsigned char>(569, 3); frameNm = 0x14; nmPlayers = 1; put<int>(1084, 500); placePlayer(0, 10.0f, 20.0f, 30.0f); put<unsigned>(1048, 0); put<int>(236, 1); msg_run__7Critter(object);
    expect(calls == head, "state 13: the frame test gates the search");
    setup(13); put<unsigned char>(569, 3); frameNm = 0x13; nmPlayers = 1; put<int>(1084, 5); put<float>(16, 10.0f); put<float>(20, 20.0f); put<float>(24, 30.0f); placePlayer(0, 11.0f, 20.0f, 30.0f); put<unsigned>(1048, 0); put<int>(236, 1); msg_run__7Critter(object);
    expect(calls == head + "state(0,14);", "state 13: inside");
    setup(13); put<int>(236, 0); put<int>(256, 50); put<unsigned>(1048, 0); put<unsigned char>(569, 1); frameNm = 2; msg_run__7Critter(object); expect(calls == head + "state(0,14);", "state 13 timed out");
    setup(13); put<int>(236, 0); put<int>(256, 500); put<unsigned>(1048, 0); put<unsigned char>(569, 1); frameNm = 2; msg_run__7Critter(object); expect(calls == head, "state 13 not timed out");
    setup(13); put<int>(236, 1); put<int>(256, 50); put<unsigned>(1048, 0); put<unsigned char>(569, 1); frameNm = 2; msg_run__7Critter(object); expect(calls == head, "state 13 with +236 set does nothing");
    setup(13); put<unsigned char>(569, 1); frameNm = 2; rollValue = 9; put<int>(1032, 10); msg_run__7Critter(object);
    expect(calls == head + "shot(288,7000,0.1,0.1,8);rand;" && get<int>(1032) == 100 + 30 + 9, "state 13 one-shot with flags 8");
    setup(15); put<unsigned>(1048, 0); put<unsigned>(12, 2); put<int>(236, 0); msg_run__7Critter(object);
    expect(calls == head && get<unsigned>(12) == 0, "state 15: bit 2 cleared when +236 is zero, no search");
    setup(15); put<unsigned>(1048, 0); put<unsigned>(12, 2); put<int>(236, 1); msg_run__7Critter(object);
    expect(calls == head && get<unsigned>(12) == 2, "state 15: bit 2 kept when +236 is set");
    setup(15); put<unsigned>(1048, 0); put<unsigned char>(569, 3); frameNm = 0x13; nmPlayers = 1; put<int>(1084, 5); placePlayer(0, 3.0f, 2.0f, 3.0f); msg_run__7Critter(object);
    expect(calls == head + "state(0,16);", "state 15: player close, state 16");
    setup(15); put<unsigned char>(569, 3); frameNm = 0x13; nmPlayers = 0; put<int>(1032, 60); msg_run__7Critter(object);
    expect(calls == head + "shot(288,7000,0.1,0.1,8);rand;", "state 15 one-shot");
    // 14 / 16
    setup(14); put<int>(236, 0); msg_run__7Critter(object); expect(calls == head + "del;", "state 14 deletes when +236 is clear");
    setup(14); put<int>(236, 1); put<unsigned>(1076, 1); put<float>(1080, 1.0f); put<float>(180, 0.5f); put<float>(184, -0.5f); msg_run__7Critter(object);
    expect(calls == head + "grid;" && get<float>(1080) == 1.0f + 0x1.fffffep+0f && get<float>(60) == 3.0f + get<float>(1080) && get<float>(16) == 1.5f && get<float>(20) == 1.5f, "state 14 drift");
    setup(14); put<int>(236, 1); put<float>(1080, 1.0f); msg_run__7Critter(object); expect(get<float>(1080) == 1.0f && get<float>(60) == 4.0f, "state 14 without bit 1");
    setup(16); put<int>(236, 0); put<float>(776, 8.0f); put<float>(780, 9.0f); put<float>(784, 10.0f); msg_run__7Critter(object);
    expect(calls == head + "grid;state(0,15);" && get<unsigned>(12) == 2 && get<float>(16) == 8.0f && get<float>(24) == 10.0f, "state 16 snaps back");
    setup(16); put<int>(236, 1); put<unsigned>(1076, 1); msg_run__7Critter(object); expect(calls == head + "grid;" && get<float>(1080) == 0x1.fffffep+0f, "state 16 drift");
    return failures ? 1 : 0;
}
