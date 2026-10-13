// Host contract for msg_run__8Scorpion; not an EE input. Callees log in order; table calls log
// "e168" / "e184(arg)". Words holding addresses are offsets from a host base. Walks every state and
// the branches inside them, including the difficulty scaling and the item description bytes.
#define W10_ACTOR_L_HOST 1
#include "../w10-actor-l-common/access.hpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10_actor_l_host_base;
static std::string calls; static int r_every, r_act = 1, r_exit, r_advance; static std::vector<int> r_enemy; static size_t enemy_i; static unsigned rolls[4]; static int roll_i;
static short seen_desc_first; static unsigned char seen_desc[64]; static int seen_desc_valid, seen_mode;
unsigned w10_actor_l_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long) {
    calls += (fn == 168 ? "e168" : "e184(" + std::to_string(static_cast<long>(a1)) + ")") + std::string(";"); return 0;
}
static std::string f(float v) { return std::to_string(v); }
extern "C" {
int gameDifficulty;
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_l_host_base) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "every;"; return r_every; }
int objectActivationCheck__8Creaturei(void*, int n) { calls += "act(" + std::to_string(n) + ");"; return r_act; }
int creatureAdvanceAnimation__8Creaturef(void*, float s) { calls += "adv(" + f(s) + ");"; return r_advance; }
void slideBack__8Creaturef(void*, float a) { calls += "slide(" + f(a) + ");"; }
void* enemyInZone__8Creaturefs(void*, float r, short v) { calls += "enemy(" + f(r) + "," + std::to_string(v) + ");"; int x = enemy_i < r_enemy.size() ? r_enemy[enemy_i] : 0; ++enemy_i; return x ? w10_actor_l_host_base + 5 : nullptr; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exitp;"; return r_exit; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* g) {
    calls += "melee(" + f(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(g != nullptr) + ");";
    seen_desc_valid = g != nullptr; if (g) std::memcpy(seen_desc, g, 64); }
void move2__8CreatureG6Point3(void*, const void* p) { float v[3]; std::memcpy(v, p, 12); calls += "move2(" + f(v[0]) + f(v[1]) + f(v[2]) + ");"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[roll_i++ & 3]; }
void floorAdd__FG6Point3sfii(const void* p, short a, float b, int c, int d) { float v[3]; std::memcpy(v, p, 12); calls += "floor(" + f(v[0]) + "," + f(v[1]) + "," + f(v[2]) + "," + std::to_string(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; }
}
#include "msg_run.cpp"
alignas(16) static unsigned char arena[0x4000];
static void put32(unsigned at, unsigned v) { std::memcpy(arena + 0x100 + at, &v, 4); }
static void put16(unsigned at, unsigned short v) { std::memcpy(arena + 0x100 + at, &v, 2); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + 0x100 + at, 4); return v; }
static short get16(unsigned at) { short v; std::memcpy(&v, arena + 0x100 + at, 2); return v; }
static const std::string head = "dmg(2560);every;act(600);adv(1.000000);slide(0.500000);e168;";
static void reset(int state) {
    std::memset(arena, 0, sizeof arena); calls.clear(); r_every = 0; r_act = 1; r_exit = 0; r_advance = 0; r_enemy.clear(); enemy_i = 0; roll_i = 0; seen_desc_valid = 0; gameDifficulty = 0;
    for (int i = 0; i < 4; ++i) rolls[i] = 0;
    put32(36, 0x800); { unsigned m = 0xa00; std::memcpy(arena + 0x800 + 12, &m, 4); } put32(72, 0x2000); put32(0x2000 - 0x100 + 168 + 4, 168); put32(0x2000 - 0x100 + 184 + 4, 184);
    put16(572, static_cast<unsigned short>(state));
}
int main() {
    unsigned char* const o = arena + 0x100; w10_actor_l_host_base = arena; int bad = 0;
    reset(0); r_every = 1; msg_run__8Scorpion(o); if (calls != "dmg(2560);every;") bad = 1;
    reset(0); r_act = 0; msg_run__8Scorpion(o); if (calls != "dmg(2560);every;act(600);") bad = 1;
    for (int s : {2, 4, 7, 11, 12, 16, 99, -1}) { reset(s); msg_run__8Scorpion(o); if (calls != head) bad = 1; }
    // case 0
    reset(0); r_enemy = {1}; msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);e184(3);") bad = 1;
    reset(0); r_enemy = {0, 1}; msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);enemy(500.000000,16000);e184(3);") bad = 1;
    reset(0); put32(256, 1); put32(808, 2); msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);enemy(500.000000,16000);e184(1);") bad = 1;
    reset(0); put32(256, 2); put32(808, 2); msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);enemy(500.000000,16000);") bad = 1;
    // case 1
    reset(1); r_enemy = {1}; msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);e184(0);") bad = 1;
    reset(1); r_enemy = {0, 1}; msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);enemy(500.000000,16000);e184(0);") bad = 1;
    reset(1); msg_run__8Scorpion(o); if (calls != head + "enemy(400.000000,32767);enemy(500.000000,16000);wander;") bad = 1;
    // case 3
    float x28 = 1.5f; reset(3); msg_run__8Scorpion(o); if (calls != head + "e184(0);") bad = 1;
    reset(3); put32(576, 1); std::memcpy(o + 28, &x28, 4); r_enemy = {1}; msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);e184(6);") bad = 1;
    reset(3); put32(576, 1); std::memcpy(o + 28, &x28, 4); r_enemy = {0, 1}; msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);enemy(21.500000,32767);turn;") bad = 1;
    reset(3); put32(576, 1); std::memcpy(o + 28, &x28, 4); msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);enemy(21.500000,32767);steer;") bad = 1;
    // case 5
    reset(5); std::memcpy(o + 28, &x28, 4); r_enemy = {1}; msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);e184(6);") bad = 1;
    reset(5); std::memcpy(o + 28, &x28, 4); r_exit = 1; msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);exitp;e184(3);") bad = 1;
    reset(5); std::memcpy(o + 28, &x28, 4); msg_run__8Scorpion(o); if (calls != head + "enemy(21.500000,6000);exitp;") bad = 1;
    // cases 8, 9, 14, 15
    reset(8); msg_run__8Scorpion(o); if (calls != head + "e184(3);") bad = 1;
    reset(8); put16(294, 0x10); msg_run__8Scorpion(o); if (calls != head) bad = 1;
    reset(9); put32(804, 1); put32(808, 2); msg_run__8Scorpion(o); if (calls != head + "e184(0);") bad = 1;
    reset(9); put32(804, 2); put32(808, 2); msg_run__8Scorpion(o); if (calls != head) bad = 1;
    for (int s : {14, 15}) { reset(s); msg_run__8Scorpion(o); if (calls != head + "move2(0.0000000.0000000.000000);e184(3);") bad = 1; reset(s); put16(294, 8); msg_run__8Scorpion(o); if (calls != head + "move2(0.0000000.0000000.000000);") bad = 1; }
    // case 10
    reset(10); put16(294, 0x80); msg_run__8Scorpion(o); if (calls != head + "remove;") bad = 1;
    reset(10); r_advance = 1; put32(12, 2); msg_run__8Scorpion(o); if (calls != head) bad = 1;
    reset(10); r_advance = 1; rolls[0] = 0x12345; rolls[1] = 33;
    { float p[3] = {2.0f, 3.0f, 4.0f}; std::memcpy(o + 16, p, 12); } put16(56, 7); msg_run__8Scorpion(o);
    if (calls != head + "cos(7);sin(7);rand;rand;floor(2.500000,2.750000,4.000000,9029,28.000000,6,1);") bad = 1;
    // cases 6 / 13
    reset(6); put32(256, 5); msg_run__8Scorpion(o); if (calls != head || get32(256) != 4) bad = 1;
    reset(6); put32(576, 1); put32(256, 5); put16(294, 8); msg_run__8Scorpion(o); if (calls != head + "turn;" || get32(256) != 5) bad = 1;
    reset(6); put32(576, 1); put32(256, 5); r_advance = 1; put16(200, 40); put16(202, 0xfff0); put16(204, 3); msg_run__8Scorpion(o);
    if (calls != head + "melee(36.000000,40,-16,3,8,0);" || get32(256) != 4) bad = 1;
    reset(13); put32(576, 1); put32(256, 5); r_advance = 3; put16(200, 40); put16(202, 0xfff0); put16(204, 3); msg_run__8Scorpion(o);
    if (calls != head + "melee(36.000000,40,-16,3,65540,1);" || !seen_desc_valid) bad = 1;
    {   // description: shorts at 36 + 8j are -1 except slot 0 which became {23, 40, -16, 0}; bytes 32/33 are 0xff; shorts 16..31 zero
        short s[4]; std::memcpy(s, seen_desc + 36, 8); if (s[0] != 23 || s[1] != 40 || s[2] != -16 || s[3] != 0) bad = 1;
        for (int j = 1; j < 4; ++j) { short v; std::memcpy(&v, seen_desc + 36 + 8 * j, 2); if (v != -1) bad = 1; }
        if (seen_desc[32] != 0xff || seen_desc[33] != 0xff) bad = 1;
        for (int i = 16; i < 32; ++i) if (seen_desc[i] != 0) bad = 1;
    }
    reset(13); gameDifficulty = 2; put32(576, 1); put32(256, 5); r_advance = 1; put16(200, 41); put16(202, 0xffe1); put16(204, 3); msg_run__8Scorpion(o);
    if (calls != head + "melee(36.000000,10,-7,3,65540,1);" || get16(200) != 10 || get16(202) != -7) bad = 1;   // 41/4 = 10, -31/4 = -7 (truncation toward zero)
    reset(13); gameDifficulty = 3; put32(576, 1); put32(256, 5); r_advance = 1; put16(200, 100); put16(202, 0xffa0); put16(204, 3); msg_run__8Scorpion(o);
    if (calls != head + "melee(36.000000,12,-12,3,65540,1);" || get16(200) != 12 || get16(202) != -12) bad = 1;
    // zero counter: the draw path
    reset(6); put32(256, 1); rolls[0] = 1; msg_run__8Scorpion(o); if (calls != head + "rand;e184(14);" || get32(256) != 0) bad = 1;
    reset(6); put32(256, 1); rolls[0] = 2; msg_run__8Scorpion(o); if (calls != head + "rand;e184(15);") bad = 1;
    reset(6); put32(256, 1); rolls[0] = 4; msg_run__8Scorpion(o); if (calls != head + "rand;e184(3);") bad = 1;
    reset(6); put32(256, 1); put16(294, 8); msg_run__8Scorpion(o); if (calls != head || get32(256) != 1) bad = 1;
    return bad;
}
