// Host contract for msg_run__5Rondo; not an EE input. Callees log in order; table calls (entry
// offsets 168 and 184) log "e168" / "e184(arg)". Words holding addresses are offsets from a host
// base. Walks every state and the branches inside them.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <string>
unsigned char* w10_actor_l_host_base;
static std::string calls; static int r_every, r_far, r_exit, r_steer, r_advance; static void* r_enemy;
unsigned w10_actor_l_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += (fn == 168 ? "e168" : "e184(" + std::to_string(static_cast<long>(a1)) + ")") + std::string(";");
    (void)adjusted; (void)n; return 0;
}
static std::string f(float v) { return std::to_string(v); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_l_host_base) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "every;"; return r_every; }
void objectActivationCheck__8Creaturei(void*, int n) { calls += "act(" + std::to_string(n) + ");"; }
int creatureAdvanceAnimation__8Creaturef(void*, float s) { calls += "adv(" + f(s) + ");"; return r_advance; }
void slideBack__8Creaturef(void*, float a) { calls += "slide(" + f(a) + ");"; }
int bFarFromHome__8Creature(void*) { calls += "far;"; return r_far; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exitp;"; return r_exit; }
void* alternateEnemyInZone__8Creatureffss(void*, float a, float b, short c, short d) { calls += "enemy(" + f(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; return r_enemy; }
int steerAndRouteToPosition__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); calls += "steer(" + f(v[0]) + "," + f(v[2]) + ");"; return r_steer; }
void flee2__8CreatureP10GameObject(void*, void* other) { calls += "flee(" + std::to_string(static_cast<unsigned char*>(other) - w10_actor_l_host_base) + ");"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* g) { calls += "melee(" + f(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(g == nullptr) + ");"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
#include "msg_run.cpp"
alignas(16) static unsigned char arena[0x4000];
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, unsigned short v) { std::memcpy(arena + at, &v, 2); }
static const std::string head = "dmg(2560);every;act(600);adv(1.000000);slide(0.300000);e168;";
static void reset(int state) {
    std::memset(arena, 0, sizeof arena); calls.clear(); r_every = r_far = r_exit = r_steer = 0; r_advance = 0; r_enemy = nullptr;
    put32(0x100 + 36, 0x800); put32(0x800 + 12, 0xa00); put32(0x100 + 72, 0x2000); put32(0x2000 + 168 + 4, 168); put32(0x2000 + 184 + 4, 184);
    put16(0x100 + 572, static_cast<unsigned short>(state)); put16(0x100 + 56, 7);
}
int main() {
    unsigned char* const o = arena + 0x100; w10_actor_l_host_base = arena; int bad = 0;
    const std::string enemy = "enemy(0.000000,20.000000,7,32767);";
    reset(0); r_every = 1; msg_run__5Rondo(o); if (calls != "dmg(2560);every;") bad = 1;
    reset(0); r_far = 1; msg_run__5Rondo(o); if (calls != head + "far;e184(1);") bad = 1;
    reset(0); r_enemy = arena + 5; msg_run__5Rondo(o); if (calls != head + "far;" + enemy + "e184(6);") bad = 1;
    reset(0); put32(0x100 + 256, 1); put32(0x100 + 808, 2); msg_run__5Rondo(o); if (calls != head + "far;" + enemy + "e184(1);") bad = 1;
    reset(0); put32(0x100 + 256, 2); put32(0x100 + 808, 2); msg_run__5Rondo(o); if (calls != head + "far;" + enemy) bad = 1;
    reset(0); put32(0x100 + 256, 0xffffffffu); put32(0x100 + 808, 0); msg_run__5Rondo(o); if (calls != head + "far;" + enemy + "e184(1);") bad = 1;  // signed compare
    reset(1); r_enemy = arena + 5; msg_run__5Rondo(o); if (calls != head + enemy + "e184(6);") bad = 1;
    reset(1); { float v[3] = {1.5f, 0.0f, 2.5f}; std::memcpy(o + 272, v, 12); } r_steer = 1; msg_run__5Rondo(o); if (calls != head + enemy + "steer(1.500000,2.500000);e184(0);") bad = 1;
    reset(1); put32(0x100 + 256, 0); put32(0x100 + 808, 1); msg_run__5Rondo(o); if (calls != head + enemy + "steer(0.000000,0.000000);e184(0);") bad = 1;
    reset(1); msg_run__5Rondo(o); if (calls != head + enemy + "steer(0.000000,0.000000);") bad = 1;
    reset(5); r_exit = 1; msg_run__5Rondo(o); if (calls != head + "exitp;e184(0);") bad = 1;
    reset(5); msg_run__5Rondo(o); if (calls != head + "exitp;") bad = 1;
    reset(6); put32(0x100 + 576, 1); r_advance = 1; put16(0x100 + 200, 3); put16(0x100 + 202, 0xfffe); put16(0x100 + 204, 5); msg_run__5Rondo(o);
    if (calls != head + "melee(36.000000,3,-2,5,1,1);turn;e184(0);") bad = 1;
    reset(6); put32(0x100 + 576, 1); r_advance = 2; put16(0x100 + 294, 8); msg_run__5Rondo(o); if (calls != head + "turn;") bad = 1;
    reset(6); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1;
    reset(7); put32(0x100 + 256, 0); put32(0x100 + 808, 1); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1;
    reset(7); put32(0x100 + 960, 0x1800); put32(0x1800 + 12, 0x1900); msg_run__5Rondo(o); if (calls != head + "flee(6400);") bad = 1;
    reset(7); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1;
    reset(8); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1;
    reset(8); put16(0x100 + 294, 0x10); msg_run__5Rondo(o); if (calls != head) bad = 1;
    reset(9); put32(0x100 + 804, 3); put32(0x100 + 808, 4); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1;
    reset(9); put32(0x100 + 804, 4); put32(0x100 + 808, 4); msg_run__5Rondo(o); if (calls != head) bad = 1;
    reset(10); put16(0x100 + 294, 0x80); msg_run__5Rondo(o); if (calls != head + "remove;") bad = 1;
    reset(10); msg_run__5Rondo(o); if (calls != head) bad = 1;
    for (int s : {2, 3, 4, 11, 12, -1, 0x7fff}) { reset(s); msg_run__5Rondo(o); if (calls != head + "e184(0);") bad = 1; }
    return bad;
}
