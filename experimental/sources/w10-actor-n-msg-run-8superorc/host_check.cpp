// Host contract for msg_run__8SuperOrc; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (500 random-state scenarios over states 0..17, 767 of 789 original words reached by the first
// run; see the notes file for the final coverage).
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_run.cpp"

int frameNm;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int everyResult, advanceResult, bexResult, animResult;
static unsigned rollValue;
static bool zone[8];
static int zoneIndex, clearFaceBit;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned char* o() { return obj; }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + num(off(m)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + num(off(m)) + ");"; return everyResult; }
void objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceResult; }
int slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; return 0x55; }
void move2__8CreatureG6Point3(void*, Point3 p) { calls += "move(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
int enemyInZone__8Creaturefs(void*, float f, short a) { calls += "zone(" + num(f) + "," + num(a) + ");"; return zone[zoneIndex++ & 7] ? 1 : 0; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return bexResult; }
void turnToFaceEnemy__8Creature(void*) {
    calls += "turn;";
    if (clearFaceBit) { short f; std::memcpy(&f, obj + 294, 2); f = static_cast<short>(f & ~8); std::memcpy(obj + 294, &f, 2); }
}
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float r, int a, int b, int c, int flags, void* d) {
    unsigned short colour; std::memcpy(&colour, obj + 1058, 2);
    calls += "melee(" + num(r) + "," + num(a) + "," + num(b) + "," + num(c) + "," + std::to_string(flags) + (d ? ",d" : "") + ")" + (colour == 183 ? "" : "!") + ";";
}
int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* an, float f, void* pos, void* pair, short* v) {
    calls += "anim(" + num(f) + "," + num(off(pos) - 0x1000) + "," + num(off(pair) - 0x1000) + "," + num(off(v) - 0x1000) + ");"; return animResult;
}
void superorcSlamEffect__8SuperOrcP8Creaturei(void*, void* t, int f) { calls += "slam(" + num(off(t)) + "," + num(f) + ");"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return 0.25f; }
}
unsigned w10_actor_n_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned a, unsigned v) { std::memcpy(obj + a, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned a, float v) { std::memcpy(obj + a, &v, 4); }
static void put16(unsigned a, int v) { short s = static_cast<short>(v); std::memcpy(obj + a, &s, 2); }
static float getf(unsigned a) { float v; std::memcpy(&v, obj + a, 4); return v; }
static short get16(unsigned a) { short v; std::memcpy(&v, obj + a, 2); return v; }
static unsigned get32(unsigned a) { unsigned v; std::memcpy(&v, obj + a, 4); return v; }
static void reset(int state, bool target) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    everyResult = 0; advanceResult = 0; bexResult = 0; animResult = 0; rollValue = 0; zoneIndex = 0; clearFaceBit = 0; frameNm = 0;
    for (bool& z : zone) z = false;
    put32(36, 0x300); putA(0x300 + 12, 0x400); put32(72, 0x800);
    putA(0x800 + 168, 8); putA(0x800 + 172, 0xa8); putA(0x800 + 184, 0); putA(0x800 + 188, 0xb8);
    put16(572, state);
    if (target) { put32(576, 0x500); putA(0x500 + 12, 0x600); }
    putf(28, 30.0f);
    put16(200, 11); put16(202, 12); put16(204, 13);
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    const std::string head = "dmg(1024);every(1024);act600;";
    const std::string move = "adv1;slide0.4;move(0:0:0);";
    const std::string v168 = "v168(8,0,0);";
    auto st = [](int n) { return "v184(0,1," + num(n) + ");"; };
    // everyFrame ends the call at once
    reset(0, true); everyResult = 1; msg_run__8SuperOrc(obj); check("every", "dmg(1024);every(1024);");
    // unknown/idle states do nothing more
    for (int s : {2, 4, 7, 11, 12, 17, 40}) { reset(s, true); msg_run__8SuperOrc(obj); check("idle", head + move + v168); }
    // 0: the first zone test or the second one
    reset(0, true); msg_run__8SuperOrc(obj); check("0 none", head + move + v168 + "zone(400,32767);zone(500,16000);");
    reset(0, true); zone[0] = true; msg_run__8SuperOrc(obj); check("0 a", head + move + v168 + "zone(400,32767);" + st(3));
    reset(0, true); zone[1] = true; msg_run__8SuperOrc(obj); check("0 b", head + move + v168 + "zone(400,32767);zone(500,16000);" + st(3));
    reset(1, true); msg_run__8SuperOrc(obj); check("1 none", head + move + v168 + "zone(400,32767);zone(500,16000);wander;");
    reset(1, true); zone[1] = true; msg_run__8SuperOrc(obj); check("1 b", head + move + v168 + "zone(400,32767);zone(500,16000);" + st(0));
    // 3
    reset(3, false); msg_run__8SuperOrc(obj); check("3 no target", head + move + v168 + st(0));
    reset(3, true); zone[0] = true; rollValue = 64; msg_run__8SuperOrc(obj); check("3 a64", head + move + v168 + "zone(80,11000);rand;" + st(6));
    reset(3, true); zone[0] = true; rollValue = 65; msg_run__8SuperOrc(obj); check("3 a65", head + move + v168 + "zone(80,11000);rand;" + st(14));
    reset(3, true); zone[1] = true; rollValue = 7; msg_run__8SuperOrc(obj); check("3 b", head + move + v168 + "zone(80,11000);zone(120,5500);rand;" + st(16));
    reset(3, true); zone[1] = true; rollValue = 165; msg_run__8SuperOrc(obj); check("3 b2", head + move + v168 + "zone(80,11000);zone(120,5500);rand;" + st(6));
    reset(3, true); zone[2] = true; rollValue = 7; msg_run__8SuperOrc(obj); check("3 c", head + move + v168 + "zone(80,11000);zone(120,5500);zone(100,22000);rand;" + st(14));
    reset(3, true); zone[2] = true; rollValue = 99; msg_run__8SuperOrc(obj); check("3 c2", head + move + v168 + "zone(80,11000);zone(120,5500);zone(100,22000);rand;" + st(6));
    reset(3, true); zone[3] = true; frameNm = 5; put32(968, 5); msg_run__8SuperOrc(obj); check("3 d", head + move + v168 + "zone(80,11000);zone(120,5500);zone(100,22000);zone(250,3000);" + st(13));
    reset(3, true); zone[3] = true; frameNm = 4; put32(968, 5); msg_run__8SuperOrc(obj); check("3 d later", head + move + v168 + "zone(80,11000);zone(120,5500);zone(100,22000);zone(250,3000);steer;");
    reset(3, true); msg_run__8SuperOrc(obj); check("3 steer", head + move + v168 + "zone(80,11000);zone(120,5500);zone(100,22000);zone(250,3000);steer;");
    // 5
    reset(5, true); zone[0] = true; msg_run__8SuperOrc(obj); check("5 a", head + move + v168 + "zone(80,11000);" + st(6));
    reset(5, true); zone[1] = true; msg_run__8SuperOrc(obj); check("5 b", head + move + v168 + "zone(80,11000);zone(100,22000);" + st(14));
    reset(5, true); bexResult = 1; msg_run__8SuperOrc(obj); check("5 exit", head + move + v168 + "zone(80,11000);zone(100,22000);bexit;" + st(3));
    reset(5, false); msg_run__8SuperOrc(obj); check("5 no target", head + move + v168 + "bexit;");
    // 8, 9, 10
    reset(8, true); put16(294, 0x10); msg_run__8SuperOrc(obj); check("8 set", head + move + v168);
    reset(8, true); put16(294, 0xef); msg_run__8SuperOrc(obj); check("8 clear", head + move + v168 + st(3));
    reset(9, true); put32(804, 4); put32(808, 5); msg_run__8SuperOrc(obj); check("9 <", head + move + v168 + st(0));
    reset(9, true); put32(804, 5); put32(808, 5); msg_run__8SuperOrc(obj); check("9 =", head + move + v168);
    reset(9, true); put32(804, 0xfffffffeu); put32(808, 1); msg_run__8SuperOrc(obj); check("9 signed", head + move + v168 + st(0));
    reset(10, true); put16(294, 0x80); msg_run__8SuperOrc(obj); check("10", head + move + v168 + "rmrun;");
    reset(10, true); put16(294, 0x7f); msg_run__8SuperOrc(obj); check("10 clear", head + move + v168);
    // 6: one round when bit 0 of the advance result is set; the round picks its flags by the roll
    reset(6, true); advanceResult = 1; rollValue = 49; put16(294, 8); clearFaceBit = 1;
    msg_run__8SuperOrc(obj); check("6 round", head + move + v168 + "rand;melee(50,11,12,13,262152);turn;" + st(3));
    if (get16(1066) != 183 || get16(1064) != 110 || get16(1068) != 255 || get16(1070) != 100 || get16(1056) != 110 || get16(1058) != 183 || get16(1060) != 255 || get16(1062) != 100) { std::puts("fail 6 colours"); bad = 1; }
    reset(6, true); advanceResult = 1; rollValue = 50; put16(294, 8); msg_run__8SuperOrc(obj); check("6 round hi", head + move + v168 + "rand;melee(50,11,12,13,8);turn;");
    reset(6, true); advanceResult = 6; put16(294, 0); msg_run__8SuperOrc(obj); check("6 no round", head + move + v168 + st(3));
    reset(6, false); put16(294, 0); msg_run__8SuperOrc(obj); check("6 no target", head + move + v168 + st(3));
    reset(6, false); put16(294, 8); msg_run__8SuperOrc(obj); check("6 no target faces", head + move + v168);
    // 14: three rounds (bits 0..2), 16: two rounds (bits 0..1), 15 only the final check
    reset(14, true); advanceResult = 7; rollValue = 39; put16(294, 8); msg_run__8SuperOrc(obj);
    check("14", head + move + v168 + "rand;melee(70,11,12,13,262152);rand;melee(70,11,12,13,262152);rand;melee(70,11,12,13,262152);turn;");
    reset(14, true); advanceResult = 4; rollValue = 40; put16(294, 8); msg_run__8SuperOrc(obj); check("14 bit 2 only", head + move + v168 + "rand;melee(70,11,12,13,8);turn;");
    reset(16, true); advanceResult = 7; rollValue = 39; put16(294, 8); msg_run__8SuperOrc(obj);
    check("16", head + move + v168 + "rand;melee(90,11,12,13,262152);rand;melee(90,11,12,13,262152);turn;");
    reset(15, true); put16(294, 8); msg_run__8SuperOrc(obj); check("15 faces", head + move + v168 + "turn;");
    reset(15, true); put16(294, 8); clearFaceBit = 1; msg_run__8SuperOrc(obj); check("15 faces clears", head + move + v168 + "turn;" + st(3));
    reset(15, true); put16(294, 0); msg_run__8SuperOrc(obj); check("15 clear", head + move + v168 + st(3));
    reset(15, false); put16(294, 0); msg_run__8SuperOrc(obj); check("15 no target", head + move + v168 + st(3));
    // 13 without a target: advance, slide, move, then the decision from the word at +964
    reset(13, false); put16(294, 0); put32(964, 0); msg_run__8SuperOrc(obj); check("13 nt0", head.substr(0, 0) + "dmg(1024);every(1024);act600;v168(8,0,0);adv1;slide0.4;move(0:0:0);" + st(3));
    reset(13, false); put16(294, 0); put32(964, 5); rollValue = 29; msg_run__8SuperOrc(obj); check("13 nt15", "dmg(1024);every(1024);act600;v168(8,0,0);adv1;slide0.4;move(0:0:0);rand;" + st(15));
    reset(13, false); put16(294, 0); put32(964, 5); rollValue = 30; msg_run__8SuperOrc(obj); check("13 nt3", "dmg(1024);every(1024);act600;v168(8,0,0);adv1;slide0.4;move(0:0:0);rand;" + st(3));
    reset(13, false); put16(294, 8); put32(964, 5); msg_run__8SuperOrc(obj); check("13 nt face", "dmg(1024);every(1024);act600;v168(8,0,0);adv1;slide0.4;move(0:0:0);");
    // 13 with a target: near (d < 36) and the counter below 55 advances by 55 - t and clears the pair
    reset(13, true); frameNm = 100; putf(16, 10); putf(20, 20); putA(0x600 + 16, 0x41200000u + 0x0b000000u * 0); { float tx = 10 + 30.0f, ty = 20; std::memcpy(arena + 0x600 + 16, &tx, 4); std::memcpy(arena + 0x600 + 20, &ty, 4); }
    putf(960, 20); putf(180, 5); putf(184, 6); put16(188, 9); animResult = 0; put16(294, 0); put32(964, 0);
    msg_run__8SuperOrc(obj);
    check("13 near", "dmg(1024);every(1024);act600;v168(8,0,0);anim(35,16,180,188);slide0.4;move(0:0:0);" + st(3));
    if (get32(968) != 280 || getf(960) != 55.0f || getf(180) != 0.0f || getf(184) != 0.0f || get16(188) != 0) { std::puts("fail 13 near stores"); bad = 1; }
    // 13 far with the byte at +800 outside 1..60: step 1.0, scaled and rotated, no extra scaling
    reset(13, true); frameNm = 0; putf(16, 0); putf(20, 0); { float tx = 100, ty = 0; std::memcpy(arena + 0x600 + 16, &tx, 4); std::memcpy(arena + 0x600 + 20, &ty, 4); }
    putf(960, 70); putf(180, 2); putf(184, 4); putf(224, 2.0f); put16(56, 20000); obj[800] = 0; animResult = 0; put16(294, 0); put32(964, 0);
    msg_run__8SuperOrc(obj);
    check("13 far", "dmg(1024);every(1024);act600;v168(8,0,0);anim(1,16,180,188);cos3616;sin3616;slide0.4;move(0:0:0);" + st(3));
    // x = -(2*2) = -4, y = 4*2 = 8: new x = -4*0.5 + 8*0.25 = 0, new y = -4*0.25 - 8*0.5 = -5
    if (getf(960) != 71.0f || getf(180) != 0.0f || getf(184) != -5.0f) { std::printf("fail 13 far stores %g %g %g\n", getf(960), getf(180), getf(184)); bad = 1; }
    // 13 far with +800 = 30: step 0.5, then scaled by 0.5 and the halfword at +188 times int(0.5) = 0
    reset(13, true); putf(16, 0); putf(20, 0); { float tx = 100, ty = 0; std::memcpy(arena + 0x600 + 16, &tx, 4); std::memcpy(arena + 0x600 + 20, &ty, 4); }
    putf(960, 70); putf(180, 2); putf(184, 4); putf(224, 1.0f); put16(56, 16384); obj[800] = 30; put16(188, 7); animResult = 0; put16(294, 0); put32(964, 0);
    msg_run__8SuperOrc(obj);
    check("13 scaled", "dmg(1024);every(1024);act600;v168(8,0,0);anim(0.5,16,180,188);cos0;sin0;slide0.4;move(0:0:0);" + st(3));
    if (getf(960) != 70.5f || getf(180) != 0.0f || (getf(184) + 1.25f > 0.001f || getf(184) + 1.25f < -0.001f) || get16(188) != 0) { std::printf("fail 13 scaled stores %g %g %g %d\n", getf(960), getf(180), getf(184), get16(188)); bad = 1; }
    // 13 slam: b bit 0 and distance < 50 (far branch, so d < 36 is false: place the target at 40)
    reset(13, true); putf(16, 0); putf(20, 0); { float tx = 40, ty = 0; std::memcpy(arena + 0x600 + 16, &tx, 4); std::memcpy(arena + 0x600 + 20, &ty, 4); }
    putf(960, 70); obj[800] = 0; putf(224, 1.0f); animResult = 1; put32(964, 7); put16(294, 0);
    msg_run__8SuperOrc(obj);
    check("13 slam", "dmg(1024);every(1024);act600;v168(8,0,0);anim(1,16,180,188);cos-16384;sin-16384;slide0.4;move(0:0:0);slam(1536,1);melee(50,22,24,13,1048648);rand;" + st(15));
    reset(13, true); putf(16, 0); putf(20, 0); { float tx = 40, ty = 0; std::memcpy(arena + 0x600 + 16, &tx, 4); std::memcpy(arena + 0x600 + 20, &ty, 4); }
    putf(960, 70); obj[800] = 0; putf(224, 1.0f); animResult = 1; put32(964, 0); put16(294, 0);
    msg_run__8SuperOrc(obj);
    check("13 slam0", "dmg(1024);every(1024);act600;v168(8,0,0);anim(1,16,180,188);cos-16384;sin-16384;slide0.4;move(0:0:0);slam(1536,1);melee(50,22,24,13,524296);" + st(3));
    return bad;
}
