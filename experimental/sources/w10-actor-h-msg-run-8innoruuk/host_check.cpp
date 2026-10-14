// Host contract for Innoruuk msg_run; not an EE input. Words are offsets from a host base; table
// function words 1..5 select the stubs for the model getter (slot 128), slots 168 and 176, the
// state setter (slot 184) and the held object's destructor. Every callee records a short token; the
// checks compare the token strings of chosen paths and a few stored values.
#define W10_ACTOR_H_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10h_host_base;
static void* getModel(void*);
static void slot168(void*);
static void slot176(void*, int, void*, void*);
static void setStateStub(void*, int);
static void destroyStub(void*, int);
void* w10h_host_fn(unsigned w) {
    switch (w) {
        case 1: return reinterpret_cast<void*>(&getModel);
        case 2: return reinterpret_cast<void*>(&slot168);
        case 3: return reinterpret_cast<void*>(&slot176);
        case 4: return reinterpret_cast<void*>(&setStateStub);
        case 5: return reinterpret_cast<void*>(&destroyStub);
    }
    return nullptr;
}
extern "C" {
signed char netNetworkMode = 0;
int frameNm = 0;
void* gameCam = reinterpret_cast<void*>(0x66);
void* haloTexture1 = nullptr;
void* haloTexture2 = nullptr;
unsigned skills[64];
unsigned innoruukResourceMap[60];
unsigned char innoruukParticle2[16], innoruukParticle3[16], summonhammerParticle[16], shockwaveInnoruukParticle[16], shockwaveInnoruukParticle2[16];
char levelName[322];
}
#include "msg_run.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const O = arena + 0x2000;
static std::string calls; static std::vector<unsigned> rolls; static size_t rollIndex;
static int flagsResult, everyResult, zone1, exitResult; static std::vector<int> zone2, alt; static size_t zone2i, alti;
static int lastState = -1, particles, lights, soulCount, flashes, shocks;
static std::vector<float> cameraShakes; static std::vector<std::string> sounds;
static std::string num(long v) { return std::to_string(v); }
static void* getModel(void*) { calls += "G;"; return arena + 0x7000; }
static void slot168(void*) { calls += "V168;"; }
static void slot176(void*, int a, void*, void* t) { calls += "V176;"; (void)a; (void)t; }
static void setStateStub(void*, int n) { calls += "S" + num(n) + ";"; lastState = n; }
static void destroyStub(void*, int n) { calls += "D" + num(n) + ";"; }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(void*, void*, void*, const void*, short, void*, void*, float, int, int, float, void*) { calls += "W;"; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(void*, void*, int, void* p, float*) { calls += "H;"; float v[3] = {3, 4, 5}; std::memcpy(p, v, 12); }
void lightUpdatePosition__FP5Light(void*) { calls += "L;"; }
void newDamageHandler__8CreatureP12_modelHeader(void*, void*) { calls += "N;"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "E;"; return everyResult; }
void objectActivationCheck__8Creaturei(void*, int) { calls += "A;"; }
int creatureAdvanceAnimation__8Creaturef(void*, float) { calls += "C;"; return flagsResult; }
void slideBack__8Creaturef(void*, float) { calls += "B;"; }
int enemyInZone__8Creaturefs(void*, float, short) { calls += "z;"; return zone1; }
int enemyInZone2__8Creatureffssfi(void*, float, float, short, short, float, int) { calls += "Z;"; return zone2.empty() ? 0 : zone2[zone2i++ % zone2.size()]; }
int alternateEnemyInZone__8Creatureffss(void*, float, float, short, short) { calls += "a;"; return alt.empty() ? 0 : alt[alti++ % alt.size()]; }
void smartWander__8Creature(void*) { calls += "w;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "r;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "x;"; return exitResult; }
void move2__8CreatureG6Point3(void*, const void*) { calls += "m;"; }
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void*, float, float, short, short, int, int, int, int mask, void*) { calls += "d" + num(mask) + ";"; }
void gameSetCameraShake__Ffb(float amount, char) { calls += "k;"; cameraShakes.push_back(amount); }
float icos__Fi(int angle) { return std::cos(angle * 3.14159265f / 32768.0f); }
float isin__Fi(int angle) { return std::sin(angle * 3.14159265f / 32768.0f); }
void* blockAlloc__Fi(int size) { calls += "b" + num(size) + ";"; static unsigned char pool[8][700]; static int next; return pool[next++ & 7]; }
void light_effect_construct(void*, const void*, float, float, float, float, float, float, float, float, float, float, float, float, void*, int) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void light_effect_construct(void*, const void*, float, float, float, float, float, float, float, float, float, float, float, float, void*, int) { calls += "F;"; ++lights; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, void*, void*, float) { ++particles; }
void turnToFaceEnemy__8Creature(void*) { calls += "t;"; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float) { calls += "P;"; return 7; }
void soundStopSound__Fi(int) { calls += "Q;"; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void*, float, float, int, int, int, int, void*, short, int) { calls += "f;"; }
void hate_beam_construct(void*, int, const void*, const void*, void*, void*) __asm__("__8HateBeamiG6Point3T2P8CreatureP5Skill");
void hate_beam_construct(void*, int, const void*, const void*, void*, void*) { calls += "J;"; }
void hate_projectile_construct(void*, const void*, short, void*, void*, int, void*) __asm__("__14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader");
void hate_projectile_construct(void*, const void*, short, void*, void*, int, void*) { calls += "j;"; }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float, void*) { calls += "l;"; }
int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void*, float) { calls += "y;"; return 1000; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void*, const void*, short, void*, int, void* out, int, float) { calls += "c;"; float v[3] = {7, 8, 9}; std::memcpy(out, v, 12); }
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(void*, const void*, short, void*, int, void*, float) { calls += "M;"; }
void phprop_construct(void*, const void*, void*, char*, char*, char*, long, int, void*, float) __asm__("__6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf");
void phprop_construct(void*, const void*, void*, char*, char*, char*, long, int flag, void*, float) { calls += "O" + num(flag) + ";"; }
void* listOnewayLink__FP8ListHeadT0Pv(void*, void*, void*) { calls += "K;"; return nullptr; }
void listDestroy__FP8ListHead(void*) { calls += "X;"; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { calls += "n;"; return 2; }
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(void*, void*, const void*, short, void*, float, float, float, unsigned) { ++shocks; }
void hate_soul_construct(void*, const void*, const void*, int, int, int) __asm__("__8HateSoulG6Point3T1iii");
void hate_soul_construct(void*, const void*, const void*, int, int, int) { ++soulCount; }
void engineSetScreenColor__Fiiii(int, int, int, int) { ++flashes; }
void deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff(void*, void*, void*, void*, const void*, short, float, float) { calls += "T;"; }
int P_StartTrail__Fiifiiiii(int, int, float, int, int, int, int, int) { calls += "R;"; return 99; }
void Pop__6Camera(void*) { calls += "q;"; }
void P_RevolveTrail__FiG6Point3N31i(int, const void*, const void*, const void*, const void*, int) { calls += "v;"; }
void P_DeleteTrail__Fi(int) { calls += "u;"; }
long long litodp(int a) { double d = a; long long b; std::memcpy(&b, &d, 8); return b; }
long long dpmul(long long a, long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double d = x * y; long long r; std::memcpy(&r, &d, 8); return r; }
int dpcmp(long long a, long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); return x < y ? -1 : (x == y ? 0 : 1); }
void MC_SaveCheckPoint__Fiii(int, int, int) { calls += "!;"; }
void gameNetKick__Fi(int) { calls += "?;"; }
void netDisconnect__Fv() { calls += "-;"; }
void scriptSetGameState__FPci(char*, int v) { calls += "s" + num(v) + ";"; }
void soundFadeStream__Fi(int) { calls += "e;"; }
int objectRemoveFromRunList__FP10GameObject(void*) { calls += "o;"; return 0; }
int eRand__10GameObject(void*) { return static_cast<int>(rolls.at(rollIndex++ % rolls.size())); }
void* lumpFindResource__FPcT0(char*, char*) { return arena + 0x7100; }
}
static void put_word(unsigned off, unsigned v) { std::memcpy(O + off, &v, 4); }
static unsigned get_word(unsigned off) { unsigned v; std::memcpy(&v, O + off, 4); return v; }
static void put_half(unsigned off, short v) { std::memcpy(O + off, &v, 2); }
static short get_half(unsigned off) { short v; std::memcpy(&v, O + off, 2); return v; }
static void put_float(unsigned off, float v) { std::memcpy(O + off, &v, 4); }
static float get_float(unsigned off) { float v; std::memcpy(&v, O + off, 4); return v; }
static void reset(int state) {
    std::memset(arena, 0, sizeof arena);
    calls.clear(); rolls = {40000, 30000, 36000, 20000, 50000, 33000}; rollIndex = 0; flagsResult = 0; everyResult = 0; zone1 = 0; exitResult = 0; zone2.clear(); alt.clear(); zone2i = alti = 0;
    particles = lights = soulCount = flashes = shocks = 0; cameraShakes.clear(); lastState = -1; frameNm = 1; netNetworkMode = 0;
    put_word(36, 0x7200); put_word(72, 0x300);
    const unsigned slots[] = {128, 168, 176, 184};
    for (int i = 0; i < 4; ++i) { short zero = 0; std::memcpy(arena + 0x300 + slots[i], &zero, 2); unsigned w = i + 1; std::memcpy(arena + 0x300 + slots[i] + 4, &w, 4); }
    short zero = 0; std::memcpy(arena + 0x300 + 8, &zero, 2); unsigned dw = 5; std::memcpy(arena + 0x300 + 12, &dw, 4);
    put_half(572, static_cast<short>(state)); put_word(808, 1000);
    put_float(28, 5.0f); put_float(224, 2.0f);
    // held objects: the heads at +576 and +988 point at nodes whose word at +12 is an object
    std::memset(arena + 0x4000, 0, 0x400);
    put_word(1004, 0);
    skills[27] = 0x400; skills[28] = 0x440; innoruukResourceMap[3] = 0x500;
}
static void holdTarget() { put_word(576, 0x3000); unsigned obj = 0x3400; std::memcpy(arena + 0x3000 + 12, &obj, 4); std::memcpy(arena + 0x3400 + 72, &(const unsigned&)(0x300u), 4); float f = 20.0f; std::memcpy(arena + 0x3400 + 28, &f, 4); }
static int count(const std::string& s, const std::string& token) { int n = 0; for (size_t at = s.find(token); at != std::string::npos; at = s.find(token, at + token.size())) ++n; return n; }
int main() {
    int bad = 0;
    w10h_host_base = arena;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n   calls %s\n", what, calls.c_str()); bad = 1; } };
    const std::string prefix = "W;H;L;N;E;A;C;B;V168;";
    // prologue and early exit
    reset(0); everyResult = 1; msg_run__8Innoruuk(O); expect(calls == "W;H;L;N;E;", "everyFrame returning nonzero ends the frame");
    reset(2); msg_run__8Innoruuk(O); expect(calls == prefix, "states without behaviour only run the prologue");
    reset(22); msg_run__8Innoruuk(O); expect(calls == prefix, "state 22 and above do nothing more");
    reset(0); put_float(1076, 100.0f); msg_run__8Innoruuk(O);
    expect(std::fabs(get_float(1076) - 98.2f) < 1e-3f && get_float(1044) == get_float(1076) && get_float(1052) == get_float(1076) && get_float(1056) == 1.0f / (get_float(1076) * get_float(1076)) && get_float(1048) == get_float(1056), "light size decays by 1.8 and feeds the light");
    reset(0); put_float(1076, 1.0f); msg_run__8Innoruuk(O); expect(get_float(1076) == 0.0f, "size is held at zero");
    { float p[3]; std::memcpy(p, O + 1064, 12); expect(p[0] == 3 && p[1] == 4 && p[2] == 5, "shadow point copied to +1064"); }
    // state 0 and 1
    reset(0); zone1 = 1; msg_run__8Innoruuk(O); expect(calls == prefix + "z;S3;", "state 0: enemy in zone gives state 3");
    reset(0); put_word(256, 5); put_word(808, 10); msg_run__8Innoruuk(O); expect(calls == prefix + "z;S1;", "state 0: timer gives state 1");
    reset(0); put_word(256, 50); put_word(808, 10); msg_run__8Innoruuk(O); expect(calls == prefix + "z;", "state 0: nothing");
    reset(1); put_word(256, 50); put_word(808, 10); msg_run__8Innoruuk(O); expect(calls == prefix + "z;w;", "state 1: wander");
    reset(1); put_word(256, 5); put_word(808, 10); zone1 = 1; msg_run__8Innoruuk(O); expect(calls == prefix + "z;S3;S0;", "state 1: set state 3, then state 0 from the timer");
    // state 5, 14, 19
    reset(5); exitResult = 1; msg_run__8Innoruuk(O); expect(calls == prefix + "x;S3;", "state 5 exit");
    reset(14); put_word(256, 5); put_word(808, 10); msg_run__8Innoruuk(O); expect(calls == prefix + "S3;", "state 14");
    reset(19); msg_run__8Innoruuk(O); expect(calls == prefix + "S3;", "state 19 with the animation not done");
    reset(19); put_half(294, 8); msg_run__8Innoruuk(O); expect(calls == prefix, "state 19 with the animation done");
    // state 3 with no head and the animation checks
    reset(3); put_word(960, 1); msg_run__8Innoruuk(O); expect(calls == prefix + "S0;", "state 3: no target and the flag word set gives state 0");
    reset(3); msg_run__8Innoruuk(O); expect(calls == prefix + "S20;", "state 3: no target and the flag word clear gives state 20");
    // state 21: ring of 60 particles and one light effect
    reset(21); flagsResult = 1; holdTarget(); msg_run__8Innoruuk(O);
    expect(particles == 60 && lights == 1 && count(calls, "b188;") == 1 && count(calls, "d524289;") == 1 && count(calls, "m;") == 1, "state 21: shock effect and damage with mask 0x80001");
    expect(!cameraShakes.empty() && cameraShakes[0] == 22.0f, "state 21 camera shake 22.0");
    // state 17 differs in the damage amount only through its argument (mask the same)
    reset(17); flagsResult = 1; holdTarget(); msg_run__8Innoruuk(O); expect(particles == 60 && count(calls, "d524289;") == 1, "state 17 damage");
    // state 18: sound, 60 particles, camera shake and a mask chosen by the draw
    reset(18); flagsResult = 1; holdTarget(); rolls = {10}; msg_run__8Innoruuk(O);
    expect(particles == 60 && count(calls, "d262145;") == 1 && cameraShakes.size() >= 1 && cameraShakes[0] == 14.0f && get_word(968) == 0, "state 18 draw below 50: mask 0x40001, shake 14.0");
    // state 20 particles: 80 for the two attachments
    reset(20); msg_run__8Innoruuk(O); expect(particles == 80, "state 20 draws 40 particles for each attachment point");
    // state 10 timeline
    reset(10); put_word(1000, 1000); put_word(808, 1000 + 3061); put_word(960, 0); msg_run__8Innoruuk(O);
    expect(count(calls, "T;") == 1 && count(calls, "R;") == 1 && get_word(1008) == 99 && (get_word(12) & 3) == 3, "state 10 at tick 3061: death effect and trail start");
    reset(10); put_word(1000, 1000); put_word(808, 1000 + 3062); msg_run__8Innoruuk(O); expect(count(calls, "q;") == 1 && count(calls, "v;") == 1, "state 10 at tick 3062: camera pop, trail revolve");
    reset(10); put_word(1000, 1000); put_word(808, 1000 + 3125); msg_run__8Innoruuk(O); expect(count(calls, "u;") == 1 && count(calls, "v;") == 0, "state 10 at tick 3125: trail deleted");
    reset(10); put_word(1000, 1000); put_word(808, 1000 + 4500); netNetworkMode = 1; msg_run__8Innoruuk(O);
    expect(count(calls, "!;") == 1 && count(calls, "?;") == 1 && count(calls, "-;") == 1 && count(calls, "o;") == 1 && count(calls, "s-2;") == 1 && count(calls, "e;") == 1 && std::strcmp(levelName, "actintro") == 0 && levelName[80] == -1, "state 10 at tick 4500: final steps");
    reset(10); put_word(1000, 1000); put_word(808, 1000 + 2000); put_word(808, 47 * 40); put_word(1000, 47 * 40 - 2000); msg_run__8Innoruuk(O); expect(soulCount >= 1, "state 10 at tick 2000: a soul on the 47 cadence");
    reset(10); put_word(1000, 0); put_word(808, 2900); frameNm = 1; msg_run__8Innoruuk(O); expect(particles >= 10 && get_word(1000) == 0, "state 10 at tick 2900: ten particles");
    reset(10); put_word(1000, 0); put_word(808, 1500); msg_run__8Innoruuk(O); expect(get_half(140) == 115 && get_half(142) == 40 && get_half(146) == 100 && get_half(148) == 64 && get_half(154) == -64, "state 10 at tick 1500: shock colours");
    if (!bad) std::puts("innoruuk msg_run ok");
    return bad;
}
