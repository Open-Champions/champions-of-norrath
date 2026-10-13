// Host contract for msg_run__5Gnome; not an EE input. Words are offsets from a host base
// (W10_ACTOR_G_HOST); the callee sequence and arguments are checked per state.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
#include "msg_run.cpp"
static std::string calls;
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
static void v168(void* o) { calls += "v168(" + num(off(o) - 0x1000) + ");"; }
static void v176(void* o, int a, Point3 p) { (void)o; calls += "v176(" + num(a) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + ");"; }
static void v184(void* o, int a) { calls += "v184(" + num(off(o) - 0x1000) + "," + num(a) + ");"; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(v168) : w == 2 ? reinterpret_cast<void*>(v176) : reinterpret_cast<void*>(v184); }
static int everyResult, activationResult, advanceResult, exitResult;
static int zoneResults[4], zoneIndex;
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + num(off(m)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + num(off(m)) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; return activationResult; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceResult; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int enemyInZone__8Creaturefs(void*, float f, short s) { calls += "zone(" + num(f) + "," + num(s) + ");"; return zoneResults[zoneIndex++ & 3]; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitResult; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* p) {
    calls += "melee(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + (p ? "desc" : "null") + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
static int bad;
static void w32(int o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void wa(int o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void h16(int o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static unsigned r32(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10g_host_base = arena; calls.clear();
    w32(36, 0x200); wa(0x200 + 12, 0x777); w32(72, 0x800);
    for (int e : {168, 176, 184}) { unsigned fn = e == 168 ? 1 : e == 176 ? 2 : 3; std::memcpy(arena + 0x800 + e + 4, &fn, 4); short adj = 16; std::memcpy(arena + 0x800 + e, &adj, 2); }
    h16(572, static_cast<unsigned short>(state));
    everyResult = 0; activationResult = 1; advanceResult = 0; exitResult = 0; zoneIndex = 0;
    for (int& z : zoneResults) z = 0;
    float f28 = 10.0f; std::memcpy(obj + 28, &f28, 4);
}
static const std::string START = "dmg(1911);every(1911);act600;adv1;slide1;v168(16);";
static void expectCalls(const char* name, const std::string& want) {
    if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; }
}
int main() {
    reset(0); everyResult = 1; msg_run__5Gnome(obj); expectCalls("everyFrame stops", "dmg(1911);every(1911);");
    reset(0); activationResult = 0; msg_run__5Gnome(obj); expectCalls("activation stops", "dmg(1911);every(1911);act600;");
    reset(14); msg_run__5Gnome(obj); expectCalls("state 14", START);
    reset(static_cast<short>(0x400d)); msg_run__5Gnome(obj); expectCalls("state 0x400d (unsigned compare)", START);
    reset(-1); msg_run__5Gnome(obj); expectCalls("state -1", START);
    for (int s : {2, 4, 7, 11, 12}) { reset(s); msg_run__5Gnome(obj); expectCalls("quiet state", START); }
    // state 0
    reset(0); w32(256, 5); w32(808, 9); msg_run__5Gnome(obj);
    expectCalls("0 idle", START + "zone(300,32767);zone(400,16000);v184(16,1);");
    reset(0); zoneResults[0] = 1; msg_run__5Gnome(obj); expectCalls("0 near", START + "zone(300,32767);v184(16,3);");
    reset(0); zoneResults[1] = 1; msg_run__5Gnome(obj); expectCalls("0 far", START + "zone(300,32767);zone(400,16000);v184(16,3);");
    reset(0); w32(964, 1); w32(256, 9); w32(808, 9); msg_run__5Gnome(obj); expectCalls("0 busy, timer not below", START);
    reset(0); w32(964, 1); w32(256, 0xfffffffe); w32(808, 1); msg_run__5Gnome(obj); expectCalls("0 busy, timer below (signed)", START + "v184(16,1);");
    // state 1
    reset(1); msg_run__5Gnome(obj); expectCalls("1 idle", START + "zone(300,32767);zone(400,16000);wander;");
    reset(1); zoneResults[0] = 1; msg_run__5Gnome(obj); expectCalls("1 near", START + "zone(300,32767);v184(16,0);");
    reset(1); zoneResults[1] = 1; msg_run__5Gnome(obj); expectCalls("1 far", START + "zone(300,32767);zone(400,16000);v184(16,0);");
    reset(1); w32(964, 7); msg_run__5Gnome(obj); expectCalls("1 busy", START + "wander;");
    // state 3
    reset(3); msg_run__5Gnome(obj); expectCalls("3 no link", START + "v184(16,0);");
    reset(3); w32(576, 1); zoneResults[0] = 1; msg_run__5Gnome(obj); expectCalls("3 close", START + "zone(30,6000);v184(16,6);");
    reset(3); w32(576, 1); zoneResults[1] = 1; msg_run__5Gnome(obj); expectCalls("3 face", START + "zone(30,6000);zone(30,32767);turn;");
    reset(3); w32(576, 1); msg_run__5Gnome(obj); expectCalls("3 steer", START + "zone(30,6000);zone(30,32767);steer;");
    // state 5
    reset(5); zoneResults[0] = 1; msg_run__5Gnome(obj); expectCalls("5 close", START + "zone(30,6000);v184(16,6);");
    reset(5); exitResult = 1; msg_run__5Gnome(obj); expectCalls("5 exit", START + "zone(30,6000);exit;v184(16,3);");
    reset(5); msg_run__5Gnome(obj); expectCalls("5 stay", START + "zone(30,6000);exit;");
    // state 6
    reset(6); w32(576, 1); advanceResult = 1; w32(256, 5); h16(200, 3); h16(202, 0xfffe); h16(204, 7); msg_run__5Gnome(obj);
    expectCalls("6 hit", "dmg(1911);every(1911);act600;adv1;slide1;v168(16);melee(36,3,-2,7,8,null);");
    if (r32(256) != 4) { std::puts("fail 6 timer"); bad = 1; }
    reset(6); w32(576, 1); advanceResult = 0xfe; w32(256, 1); msg_run__5Gnome(obj);
    expectCalls("6 no hit, timer expires", START + "v184(16,3);"); if (r32(256) != 0) { std::puts("fail 6 timer 0"); bad = 1; }
    reset(6); w32(576, 1); advanceResult = 3; h16(294, 8); w32(256, 1); msg_run__5Gnome(obj);
    expectCalls("6 hit with bit 3 set", "dmg(1911);every(1911);act600;adv1;slide1;v168(16);melee(36,0,0,0,8,null);turn;");
    if (r32(256) != 1) { std::puts("fail 6 timer kept"); bad = 1; }
    reset(6); w32(256, 0xffffffffu); msg_run__5Gnome(obj); expectCalls("6 without link, negative timer", START + "v184(16,3);");
    if (r32(256) != 0xfffffffeu) { std::puts("fail 6 decrement"); bad = 1; }
    // state 8, 9, 10
    reset(8); msg_run__5Gnome(obj); expectCalls("8", START + "v184(16,3);");
    reset(8); h16(294, 0x10); msg_run__5Gnome(obj); expectCalls("8 bit 4", START);
    reset(9); w32(804, 1); w32(808, 2); msg_run__5Gnome(obj); expectCalls("9 below", START + "v184(16,3);");
    reset(9); w32(804, 2); w32(808, 2); msg_run__5Gnome(obj); expectCalls("9 equal", START);
    reset(9); w32(804, 0x80000000u); w32(808, 2); msg_run__5Gnome(obj); expectCalls("9 signed", START + "v184(16,3);");
    reset(10); msg_run__5Gnome(obj); expectCalls("10", START);
    reset(10); h16(294, 0x80); msg_run__5Gnome(obj); expectCalls("10 bit 7", START + "remove;");
    // state 13
    reset(13); msg_run__5Gnome(obj); expectCalls("13 no link", START + "v184(16,0);");
    reset(13); w32(968, 0x300); wa(0x300 + 12, 0x2800); { float p[3] = {1, 2, 3}; std::memcpy(arena + 0x2800 + 16, p, 12); }
    w32(972, 5); w32(808, 10); msg_run__5Gnome(obj);
    expectCalls("13 follow", START + "v176(0,1,2,3);v184(16,0);");
    if (r32(972) != 730) { std::puts("fail 13 timer"); bad = 1; }
    reset(13); w32(968, 0x300); wa(0x300 + 12, 0x2800); w32(972, 10); w32(808, 10); msg_run__5Gnome(obj);
    expectCalls("13 follow, timer not below", START + "v176(0,0,0,0);"); if (r32(972) != 10) { std::puts("fail 13 keep"); bad = 1; }
    if (!bad) std::puts("gnome msg_run ok");
    return bad;
}
