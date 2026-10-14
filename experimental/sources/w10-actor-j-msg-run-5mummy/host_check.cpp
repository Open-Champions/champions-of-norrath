// Host contract for msg_run__5Mummy; not an EE input. 32-bit words are offsets from a host base; callees
// are stubs that log. The expected sequences follow the order of the original; the authored function was
// also compared with the original in a private R5900 run (over 1,200 scenarios across every state, the
// word +960 from 0 to 6, spell choices and spawn results).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int efResult, oacResult, boolsList[16], boolIndex, intsList[8], intIndex, yawResult;
static unsigned advResult, rolls[16]; static int rollIndex;
static unsigned long deadResult;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
float worldDifficulty;
unsigned skills[64];
void newDamageHandler__8CreatureP12_modelHeader(void*, void* h) { calls += "ndh(" + num(off(h)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* h) { calls += "ef(" + num(off(h)) + ");"; return efResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "oac" + num(r) + ";"; return oacResult; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "adv" + num(a) + ";"; return advResult; }
void slideBack__8Creaturef(void*, float a) { calls += "slide" + num(a) + ";"; }
int enemyInZone__8Creaturefs(void*, float r, short s) { calls += "zone(" + num(r) + "," + num(s) + ");"; return boolsList[boolIndex++ & 15]; }
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int f) {
    calls += "zone2(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");"; return boolsList[boolIndex++ & 15];
}
int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* t, float a, float b, int c) { calls += "crowd(" + num(off(t)) + "," + num(a) + "," + num(b) + "," + num(c) + ");"; return boolsList[boolIndex++ & 15]; }
int numInRegion__8creatureG6Point3f(Point3 p, float r) { calls += "region(" + pt(p) + "," + num(r) + ");"; return intsList[intIndex++ & 7]; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return intsList[intIndex++ & 7]; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return boolsList[boolIndex++ & 15]; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + pt(p) + ");"; }
void creatureSlamEffect__8CreatureP8Creaturei(void*, void* t, int v) { calls += "slam(" + num(off(t)) + "," + num(v) + ");"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* desc) {
    calls += "melee(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(off(desc) - 0x1000) + ");";
}
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 15]; }
float icos__Fi(int a) { calls += "icos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "isin" + num(a) + ";"; return 0.25f; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult; }
void dust_devil_construct(void*, Point3 p) { calls += "devil(" + pt(p) + ");"; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "sound(" + num(off(h)) + (o == obj ? ",this," : ",other,") + num(v) + ");"; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void RandomTeleportSpell__FP10GameObjectP6Point3f(void*, Point3* w, float r) { calls += "tele(" + (w ? num(off(w)) : std::string("null")) + "," + num(r) + ");"; }
void* __tf15SpellConeOfFire() { calls += "tf;"; return arena + 0x7000; }
void terminateSpell__8CreatureRC9type_info(void*, const void* t) { calls += std::string("term") + (t == arena + 0x7000 ? "ok" : "bad") + ";"; }
void* ISeeDeadPeople__8Creaturef(void*, float r) { calls += "seedead(" + num(r) + ");"; return reinterpret_cast<void*>(deadResult); }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, Point3* p) { calls += "lead(" + num(r) + "," + pt(*p) + ");"; }
int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* t, float s) { calls += "yaw(" + num(off(t)) + "," + num(s) + ");"; return yawResult; }
void addSpell__8CreatureP5Spell(void*, void* s) { calls += std::string("addspell(") + (s ? "s" : "null") + ");"; }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return arena + 0x7100; }
void sparks_construct(void*, Point3 p, Point3 v, void* c, void* v1, void* v2, int a, int b, int d, int e) {
    calls += "sparks(" + pt(p) + "," + pt(v) + "," + (c == obj ? "this" : "other") + "," + (v1 || v2 ? "vag" : "novag") + "," + num(a) + "," + num(b) + "," + num(d) + "," + num(e) + ");";
}
void fire_storm_construct(void*, int d, void* c, Point3 p, void* s) { calls += "firestorm(" + num(d) + "," + (c == obj ? "this" : "other") + "," + pt(p) + "," + num(off(s)) + ");"; }
void cone_of_fire_construct(void*, int d, void* c, void* s) { calls += "cone(" + num(d) + "," + (c == obj ? "this" : "other") + "," + num(off(s)) + ");"; }
void disease_bolt_construct(void*, int h, int d, Point3 p, void* c, void* s) { calls += "bolt(" + num(h) + "," + num(d) + "," + pt(p) + "," + (c == obj ? "this" : "other") + "," + num(off(s)) + ");"; }
void poison_rain_construct(void*, int d, void* c, Point3 p, void* s, void* h) { calls += "rain(" + num(d) + "," + (c == obj ? "this" : "other") + "," + pt(p) + "," + num(off(s)) + "," + num(off(h)) + ");"; }
void disease_shield_construct(void*, int d, void* c, void* s) { calls += "shield(" + num(d) + "," + (c == obj ? "this" : "other") + "," + num(off(s)) + ");"; }
void shock_of_frost_construct(void*, Point3 p, int h, int d, void* c, void* s) { calls += "shock(" + pt(p) + "," + num(h) + "," + num(d) + "," + (c == obj ? "this" : "other") + "," + num(off(s)) + ");"; }
void frost_storm_construct(void*, Point3 p, int a, float b, int c, void* o, Point3 v) { calls += "frost(" + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + (o == obj ? "this" : "other") + "," + pt(v) + ");"; }
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long a2) {
    if (fw == 0xe0) { static int index; calls += "skill(" + num(off(adjusted)) + "," + num(a1) + "," + num(static_cast<int>(a2)) + ");"; return static_cast<unsigned>(10 * (static_cast<int>(a1) + 1) + (index++ & 0)); }
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(static_cast<long>(a1)) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned short get16(unsigned o) { unsigned short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(int state, unsigned k) {
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
    efResult = 0; oacResult = 1; advResult = 1; boolIndex = 0; intIndex = 0; rollIndex = 0; yawResult = 0x12345; deadResult = 0; allocResult = arena + 0x6000;
    worldDifficulty = 1.0f;
    std::memset(boolsList, 0, sizeof boolsList); std::memset(intsList, 0, sizeof intsList); std::memset(rolls, 0, sizeof rolls);
    put32(36, 0x300); put32(72, 0x900); putA(0x900 + 168, 8); putA(0x900 + 172, 0xa8); putA(0x900 + 176, 8); putA(0x900 + 180, 0xb0);
    putA(0x900 + 184, 16); putA(0x900 + 188, 0xb8);
    for (unsigned i = 0; i < 130; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    put16(572, static_cast<unsigned short>(state)); put32(960, k);
    putf(16, 4); putf(20, 5); putf(24, 6); putf(28, 100); putf(32, 40); put32(808, 5000); put32(256, 0);
    put16(56, 7);
    put32(576, 0x2800); putA(0x2800 + 12, 0x2900);                                  // node -> target object at 0x2900
    putf(0x2900 - 0x1000 + 0x1000 + 0, 0);
    std::memset(arena + 0x2900, 0, 0x80);
    float tp[3] = {50, 60, 70}; std::memcpy(arena + 0x2900 + 16, tp, 12); float tf = 8; std::memcpy(arena + 0x2900 + 28, &tf, 4);
    for (unsigned idx : {4u, 5u, 7u, 18u, 23u, 24u, 57u, 58u}) {
        const unsigned sk = 0x3000 + 0x100 * idx; skills[idx] = sk; putA(sk + 96, sk + 0x80); putA(sk + 0x80 + 104, 4); putA(sk + 0x80 + 108, 0xe0);
    }
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
static const std::string head = "ndh(8204);ef(8204);oac600;adv1;slide0.5;v168(4104,0,0);";
static const std::string headBusy = "ndh(8204);ef(8204);oac600;slide0.5;v168(4104,0,0);";
static std::string set(int n) { return "v184(" + num(0x1010) + ",1," + num(n) + ");"; }
int main() {
    int bad = 0;
    reset(2, 0); efResult = 1; msg_run__5Mummy(obj); bad |= check("everyFrame", "ndh(8204);ef(8204);");
    reset(2, 0); oacResult = 0; msg_run__5Mummy(obj); bad |= check("activation", "ndh(8204);ef(8204);oac600;");
    reset(2, 0); put32(1104, 5); msg_run__5Mummy(obj); bad |= check("busy", headBusy);
    reset(18, 0); msg_run__5Mummy(obj); bad |= check("18", head);
    // State 0 and 1.
    reset(0, 0); boolsList[1] = 1; msg_run__5Mummy(obj); bad |= check("0 second zone", head + "zone(400,32767);zone(500,16000);" + set(3));
    reset(0, 3); boolsList[2] = 1; msg_run__5Mummy(obj); bad |= check("0 zone2", head + "zone(400,32767);zone(500,16000);zone2(0,500,7,16000,0,1);" + set(3));
    reset(0, 3); msg_run__5Mummy(obj); bad |= check("0 timer", head + "zone(400,32767);zone(500,16000);zone2(0,500,7,16000,0,1);" + set(1));
    reset(0, 0); put32(256, 9000); msg_run__5Mummy(obj); bad |= check("0 idle", head + "zone(400,32767);zone(500,16000);");
    reset(1, 0); msg_run__5Mummy(obj); bad |= check("1 wander", head + "zone(400,32767);zone(500,16000);wander;");
    reset(1, 0); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("1 zone", head + "zone(400,32767);" + set(0));
    reset(9, 0); put32(804, 1); msg_run__5Mummy(obj); bad |= check("9", head + set(3));
    reset(8, 0); msg_run__5Mummy(obj); bad |= check("8", head + "move2(0:0:0);" + set(3));
    reset(8, 0); put16(294, 0x10); msg_run__5Mummy(obj); bad |= check("8 held", head + "move2(0:0:0);");
    reset(10, 0); put16(294, 0x80); msg_run__5Mummy(obj); bad |= check("10", head + "move2(0:0:0);rmrun;");
    // State 3.
    reset(3, 0); put32(576, 0); msg_run__5Mummy(obj); bad |= check("3 no target", head + set(0));
    reset(3, 1); put32(1112, 1); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("3 group zone", head + "zone(72,32767);" + set(15));
    reset(3, 1); put32(1112, 9000); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("3 group zone2", head + "zone2(0,400,7,6000,0,1);" + set(13));
    reset(3, 5); put32(1112, 9000); msg_run__5Mummy(obj); bad |= check("3 group steer", head + "zone2(0,400,7,6000,0,1);steer;");
    reset(3, 0); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("3 close", head + "zone(140,6000);" + set(6));
    reset(3, 4); boolsList[1] = 1; msg_run__5Mummy(obj); bad |= check("3 kind 4", head + "zone(140,6000);zone(100,4000);" + set(14));
    reset(3, 0); boolsList[1] = 1; rolls[0] = 7; msg_run__5Mummy(obj);
    bad |= check("3 crowded", head + "zone(140,6000);crowd(10496,500,300,0);rand;" + set(5));
    if (get32(256) != 5000 + 7 + 240) { std::puts("fail 3 crowded store"); bad = 1; }
    reset(3, 0); boolsList[2] = 1; msg_run__5Mummy(obj); bad |= check("3 face", head + "zone(140,6000);crowd(10496,500,300,0);zone(140,32767);turn;");
    reset(3, 0); intsList[0] = 2; intsList[1] = 0x4000; { unsigned short one = 1; std::memcpy(arena + 0x2900 + 56, &one, 2); } msg_run__5Mummy(obj);
    bad |= check("3 flank", head + "zone(140,6000);crowd(10496,500,300,0);zone(140,32767);rand;region(50:60:70,14);atan(55,46);" + set(17));
    reset(3, 0); rolls[0] = 1; msg_run__5Mummy(obj); bad |= check("3 steer", head + "zone(140,6000);crowd(10496,500,300,0);zone(140,32767);rand;steer;");
    // State 5.
    reset(5, 3); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("5 zone2", head + "zone2(0,400,7,6000,0,1);" + set(13));
    reset(5, 2); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("5 melee", head + "zone(140,6000);" + set(6));
    reset(5, 2); msg_run__5Mummy(obj); bad |= check("5 idle", head + "zone(140,6000);bexit;");
    reset(5, 2); boolsList[1] = 1; put32(256, 1); msg_run__5Mummy(obj); bad |= check("5 timer", head + "zone(140,6000);bexit;" + set(3));
    reset(5, 3); boolsList[1] = 1; put32(256, 9000); msg_run__5Mummy(obj); bad |= check("5 group face", head + "zone2(0,400,7,6000,0,1);bexit;turn;");
    reset(5, 2); boolsList[1] = 1; put32(256, 9000); msg_run__5Mummy(obj); bad |= check("5 crowd", head + "zone(140,6000);bexit;crowd(10496,500,300,1);" + set(3));
    // States 6 and 14.
    reset(6, 4); advResult = 1; put16(200, 4); put16(202, 5); put16(204, 6); put32(208, 3); rolls[0] = 0x1000; put16(294, 8); msg_run__5Mummy(obj);
    bad |= check("6 slam", head + "move2(0:0:0);rand;slam(10496,1);melee(36,8,10,6,8,1032);turn;");
    reset(14, 4); advResult = 1; put16(200, 4); put16(202, 5); put16(204, 6); put32(208, 3); rolls[0] = 0x9000; msg_run__5Mummy(obj);
    bad |= check("14 no slam", head + "move2(0:0:0);rand;melee(36,4,5,6,8,1032);turn;" + set(3));
    reset(6, 0); advResult = 0; put16(294, 8); msg_run__5Mummy(obj); bad |= check("6 no hit", head + "move2(0:0:0);turn;");
    // State 15: dust, return home, flag clear.
    reset(15, 0); advResult = 2; putf(776, 100); putf(780, 100); putf(784, 100); putf(788, 1); put16(294, 8); put32(576, 0); msg_run__5Mummy(obj);
    bad |= check("15 home", head + "sound(8636,this,1);grid;alloc88;devil(100:100:100);");
    reset(15, 0); advResult = 3; putf(776, 4); putf(780, 5); putf(784, 6); putf(788, 5); put16(294, 8); msg_run__5Mummy(obj);
    bad |= check("15 teleport", head + "alloc88;devil(4:5:6);sound(8636,this,1);tele(10512,400);alloc88;devil(4:5:6);");
    reset(15, 0); advResult = 0; put32(12, 0xffffffffu); msg_run__5Mummy(obj); bad |= check("15 clear", head + set(3));
    if (get32(12) != 0xfffffffcu) { std::puts("fail 15 clear store"); bad = 1; }
    // State 17: remembered position.
    reset(17, 0); msg_run__5Mummy(obj); bad |= check("17 steer", head + "zone(140,6000);rand;steer;");
    { float f; std::memcpy(&f, obj + 1116, 4); if (f != 50) { std::puts("fail 17 remembered x"); bad = 1; } }
    reset(17, 0); boolsList[0] = 1; msg_run__5Mummy(obj); bad |= check("17 close", head + "zone(140,6000);" + set(6));
    reset(17, 4); boolsList[1] = 1; msg_run__5Mummy(obj); bad |= check("17 kind 4", head + "zone(140,6000);zone(100,4000);" + set(14));
    reset(17, 0); put32(576, 0); msg_run__5Mummy(obj); bad |= check("17 no target", head + set(0));
    // State 13: casting.
    reset(13, 1); advResult = 1; rolls[0] = 1; msg_run__5Mummy(obj);
    bad |= check("13 fire storm", head + "move2(0:0:0);icos7;isin7;lead(400,154:80:6);seedead(300);zone(144,32767);rand;"
                 "alloc168;firestorm(1,this,154:80:12," + num(0x3000 + 0x100 * 7) + ");v176(4104,2,0);tf;termok;tf;termok;" + set(3));
    reset(13, 5); advResult = 1; rolls[0] = 2; msg_run__5Mummy(obj);
    bad |= check("13 sparks", head + "move2(0:0:0);icos7;isin7;lead(400,154:80:6);seedead(300);rand;skill(" + num(0x3000 + 0x100 * 57 + 4) + ",0,1);rand;skill(" + num(0x3000 + 0x100 * 57 + 4) + ",1,1);skill(" + num(0x3000 + 0x100 * 57 + 4) + ",2,1);skill(" + num(0x3000 + 0x100 * 57 + 4) + ",3,1);yaw(10496,40);icos74565;isin74565;alloc184;sparks(4:5:26,20:10:0,this,novag,10,10,20,360);sound(8620,this,1);v176(4104,2,0);tf;termok;tf;termok;" + set(3));
    reset(13, 3); advResult = 0; msg_run__5Mummy(obj);
    bad |= check("13 no cast", head + "move2(0:0:0);icos7;isin7;lead(400,154:80:6);v176(4104,2,0);tf;termok;tf;termok;" + set(3));
    reset(13, 0); put32(576, 0); put32(256, 9000); put32(1108, 9000); msg_run__5Mummy(obj); bad |= check("13 no target", head + "move2(0:0:0);zone(72,32767);");
    reset(13, 3); advResult = 1; deadResult = reinterpret_cast<unsigned long>(arena + 0x2a00); putA(0x2a00 + 72, 0x2b00); putA(0x2b00 + 184, 4); putA(0x2b00 + 188, 0xd0); rolls[0] = 0x1000; msg_run__5Mummy(obj);
    bad |= check("13 heal", head + "move2(0:0:0);icos7;isin7;lead(400,154:80:6);seedead(300);rand;v208(" + num(0x2a00 + 4) + ",1,16);lump(fx.lmp,spel_he_heal.anm);sound(" + num(0x7100) + ",other,1);v176(4104,2,0);tf;termok;tf;termok;" + set(3));
    return bad;
}
