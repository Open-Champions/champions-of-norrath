// Host contract for msg_run__9MummyKing; not an EE input. 32-bit words are offsets from a host base;
// callees are stubs that log. The expected sequences follow the order of the original; the authored
// function was also compared with the original in a private R5900 run (1,000+ scenarios over every state).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int efResult, advResult, skillResult;
static int bools[16], boolIndex;
static unsigned rolls[16]; static int rollIndex;
static unsigned spawnResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
float worldDifficulty;
unsigned skills[64];
int nmPlayers;
unsigned char player[0x20000];
void* findTagString__10GameObjectPci(void*, const char* t, int) { calls += std::string("tag(") + t + ");"; return arena + 0x5000; }
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short a, void* m2, void* an, float f, int g, int h, float i, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + pt(p) + "," + num(a) + "," + num(off(m2)) + "," + num(off(an) - 0x1000) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + ");";
}
void newDamageHandler__8CreatureP12_modelHeader(void*, void* h) { calls += "ndh(" + num(off(h)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* h) { calls += "ef(" + num(off(h)) + ");"; return efResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "oac" + num(r) + ";"; return 0; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "adv" + num(a) + ";"; return static_cast<unsigned>(advResult); }
void slideBack__8Creaturef(void*, float a) { calls += "slide" + num(a) + ";"; }
int enemyInZone__8Creaturefs(void*, float r, short s) { calls += "zone(" + num(r) + "," + num(s) + ");"; return bools[boolIndex++ & 15]; }
int alternateEnemyInZone__8Creatureffss(void*, float a, float b, short c, short d) { calls += "azone(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");"; return bools[boolIndex++ & 15]; }
int bSpawnMummy__9MummyKing(void*) { calls += "bsm;"; return bools[boolIndex++ & 15]; }
int bSpawnSnake__9MummyKing(void*) { calls += "bss;"; return bools[boolIndex++ & 15]; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return bools[boolIndex++ & 15]; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + pt(p) + ");"; }
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void*, float a, float b, short c, short d, int e, int f, int g, int h, void* desc) {
    calls += "dmg(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + num(h) + ",";
    if (!desc) { calls += "none);"; return; }
    const unsigned char* p = static_cast<const unsigned char*>(desc);
    short code, rolled, boosted, z; std::memcpy(&code, p + 36, 2); std::memcpy(&rolled, p + 38, 2); std::memcpy(&boosted, p + 40, 2); std::memcpy(&z, p + 42, 2);
    short second; std::memcpy(&second, p + 44, 2);
    calls += "code" + num(code) + ",r" + num(rolled) + ",b" + num(boosted) + ",z" + num(z) + ",next" + num(second) + ",b" + num(p[32]) + num(p[33]) + ");";
}
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 15]; }
float icos__Fi(int a) { calls += "icos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "isin" + num(a) + ";"; return 0.25f; }
void objectFindEmptySpot__FG6Point3iiPc(Point3* out, Point3 p, int a, int b, char* found) {
    calls += "spot(" + pt(p) + "," + num(a) + "," + num(b) + ");"; *out = Point3{70, 80, 90}; *found = static_cast<char>(spawnResult);
}
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return arena + 0x6000; }
void spawn_mummy_construct(void*, Point3 p, char* name, void* owner) { calls += "mummy(" + pt(p) + "," + name + "," + (owner == obj ? "king" : "other") + ");"; }
void mummy_snake_construct(void*, Point3 p, short h, void* owner, void* who, void* header) {
    calls += "snake(" + pt(p) + "," + num(h) + "," + (owner == obj ? "king" : "other") + "," + num(off(who)) + "," + num(off(header)) + ");";
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void*, void*) { calls += "link(" + num(off(head) - 0x1000) + ");"; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float v) { calls += "sound(" + num(off(h)) + "," + num(v) + ");"; }
void soundFadeStream__Fi(int s) { calls += "fade" + num(s) + ";"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void RandomTeleportSpell__FP10GameObjectP6Point3f(void*, Point3* where, float r) { calls += "tele(" + (where ? num(off(where)) : std::string("null")) + "," + num(r) + ");"; }
void dust_devil_construct(void*, Point3 p) { calls += "devil(" + pt(p) + ");"; }
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long a2) {
    if (fw == 0xe0) { calls += "skill(" + num(off(adjusted)) + "," + num(a1) + "," + num(static_cast<int>(a2)) + ");"; return static_cast<unsigned>(skillResult); }
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(static_cast<int>(a1)) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void reset(int state) {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); w10_actor_j_host_base = arena; calls.clear();
    efResult = 0; advResult = 0; boolIndex = 0; rollIndex = 0; spawnResult = 1; skillResult = 0; nmPlayers = 0; worldDifficulty = 1.0f;
    std::memset(bools, 0, sizeof bools); std::memset(rolls, 0, sizeof rolls);
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 168, 8); putA(0x400 + 172, 0xa8); putA(0x400 + 184, 16); putA(0x400 + 188, 0xb8);
    for (unsigned i = 0; i < 100; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    put16(572, static_cast<unsigned short>(state));
    putf(16, 4); putf(20, 5); putf(24, 6); putf(28, 100); put32(808, 5000); put32(256, 0);
    put16(56, 7);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
static const std::string head = "ndh(8204);ef(8204);oac600;adv1;slide0.2;v168(4104,0,0);";
static std::string set(int n) { return "v184(" + num(0x1010) + ",1," + num(n) + ");"; }
int main() {
    int bad = 0;
    // The flag (bit 26) fills +892 once from the tag; clear flag clears it.
    reset(2); put32(160, 0x04000000u); msg_run__9MummyKing(obj); bad |= check("tag", "tag(ShowName);" + head);
    if (get32(892) == 0) { std::puts("fail tag store"); bad = 1; }
    reset(2); put32(160, 0x04000000u); put32(892, 9); msg_run__9MummyKing(obj); bad |= check("tag kept", head);
    reset(2); put32(892, 9); msg_run__9MummyKing(obj); bad |= check("tag cleared", head); if (get32(892) != 0) bad = 1;
    // Weapon effect: states 6, 13, 15 and 16 set the flag argument.
    reset(6); put32(960, 3); msg_run__9MummyKing(obj);
    bad |= check("weapon", "fx(1032,8220,964,4:5:6,7,8204,288,0.5,1,0,1,null);" + head + "move2(0:0:0);" + set(3));
    reset(2); put32(960, 3); msg_run__9MummyKing(obj); bad |= check("weapon idle", "fx(1032,8220,964,4:5:6,7,8204,288,0.5,0,0,1,null);" + head);
    reset(2); efResult = 1; msg_run__9MummyKing(obj); bad |= check("everyFrame", "ndh(8204);ef(8204);");
    // State 0, 1, 9, 14 and the large states.
    reset(0); bools[0] = 1; msg_run__9MummyKing(obj); bad |= check("0", head + "zone(400,32767);" + set(3));
    reset(0); msg_run__9MummyKing(obj); bad |= check("0 none", head + "zone(400,32767);");
    reset(1); put32(256, 6000); msg_run__9MummyKing(obj); bad |= check("1 wander", head + "zone(400,32767);wander;");
    reset(1); put32(256, 10); put32(808, 20); msg_run__9MummyKing(obj); bad |= check("1 idle", head + "zone(400,32767);" + set(0));
    reset(9); put32(804, 1); msg_run__9MummyKing(obj); bad |= check("9", head + set(3));
    reset(14); put32(256, 1); msg_run__9MummyKing(obj); bad |= check("14", head + set(3));
    reset(18); msg_run__9MummyKing(obj); bad |= check("18", head);
    // State 3: no target -> state 0; spawn paths; zone choices.
    reset(3); msg_run__9MummyKing(obj); bad |= check("3 no target", head + set(0));
    reset(3); put32(576, 1); put32(1104, 1); bools[0] = 1; msg_run__9MummyKing(obj); bad |= check("3 spawn mummy", head + "bsm;" + set(16));
    reset(3); put32(576, 1); put32(1104, 9000); put32(1152, 9000); bools[0] = 1; rolls[0] = 1; msg_run__9MummyKing(obj);
    bad |= check("3 zone odd", head + "zone(150,4000);rand;" + set(6));
    reset(3); put32(576, 1); put32(1104, 9000); put32(1152, 9000); bools[0] = 1; rolls[0] = 2; msg_run__9MummyKing(obj);
    bad |= check("3 zone even", head + "zone(150,4000);rand;" + set(13));
    reset(3); put32(576, 1); put32(1104, 9000); put32(1152, 10); bools[0] = 1; msg_run__9MummyKing(obj); bad |= check("3 zone timer", head + "zone(150,4000);" + set(17));
    reset(3); put32(576, 1); put32(1104, 9000); put32(1152, 9000); bools[1] = 1; rolls[0] = 1; msg_run__9MummyKing(obj);
    bad |= check("3 azone", head + "zone(150,4000);azone(0,150,7,6000);rand;" + set(6));
    reset(3); put32(576, 1); put32(1104, 9000); bools[2] = 1; msg_run__9MummyKing(obj); bad |= check("3 snake", head + "zone(150,4000);azone(0,150,7,6000);bss;" + set(15));
    reset(3); put32(576, 1); put32(1104, 9000); bools[3] = 1; msg_run__9MummyKing(obj); bad |= check("3 mummy", head + "zone(150,4000);azone(0,150,7,6000);bss;bsm;" + set(16));
    reset(3); put32(576, 1); put32(1104, 9000); msg_run__9MummyKing(obj); bad |= check("3 steer", head + "zone(150,4000);azone(0,150,7,6000);bss;bsm;steer;");
    reset(5); put32(1104, 9000); rolls[0] = 1; bools[0] = 1; msg_run__9MummyKing(obj); bad |= check("5 zone", head + "zone(150,4000);rand;" + set(6));
    reset(5); put32(1104, 9000); bools[4] = 1; msg_run__9MummyKing(obj); bad |= check("5 exit", head + "zone(150,4000);azone(0,150,7,6000);bss;bsm;bexit;" + set(3));
    // State 6: damage rolls by eRand & 3.
    reset(6); put32(576, 1); advResult = 1; rolls[0] = 4; put16(200, 1); put16(202, 2); put16(204, 3); put16(294, 8); msg_run__9MummyKing(obj);
    bad |= check("6 a", head + "move2(0:0:0);rand;dmg(0,150,7,6000,1,2,3,524289,none);turn;");
    reset(6); put32(576, 1); advResult = 1; rolls[0] = 5; msg_run__9MummyKing(obj);
    bad |= check("6 b", head + "move2(0:0:0);rand;dmg(0,150,7,6000,0,0,0,262145,none);turn;" + set(3));
    // State 8 and 10.
    reset(8); msg_run__9MummyKing(obj); bad |= check("8", head + set(0));
    reset(8); put16(294, 0x10); msg_run__9MummyKing(obj); bad |= check("8 held", head);
    reset(10); put16(294, 0x80); msg_run__9MummyKing(obj); bad |= check("10", head + "fade0;rmrun;");
    // State 13: description roll from the skill table (index 26 when the byte is set).
    reset(13); put32(576, 1); advResult = 1; put32(1160, 0); obj[1160] = 1; skills[26] = 0x3000; putA(0x3000 + 96, 0x3100); putA(0x3100 + 96, 4); putA(0x3100 + 100, 0xe0);
    skillResult = 40; rolls[0] = 0x8000; msg_run__9MummyKing(obj);
    bad |= check("13 roll", head + "move2(0:0:0);skill(" + num(0x3004) + ",0,1);rand;dmg(0,150,7,6000,0,0,0,1,code23,r40,b43,z0,next-1,b255255);turn;" + set(3));
    reset(13); put32(576, 1); advResult = 1; skills[25] = 0x3000; putA(0x3000 + 96, 0x3100); putA(0x3100 + 96, 0); putA(0x3100 + 100, 0xe0);
    skillResult = 10; rolls[0] = 0; msg_run__9MummyKing(obj);
    bad |= check("13 roll b", head + "move2(0:0:0);skill(12288,0,1);rand;dmg(0,150,7,6000,0,0,0,1,code38,r10,b10,z0,next-1,b255255);turn;" + set(3));
    // State 15: snakes for players that have a free slot.
    reset(15); advResult = 1; nmPlayers = 2; putA(0, 0); { unsigned t = 0x7000; std::memcpy(player + 21872, &t, 4); std::memcpy(player + 21872 + 23984, &t, 4); }
    putA(0x7000 + 12, 0); put32(1140, 5);
    msg_run__9MummyKing(obj);
    bad |= check("15", head + "icos-2000;isin-2000;alloc100;snake(16:11:54,7,king,28672,8332);link(1136);sound(8324,1);" + set(3));
    // State 16: spawns by tier.
    reset(16); nmPlayers = 2; put32(1128, 0); put32(256, 0); putf(560, 90); putf(564, 100); spawnResult = 0; msg_run__9MummyKing(obj);
    bad |= check("16 first", head + "spot(4:5:6,200,15);icos7;isin7;alloc104;mummy(29:17.5:6,mummydesertsoldier,king);link(1108);sound(8340,1);");
    if (get32(1128) != 1) { std::puts("fail 16 counter"); bad = 1; }
    // State 17: first call returns home when far, later calls clear flags.
    reset(17); put32(256, 4900); putf(776, 100); putf(780, 100); putf(784, 100); putf(788, 1); put32(576, 0); msg_run__9MummyKing(obj);
    bad |= check("17 home", head + "sound(8348,1);grid;alloc88;devil(100:100:100);");
    if (get32(1156) != 1 || get32(256) != 5000) { std::puts("fail 17 stores"); bad = 1; }
    reset(17); put32(1156, 1); put32(256, 4850); put32(12, 0xffffffffu); msg_run__9MummyKing(obj); bad |= check("17 clear", head + set(3));
    if (get32(12) != 0xfffffffcu) { std::puts("fail 17 clear store"); bad = 1; }
    return bad;
}
