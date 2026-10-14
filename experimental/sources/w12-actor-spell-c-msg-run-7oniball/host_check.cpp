// Host contract for msg_run__7OniBall; not an EE input. 32-bit words are offsets from a host base and the
// callees log. The authored function was also compared with the original in a private R5900 run (49 scenarios:
// player counts, skill ranks, roll values around 5160, button masks, vertical offsets around +-9, close and far
// targets, ties; 321 of 323 original words reached, the missing two being padding).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long) {
    calls += "slot(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," +
             std::to_string(argument_count) + "," + std::to_string(a1) + ");";
    return 0;
}
extern "C" {
int frameNm;
int nmPlayersAlive;
int playersAlive[4];
unsigned blockButton[4];
unsigned char pad[88 * 4 + 16];
unsigned char player[23984 * 4];
unsigned char oniball_msg_run_particle_7d7810[80];
unsigned char oniball_msg_run_particle_7d78b0[80];
}
#include "msg_run.cpp"

static int rolls[8], rollCount, rollIndex;
static int skill[4];
static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const ball = arena + 0x100;
static unsigned char* const table = arena + 0x600;
extern "C" {
int playerGetSkillRank__Fii(int p, int s) { calls += "rank(" + std::to_string(p) + "," + std::to_string(s) + ");"; return skill[p]; }
int eRand__10GameObject(void*) { calls += "r;"; return rolls[rollIndex++ % rollCount]; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return 4096; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return 0.25f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float s) {
    calls += std::string("particle(") + (d == oniball_msg_run_particle_7d7810 ? "red" : "blue") + "," + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + "," + num(s) + ");";
}
int move__10ProjectileG6Point3(void* self, Point3 v) {
    calls += "move(" + std::to_string(static_cast<unsigned char*>(self) - ball) + "," + num(v.x) + ":" + num(v.y) + ":" + num(v.z) + ");";
    return 0;
}
}
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static short get16(unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static void reset() {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    std::memset(player, 0, sizeof player);
    calls.clear(); rollIndex = 0; rolls[0] = 40000; rollCount = 1;
    frameNm = 10; nmPlayersAlive = 0;
    putf(ball + 16, 0); putf(ball + 20, 0); putf(ball + 24, 0);
    put32(ball + 84, 5); put32(ball + 72, static_cast<unsigned>(table - arena));
    short adjust = -12; std::memcpy(table + 8, &adjust, 2); put32(table + 12, 0x4321);
    std::memset(skill, 0, sizeof skill);
    for (int i = 0; i < 4; ++i) { blockButton[i] = 1; std::memset(pad + 88 * i, 0, 88); }
}
static void addPlayer(int slot, float x, float y, float z, int rank, bool pressed, float f172) {
    unsigned char* object = arena + 0x1000 + 0x200 * slot;
    putf(object + 16, x); putf(object + 20, y); putf(object + 24, z);
    put32(player + 23984 * slot + 21872, static_cast<unsigned>(object - arena));
    putf(player + 23984 * slot + 172, f172);
    playersAlive[nmPlayersAlive++] = slot;
    skill[slot] = rank;
    if (pressed) put32(pad + 88 * slot + 8, 1);
}

int main() {
    int bad = 0;
    // no player, no wander roll: the heading turns by at most 327 towards the stub angle 4096; velocity (30, 15, 50)
    reset();
    msg_run__7OniBall(ball);
    if (calls != "r;atan(0,0);cos(327);sin(327);cos(327);sin(327);particle(red,0:0:0,0:0:0,0);move(0,30:15:50);slot(17185,"+ std::to_string(ball - arena - 12) + ",1,3);") {
        std::puts(calls.c_str()); std::puts("fail: idle sequence"); bad = 1;
    }
    if (get16(ball + 56) != 327 || getf(ball + 92) != 30.0f || getf(ball + 96) != 15.0f || getf(ball + 100) != 50.0f) { std::puts("fail: idle heading/velocity"); bad = 1; }
    // wander target: a roll below 5160 stores a point around the ball
    reset(); rolls[0] = 100; rolls[1] = 250; rolls[2] = 750; rolls[3] = 499; rollCount = 4;
    msg_run__7OniBall(ball);
    if (getf(ball + 104) != -500.0f + 250.0f || getf(ball + 108) != -500.0f + 750.0f || getf(ball + 112) != 0.0f - 499.0f + 500.0f) { std::puts("fail: wander target"); bad = 1; }
    // a single qualifying player holding the button attracts the ball
    reset(); addPlayer(2, 72.0f, 0.0f, 10.0f, 1, true, 5.0f); ball[116] = 1; frameNm = 1; put32(ball + 84, 9);
    msg_run__7OniBall(ball);
    if (getf(ball + 104) != 72.0f || getf(ball + 108) != 0.0f || getf(ball + 112) != 46.0f) { std::puts("fail: homing target"); bad = 1; }
    if (calls.find("particle(blue,") == std::string::npos || calls.find("slot(") != std::string::npos) { std::puts(calls.c_str()); std::puts("fail: homing calls"); bad = 1; }
    // homing speed: 72 * (1/72 as stored) just under 1.0 gives (int)(199.99998) = 199
    if (getf(ball + 92) != 0.5f * 199.0f || getf(ball + 96) != 0.25f * 199.0f) { std::printf("fail: homing speed %g %g\n", getf(ball + 92), getf(ball + 96)); bad = 1; }
    if (getf(ball + 100) != 200.0f) { std::puts("fail: vertical speed"); bad = 1; }
    // a player without the button, with no rank or with a non-positive record float is ignored
    reset(); addPlayer(1, 1.0f, 1.0f, 1.0f, 1, false, 5.0f);
    msg_run__7OniBall(ball);
    if (get32(ball + 104) != 0 || getf(ball + 100) != 50.0f) { std::puts("fail: button not pressed"); bad = 1; }
    reset(); addPlayer(1, 1.0f, 1.0f, 1.0f, 0, true, 5.0f);
    msg_run__7OniBall(ball);
    if (getf(ball + 100) != 50.0f) { std::puts("fail: no rank"); bad = 1; }
    reset(); addPlayer(1, 1.0f, 1.0f, 1.0f, 1, true, 0.0f);
    msg_run__7OniBall(ball);
    if (getf(ball + 100) != 50.0f) { std::puts("fail: record float"); bad = 1; }
    // the nearer of two players wins; ties keep the first
    reset(); addPlayer(0, 50.0f, 0.0f, 0.0f, 1, true, 5.0f); addPlayer(3, 0.0f, 40.0f, 0.0f, 1, true, 5.0f);
    msg_run__7OniBall(ball);
    if (getf(ball + 104) != 0.0f || getf(ball + 108) != 40.0f) { std::puts("fail: nearest player"); bad = 1; }
    reset(); addPlayer(0, 30.0f, 0.0f, 0.0f, 1, true, 5.0f); addPlayer(3, 0.0f, 30.0f, 0.0f, 1, true, 5.0f);
    msg_run__7OniBall(ball);
    if (getf(ball + 104) != 30.0f) { std::puts("fail: tie keeps the first"); bad = 1; }
    return bad;
}
