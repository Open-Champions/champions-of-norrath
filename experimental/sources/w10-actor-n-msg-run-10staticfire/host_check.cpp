// Host contract for msg_run__10StaticFire; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (400 random-state scenarios; see the notes file for the coverage). Float formulas are checked
// against recomputation in single precision.
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_run.cpp"

char levelName[512];
char cutMode;
int frameNm;
unsigned char norrath_w10_actor_n_view_8d63c0[8], norrath_w10_actor_n_view_8d6460[8];
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static unsigned rollIndex;
static unsigned rolls[256];
static int visible;
static void* foundList[65];
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 255]; }
void lightDeactivate__FP5Light(void* l) { calls += "lightoff" + num(static_cast<unsigned char*>(l) - obj) + ";"; }
void newWind__10StaticFirei(void*, int a) { calls += "wind" + num(a) + ";"; }
void addSmokelet__10StaticFire(void*) { calls += "smoke;"; }
void updateSmokelets__10StaticFire(void*) { calls += "upd;"; }
int gamePositionVisible__FP6Point3f(Point3* p, float r) { calls += "vis(" + num(p->x) + "," + num(r) + ");"; return visible; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) {
    calls += std::string(d == norrath_w10_actor_n_view_8d63c0 ? "fire" : d == norrath_w10_actor_n_view_8d6460 ? "spark" : "?") + "(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," +
             num(v->x) + ":" + num(v->y) + ":" + num(v->z) + "," + num(f) + ");";
}
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** out, int n) {
    calls += "find(" + num(lo.x) + ":" + num(lo.y) + ":" + num(lo.z) + "," + num(hi.x) + ":" + num(hi.y) + ":" + num(hi.z) + "," + num(n) + ");";
    int i = 0;
    for (; foundList[i]; ++i) out[i] = foundList[i];
    out[i] = nullptr;
    return 0;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float f, float g, int c, int d, int e, int h, void* desc, bool z) {
    const unsigned char* q = static_cast<const unsigned char*>(desc);
    short m0, m1, m2, m3; std::memcpy(&m0, q + 36, 2); std::memcpy(&m1, q + 44, 2); std::memcpy(&m2, q + 52, 2); std::memcpy(&m3, q + 60, 2);
    short a38, a40, a42; std::memcpy(&a38, q + 38, 2); std::memcpy(&a40, q + 40, 2); std::memcpy(&a42, q + 42, 2);
    calls += "attack(" + std::string(a == arena + 0x7000 ? "other" : a == obj ? "SELF" : "x") + "," + std::string(b == obj ? "self" : "?") + "," + num(f) + "," + num(g) + "," +
             std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(h) + ",desc" + num(m0) + "/" + num(m1) + "/" + num(m2) + "/" + num(m3) + "/" + num(a38) + "/" + num(a40) + "/" + num(a42) +
             "/" + num(q[32]) + "/" + num(q[33]) + (z ? ",true" : ",false") + ");";
}
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
}
static void put32(unsigned a, unsigned v) { std::memcpy(obj + a, &v, 4); }
static void putf(unsigned a, float v) { std::memcpy(obj + a, &v, 4); }
static void put16(unsigned a, int v) { short s = static_cast<short>(v); std::memcpy(obj + a, &s, 2); }
static float getf(unsigned a) { float v; std::memcpy(&v, obj + a, 4); return v; }
static unsigned get32(unsigned a) { unsigned v; std::memcpy(&v, obj + a, 4); return v; }
static void reset() {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear(); rollIndex = 0;
    for (unsigned i = 0; i < 256; ++i) rolls[i] = 1 + 7 * i;          // never a multiple of 10, 14 or 33 at the points that matter
    visible = 0; cutMode = 0; frameNm = 1;
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, "faydark2");
    put32(124, 0xffffffffu);              // countdown off
    obj[108] = 1; obj[110] = 0;
    putf(100, 0.5f); putf(96, 80.0f); putf(84, 1.0f); putf(28, 2.0f); putf(80, 1.0f);
    putf(16, 10); putf(20, 20); putf(24, 30);
    foundList[0] = nullptr;
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    // Byte +109 follows the level name and cutMode.
    reset(); msg_run__10StaticFire(obj); if (obj[109] != 0) { std::puts("fail 109 other level"); bad = 1; }
    reset(); std::strcpy(levelName + 81, "faydark3"); msg_run__10StaticFire(obj); if (obj[109] != 0) { std::puts("fail 109 cut off"); bad = 1; }
    reset(); std::strcpy(levelName + 81, "faydark3"); cutMode = 3; msg_run__10StaticFire(obj); if (obj[109] != 1) { std::puts("fail 109 cut on"); bad = 1; }
    reset(); std::strcpy(levelName + 81, "faydark3"); cutMode = 3; obj[109] = 7; std::strcpy(levelName + 81, "other"); msg_run__10StaticFire(obj); if (obj[109] != 0) { std::puts("fail 109 cleared"); bad = 1; }
    // Countdown: -1 leaves it alone; 0 switches the light off; 1 -> 0 switches it off; otherwise it counts down and r follows.
    reset(); msg_run__10StaticFire(obj); check("quiet", "rand;upd;"); if (get32(124) != 0xffffffffu) { std::puts("fail countdown -1"); bad = 1; }
    reset(); put32(124, 0); msg_run__10StaticFire(obj); check("zero", "lightoff128;upd;delete;");
    reset(); put32(124, 1); msg_run__10StaticFire(obj); check("one", "lightoff128;upd;delete;"); if (get32(124) != 0) { std::puts("fail countdown 1"); bad = 1; }
    reset(); put32(124, 50); obj[108] = 1; msg_run__10StaticFire(obj);
    if (get32(124) != 49 || getf(100) < 0.4916f || getf(100) > 0.4917f) { std::printf("fail countdown 50 r=%g\n", getf(100)); bad = 1; }
    reset(); put32(124, 50); putf(100, 0.004f); msg_run__10StaticFire(obj); if (getf(100) != 0.0f) { std::puts("fail clamp at 0"); bad = 1; }
    reset(); put32(124, 200); putf(100, 0.5f); msg_run__10StaticFire(obj); if (get32(124) != 199 || getf(100) < 0.5166f || getf(100) > 0.5167f) { std::printf("fail rise r=%g\n", getf(100)); bad = 1; }
    reset(); put32(124, 200); putf(100, 0.99f); msg_run__10StaticFire(obj); if (getf(100) != 1.0f) { std::puts("fail clamp at 1"); bad = 1; }
    reset(); put32(124, 200); putf(100, 0.5f); obj[109] = 0; std::strcpy(levelName + 81, "faydark3"); cutMode = 1; msg_run__10StaticFire(obj); if (getf(100) != 1.0f) { std::puts("fail cut forces 1"); bad = 1; }
    // Wind timer (byte +108 clear): the float at +76 gains 1; above 60 it triggers newWind with a roll and clears the word.
    reset(); obj[108] = 0; putf(76, 61.0f); msg_run__10StaticFire(obj); check("wind", "rand;wind1;rand;upd;"); if (get32(76) != 0) { std::puts("fail wind word"); bad = 1; }
    reset(); obj[108] = 0; putf(76, 60.0f); msg_run__10StaticFire(obj); check("wind wait", "rand;upd;"); if (getf(76) != 61.0f) { std::puts("fail wind step"); bad = 1; }
    // Smoke: only with a live countdown and a roll that is a multiple of 14.
    reset(); put32(124, 50); rolls[0] = 28; msg_run__10StaticFire(obj); check("smoke", "rand;smoke;upd;");
    if (getf(88) != 1.0f * 0.45f * 16.0f && (getf(88) < 7.19f || getf(88) > 7.21f)) { std::puts("fail smoke size"); bad = 1; }
    reset(); put32(124, 50); rolls[0] = 29; msg_run__10StaticFire(obj); check("no smoke", "rand;upd;");
    // A visible, lit fire runs the glow and the particle loops; with the countdown at 0 it deletes only when the ring bounds match.
    reset(); put32(124, 50); obj[110] = 1; visible = 1; putf(100, 0.0f); frameNm = 1; msg_run__10StaticFire(obj);
    if (calls.find("vis(10,941);") == std::string::npos) { std::puts("fail visibility check"); bad = 1; }
    reset(); put32(124, 0); put16(120, 3); put16(122, 3); msg_run__10StaticFire(obj); check("delete", "lightoff128;upd;delete;");
    reset(); put32(124, 0); put16(120, 3); put16(122, 4); msg_run__10StaticFire(obj); check("no delete", "lightoff128;upd;");
    // Attack scan only when frameNm & 7 == 0: list gives other (hit), the object itself (skipped).
    reset(); put32(124, 50); obj[110] = 1; visible = 1; putf(100, 0.5f); putf(84, 0.1f); putf(96, 80.0f); frameNm = 8; foundList[0] = arena + 0x7000; foundList[1] = obj; foundList[2] = nullptr;
    rolls[0] = 1; msg_run__10StaticFire(obj);
    if (calls.find(",64);attack(other,self,3,3,100000,0,0,16826432,desc21/-1/-1/-1/1/1/0/255/255,false);") == std::string::npos || calls.find("find(9.01667:19.0167:29.0167,10.9833:20.9833:30.9833,64)") == std::string::npos) { std::printf("fail attack scan:\n  %s\n", calls.c_str()); bad = 1; }
    if (calls.find("SELF") != std::string::npos) { std::puts("fail: attacked itself"); bad = 1; }
    return bad;
}
