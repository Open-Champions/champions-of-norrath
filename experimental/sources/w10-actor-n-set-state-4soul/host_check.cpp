// Host contract for setState__4Souli; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (500 random-state scenarios over states -7..28; the matrix-carrying branch of state 23 can only run
// with a non-zero word at +960, which the early check rejects for state 23, so no run reaches it).
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "set_state.cpp"

char levelName[512];
unsigned char norrath_w10_actor_n_view_7d6d80[8];
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000, shadowCount;
static unsigned rollValue;
static int rollMode;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int strcasecmp(const char* a, const char* b) {
    for (;; ++a, ++b) {
        int x = *a >= 'A' && *a <= 'Z' ? *a + 32 : *a, y = *b >= 'A' && *b <= 'Z' ? *b + 32 : *b;
        if (x != y) return x - y;
        if (!x) return 0;
    }
}
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
unsigned eRand__10GameObject(void*) {
    calls += "rand;";
    if (rollMode == 0) return rollValue;
    static const unsigned ball[] = {16384, 16384, 0, 65535, 65535, 65535, 32768, 40000, 20000};   // the second triple is rejected, the third accepted
    return ball[rollMode++ - 1 < 9 ? rollMode - 2 : 8];
}
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) { calls += "trans(" + num(off(h) - 0x2000) + "," + num(t) + "," + num(f) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) { calls += "shot(" + num(off(h) - 0x2000) + "," + num(a) + "," + num(b) + "," + num(f) + ");"; }
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void listDestroy__FP8ListHead(void* h) { calls += "ld" + num(static_cast<unsigned char*>(h) - obj) + ";"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { calls += "points(" + num(off(m)) + ");"; return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* an, Point3 p, short a, int i, Point3* out, float* sc, void* mat, float f) {
    calls += "shadow(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(i) + "," + (mat ? "mat" : "null") + "," + num(f) + ");";
    *out = Point3{static_cast<float>(10 * (i + 1)), 5.0f, 6.0f};
}
void getSoulAttachMatrix__6MinionP8Matrix34(void*, float*) { calls += "UNEXPECTED soul matrix;"; }
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void*, void*, int, float*, float) { calls += "UNEXPECTED attach matrix;"; }
void matInvert__FP8Matrix34T0(float*, float*) { calls += "UNEXPECTED invert;"; }
void matMul__FP8Matrix34N20(float*, float*, float*) { calls += "UNEXPECTED mul;"; }
void matTransform__FP6Point3P8Matrix34T0(Point3*, float*, Point3*) { calls += "UNEXPECTED transform;"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) {
    calls += std::string(d == norrath_w10_actor_n_view_7d6d80 ? "soul" : "?") + "(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + "," + num(f) + ");";
}
}
unsigned w10_actor_n_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");"; return fw == 0x80 ? 0x5000 : 0;
}
static void put32(unsigned a, unsigned v) { std::memcpy(obj + a, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned a) { unsigned v; std::memcpy(&v, obj + a, 4); return v; }
static short get16(unsigned a) { short v; std::memcpy(&v, obj + a, 2); return v; }
static void put16(unsigned a, int v) { short s = static_cast<short>(v); std::memcpy(obj + a, &s, 2); }
static void reset(int current) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; rollValue = 1000; rollMode = 0; shadowCount = 2;
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, "elsewhere");
    put32(36, 0x300); put32(72, 0x800);
    putA(0x800 + 128, 8); putA(0x800 + 132, 0x80); putA(0x800 + 184, 8); putA(0x800 + 188, 0xb8); putA(0x800 + 192, 0); putA(0x800 + 196, 0xc0);
    for (unsigned i = 0; i < 200; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);
    put16(572, current); put32(808, 5000);
    float f = 2.5f; std::memcpy(obj + 224, &f, 4);
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    const std::string t1 = ",3);", s8 = ",8);";
    reset(0); setState__4Souli(obj, 0); check("0", p(0) + "rand;trans(60,0.3,3);"); if (get32(256) != 5000u + 1000u % 120u + 60u) { std::puts("fail 0 word"); bad = 1; }
    reset(0); setState__4Souli(obj, 1); check("1", p(1) + "reset;smart200;trans(76,0.3,2051);sync;rand;"); if (get32(256) != 5000u + 1000u % 120u + 300u) { std::puts("fail 1 word"); bad = 1; }
    for (int s : {5, 13}) { reset(0); setState__4Souli(obj, s); check("5/13", p(s) + "trans(52,0.3,3);"); }
    reset(0); setState__4Souli(obj, 14); check("14", p(14) + "trans(68,0.3,3);rand;"); if (get32(256) != 5000u + 1000u % 120u) { std::puts("fail 14 word"); bad = 1; }
    reset(14); setState__4Souli(obj, 15); check("15 at 14", p(15) + "shot(44,0.1,0.1,16);"); if (get16(572) != 15) { std::puts("fail 15 state"); bad = 1; }
    reset(3); setState__4Souli(obj, 15); check("15 else", p(15)); if (get16(572) != 3) { std::puts("fail 15 else state"); bad = 1; }
    reset(14); setState__4Souli(obj, 8); check("8 at 14", p(8) + "v184(8,1,15);");
    reset(3); setState__4Souli(obj, 8); check("8 else", p(8) + "v184(8,1,14);");
    reset(3); setState__4Souli(obj, 9); check("9", p(9)); if (get16(572) != 3) { std::puts("fail 9 state"); bad = 1; }
    reset(0); setState__4Souli(obj, 10); check("10", p(10) + "trans(140,0.1,6);v192(0,0,0);"); if (get32(256) != 5180u || get32(12) != 0x4000u) { std::puts("fail 10 words"); bad = 1; }
    reset(0); setState__4Souli(obj, 16); check("16", p(16) + "shot(84,0.1,0.1,8);");
    reset(0); put32(960, 0); setState__4Souli(obj, 17); check("17", p(17) + "ld960;"); if (get32(960) != 0 || get32(256) != 5240u) { std::puts("fail 17 words"); bad = 1; }
    reset(0); setState__4Souli(obj, 18); check("18", p(18) + "shot(116,0.1,0.1,12);");
    reset(0); setState__4Souli(obj, 19); check("19", p(19) + "shot(92,0.5,0.5,8);");
    reset(0); setState__4Souli(obj, 20); check("20", p(20) + "trans(108,0.1,3);");
    reset(0); setState__4Souli(obj, 21); check("21", p(21) + "trans(124,0.3,3);");
    reset(0); setState__4Souli(obj, 22); check("22", p(22) + "shot(132,0.1,0.1,8);");
    reset(0); setState__4Souli(obj, 24); check("24", p(24) + "trans(148,0.1,6);"); if (get32(256) != 5300u || get32(12) != 9u) { std::puts("fail 24 words"); bad = 1; }
    // 23: one pass per shadow point, two particles each from a rejection-sampled direction with z cleared
    reset(0); shadowCount = 1; rollMode = 1; setState__4Souli(obj, 23);
    {
        size_t first = calls.find("soul("), second = calls.find("soul(", first + 1);
        if (calls.find("points(20480);v128(8,0,0);shadow(20480,0:0:0,-16384,0,null,2.5);") == std::string::npos || first == std::string::npos || second == std::string::npos ||
            calls.find("soul(", second + 1) != std::string::npos || calls.find(":0,0);") == std::string::npos || calls.find("soul(10:5:6,") == std::string::npos ||
            calls.substr(calls.size() - 7) != "delete;" || get32(12) != 0x4002u || get16(572) != 23) { std::printf("fail 23\n  %s\n", calls.c_str()); bad = 1; }
    }
    // two shadow points: the second pass uses index 1 and the point moved by the callee
    reset(0); shadowCount = 2; rollMode = 1; setState__4Souli(obj, 23);
    if (calls.find("shadow(20480,0:0:0,-16384,1,null,2.5);") == std::string::npos || calls.find("soul(20:5:6,") == std::string::npos) { std::printf("fail 23 two\n  %s\n", calls.c_str()); bad = 1; }
    reset(0); shadowCount = 0; setState__4Souli(obj, 23); check("23 none", p(23) + "v128(8,0,0);points(20480);delete;"); if (get32(12) != 0x4002u) { std::puts("fail 23 none word"); bad = 1; }
    // The level named airhub forces state 24 regardless of the argument.
    reset(0); std::strcpy(levelName + 81, "AirHub"); setState__4Souli(obj, 0); check("airhub", p(24) + "trans(148,0.1,6);"); if (get16(572) != 24) { std::puts("fail airhub state"); bad = 1; }
    // Nothing happens and nothing is stored when state 17 is current.
    reset(17); setState__4Souli(obj, 0); check("current 17", p(0)); if (get16(572) != 17) { std::puts("fail 17 kept"); bad = 1; }
    // A non-zero word at +960 only lets states 16..22 through.
    reset(0); put32(960, 0x500); setState__4Souli(obj, 0); check("960 blocks", p(0)); if (get16(572) != 0) { std::puts("fail 960 stored"); bad = 1; }
    reset(0); put32(960, 0x500); setState__4Souli(obj, 23); check("960 blocks 23", p(23));
    reset(0); put32(960, 0x500); setState__4Souli(obj, 16); check("960 lets 16", p(16) + "shot(84,0.1,0.1,8);");
    reset(0); put32(960, 0x500); setState__4Souli(obj, 22); check("960 lets 22", p(22) + "shot(132,0.1,0.1,8);");
    // The Creature step ends the call or changes n; unknown states are just stored.
    reset(5); procResult = 1; setState__4Souli(obj, 0); check("proc ends", p(0)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    reset(5); procNew = 16; setState__4Souli(obj, 99); check("proc changes", p(99) + "shot(84,0.1,0.1,8);"); if (get16(572) != 16) { std::puts("fail changed state"); bad = 1; }
    for (int s : {2, 3, 4, 6, 7, 11, 12, 25, 99, -1}) { reset(1); setState__4Souli(obj, s); check("none", p(s)); if (get16(572) != static_cast<short>(s)) { std::puts("fail none state"); bad = 1; } }
    return bad;
}
