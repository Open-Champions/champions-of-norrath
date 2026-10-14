// Host contract for msg_run__10MummySnake; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (600 scenarios over the target link, positions,
// heading and angle results, bite distance, the owner word and its flag, and frame limits; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static int slot_amount, slot_a1;
unsigned w12_spell_c_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    calls += "amount(" + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return fn == 0x321 ? static_cast<unsigned>(slot_amount) : 0;
}
static int closest, angle_result;
static std::vector<std::string> trig_calls;
struct Added { Point3 where, velocity; void* def; float age; };
static std::vector<Added> added;
struct Effect { int kind; Point3 where; };
static std::vector<Effect> effects;
struct Attack { void* attacker; void* target; float a, b; int c, d, e, f; short slot[4]; };
static std::vector<Attack> attacks;
static float sound_volume;
static void *sound_header, *sound_object, *link_data;
static float atan_y, atan_x;
extern "C" {
int frameNm;
unsigned char player[23984 * 4 + 22000];
unsigned char skills[256];
unsigned char SnakeParticle[80];
float worldDifficulty;
int getClosestLivingPlayer__FG6Point3f(Point3, float) { calls += "closest;"; return closest; }
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void*) { calls += "link;"; link_data = link; std::memcpy(head, &link, 0); }
int iatan2__Fff(float y, float x) { calls += "atan;"; atan_y = y; atan_x = x; return angle_result; }
float icos__Fi(int a) { trig_calls.push_back("c" + std::to_string(a)); return 1.0f + static_cast<float>(a % 7); }
float isin__Fi(int a) { trig_calls.push_back("s" + std::to_string(a)); return 0.5f + static_cast<float>(a % 5); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*p, *v, d, f}); }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int c, int d, int e, int f, void* desc, bool) {
    calls += "attack;";
    Attack at{a, b, x, y, c, d, e, f, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(desc) + 36, 8);
    attacks.push_back(at);
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "sound;"; sound_header = h; sound_object = o; sound_volume = v; }
void addEffect__FiG6Point3(int kind, Point3 p) { effects.push_back({kind, p}); }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
static unsigned char arena[0x3000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static short get16(unsigned off) { short v; std::memcpy(&v, arena + off, 2); return v; }
enum { O = 0x100, TN = 0x600, W = 0x700, YN = 0x800, YO = 0x900, SK = 0xa00, TS = 0xb00, DESC = 0xc00 };
static void reset() {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); std::memset(skills, 0, sizeof skills);
    calls.clear(); trig_calls.clear(); added.clear(); effects.clear(); attacks.clear();
    frameNm = 100; closest = -1; angle_result = 0; slot_amount = 7; worldDifficulty = 3.0f;
    putf(O + 16, 10); putf(O + 20, 20); putf(O + 24, 30);
    put16(O + 84, 1000); put16(O + 92, 2000);
    put32(O + 88, 1000);
    put32(O + 76, YN); put32(YN + 12, YO); put32(YO + 12, 0);
}
int main() {
    // no target, no player in range: the heading grows by 150 and the owner word is set so only the countdown ends it
    reset();
    msg_run__10MummySnake(arena + O);
    if (calls != "closest;" || get16(O + 84) != 1150 || get16(O + 92) != 5400) return 1;
    // trig call order: icos(H), isin(H), icos(H+16384), isin(A), isin(H+16384), isin(A)
    const std::vector<std::string> order = {"c1150", "s1150", "c17534", "s5400", "s17534", "s5400"};
    if (trig_calls.size() < 6) return 2;
    for (int i = 0; i < 6; ++i) if (trig_calls[i] != order[i]) return 3;
    // drift: x += icos(H) * 2.1667 etc.
    if (std::fabs(getf(O + 16) - (10.0f + (1.0f + 1150 % 7) * 2.1666667f)) > 1e-3 || std::fabs(getf(O + 20) - (20.0f + (0.5f + 1150 % 5) * 2.1666667f)) > 1e-3) return 4;
    if (added.size() != 1 || added[0].def != SnakeParticle || added[0].age != 0.0f || added[0].velocity.x != 0.0f) return 5;
    // not done: owner alive, time left, no target
    if (!effects.empty()) return 6;
    // a closest player gets linked as the target when its word is non-zero
    reset(); closest = 2;
    std::memcpy(player + 2 * 23984 + 21872, "\x10\x0a\x00\x00", 4);
    msg_run__10MummySnake(arena + O);
    if (calls.substr(0, 13) != "closest;link;") return 7;
    // with a target: heading steers toward it by 500 each frame, z moves by 1 toward the target's centre
    reset(); put32(O + 80, TN); put32(TN + 12, W);
    putf(W + 16, 50); putf(W + 20, 60); putf(W + 24, 0); putf(W + 28, 3); putf(W + 32, 4);
    angle_result = 3000;
    msg_run__10MummySnake(arena + O);
    if (std::fabs(atan_y - 40.0f) > 1e-4 || std::fabs(atan_x - 40.0f) > 1e-4 || get16(O + 84) != 1500) return 8;
    if (getf(O + 24) != 29.0f) return 9;
    reset(); put32(O + 80, TN); put32(TN + 12, W);
    putf(W + 16, 50); putf(W + 20, 60); putf(W + 24, 100); putf(W + 28, 3); putf(W + 32, 4);
    angle_result = 100;
    msg_run__10MummySnake(arena + O);
    if (get16(O + 84) != 500 || getf(O + 24) != 31.0f) return 10;
    // a target flagged dead (bit 8) is replaced by a new search
    reset(); put32(O + 80, TN); put32(TN + 12, W); put32(W + 12, 8);
    msg_run__10MummySnake(arena + O);
    if (calls.substr(0, 8) != "closest;") return 11;
    // bite: close enough to the target, with an owner: an attack with the table amount, then the sound; the snake then ends
    reset(); put32(O + 80, TN); put32(TN + 12, W); put32(O + 96, 0xd00);
    putf(W + 16, 10); putf(W + 20, 20); putf(W + 24, 30); putf(W + 28, 100); putf(W + 32, 0);
    angle_result = 1000;
    const unsigned skill_object = SK;
    std::memcpy(skills + 104, &skill_object, 4);
    put32(SK + 96, TS); put32(TS + 96 + 4, 0x321);
    msg_run__10MummySnake(arena + O);
    if (attacks.size() != 1 || attacks[0].a != 50.0f || attacks[0].b != 50.0f || attacks[0].c != 10000 || attacks[0].f != 0x10000) return 12;
    if (attacks[0].slot[0] != 23 || attacks[0].slot[1] != 7 || attacks[0].slot[2] != 7 || attacks[0].slot[3] != 0) return 13;
    if (calls.find("amount(0,3);") == std::string::npos || calls.find("sound;") == std::string::npos || sound_volume != 1.0f) return 14;
    // end: six effects of kind 14, one of kind 3, then the delete
    if (effects.size() != 7 || effects[0].kind != 14 || effects[5].kind != 14 || effects[6].kind != 3 || calls.find("del;") == std::string::npos) return 15;
    return 0;
}
