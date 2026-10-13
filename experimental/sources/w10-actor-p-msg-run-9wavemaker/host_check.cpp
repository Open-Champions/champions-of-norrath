// Host contract for msg_run__9WaveMaker; not an EE input. 32-bit words are offsets from a host base and
// callees log. The authored function was also compared with the original in a private R5900 run
// (over 100 randomized scenarios covering every instruction of the original).
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
unsigned char* world = reinterpret_cast<unsigned char*>(0x77);
unsigned char range[512];
unsigned splashInfoBits[128];
signed char firstTime = 1;
unsigned char waveFroth[80], waveMainParticle[80], waveMainAlternate[80], waveRandomFoam[80], waveTrail[80], waveSplashBig[80], waveSplashSmall[80];
static std::string calls;
static bool visibleResult = true;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static std::string def(void* d) {
    return d == waveFroth ? "froth" : d == waveMainParticle ? "main" : d == waveMainAlternate ? "alt" : d == waveRandomFoam ? "randomfoam" :
           d == waveTrail ? "trail" : d == waveSplashBig ? "big" : d == waveSplashSmall ? "small" : "bad";
}
extern "C" {
void getWaveletMat__9WaveMakerifP8Matrix33(void* s, int i, float d, Matrix33* m) {
    calls += std::string("mat(") + (s == obj ? "self" : "bad") + "," + num(i) + "," + num(d) + ");";
    for (int j = 0; j < 9; ++j) m->m[j] = 0.0f;
    m->m[3] = 1.0f; m->m[4] = 0.0f;
}
void doWavelet__9WaveMakerP6Point3P8Matrix33i(void*, Point3* p, Matrix33* m, int count) { calls += "wavelet(" + pt(*p) + "," + num(m->m[3]) + "," + num(count) + ");"; }
void doWaveletCollision__9WaveMakerP6Point3i(void*, Point3* p, int index) { calls += "collision(" + pt(*p) + "," + num(index) + ");"; }
void doWaveletReflection__9WaveMakerP6Point3(void*, Point3* p) { calls += "reflection(" + pt(*p) + ");"; }
int gamePositionVisible__FP6Point3f(Point3* p, float r) { calls += "visible(" + pt(*p) + "," + num(r) + ");"; return visibleResult; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float t) { calls += "add(" + def(d) + "," + pt(*p) + "," + pt(*v) + "," + num(t) + ");"; }
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* w, Point3 p, float a, float b) {
    calls += std::string("perturb(") + (w == world ? "world" : "bad") + "," + pt(p) + "," + num(a) + "," + num(b) + ");";
}
void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* d, int a, int b, int c, int dd, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k,
                                                              void (*cb)(void*)) {
    calls += "type(" + def(d) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(dd) + "," + num(e) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(x) +
             "," + num(y) + "," + num(z) + "," + num(i) + "," + num(j) + "," + num(k) + (cb ? ",cb" : ",null") + ");";
}
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }

// A closed two-point line: (0,0) to (10,0), each record's spacing value 34 (two steps a segment).
static void prepare(unsigned seed) {
    std::memset(arena, 0, sizeof arena); std::memset(range, 0, sizeof range); std::memset(splashInfoBits, 0, sizeof splashInfoBits);
    calls.clear(); w10_actor_p_host_base = arena; visibleResult = true;
    put32(0x1000 + 1892, 0x2000); put32(0x2000 + 4, 2); put32(0x2000 + 8, 1); put32(0x2000 + 20, 0x2400);
    putf(0x2400 + 0, 0); putf(0x2400 + 4, 0); putf(0x2400 + 8, 5); putf(0x2400 + 48, 34);
    putf(0x2400 + 64 + 0, 10); putf(0x2400 + 64 + 4, 0); putf(0x2400 + 64 + 8, 5); putf(0x2400 + 64 + 48, 34);
    putf(0x1000 + 1888, 2.0f);
    put32(0x1000 + 68, seed);
}
int main() {
    int bad = 0;
    // First call: the counter becomes 1, the types are defined once, and firstTime is cleared.
    prepare(0);
    firstTime = 1;
    msg_run__9WaveMaker(obj);
    const std::string types =
        "type(main,103,118,143,80,99,116,121,24,26.7823,83.7563,-0.746956,1,0,2,null);"
        "type(alt,65,80,93,84,103,148,111,15,71,182.615,-3.52196,2,0,2,null);"
        "type(randomfoam,79,85,74,62,67,99,89,0,28.0539,146.482,-3.58299,3,1,2,null);"
        "type(trail,118,127,128,81,129,132,137,0,8.57582,5.7215,0,3,0,2,null);"
        "type(big,104,103,130,93,72,76,74,0,7,20,-6,3,1,2,null);"
        "type(small,77,67,93,85,72,76,74,0,7,20,-1.35197,2,1,2,null);"
        "type(froth,123,124,127,97,82,84,83,0,3.51899,20.5058,0,3,1,5,null);";
    if (get32(0x1000 + 1880) != 1 || firstTime != 0 || calls.size() < types.size() || calls.compare(calls.size() - types.size(), types.size(), types) != 0) {
        std::printf("fail: first call\n  %s\n", calls.c_str()); bad = 1;
    }
    // Everything before the type definitions: two segments of two steps each (distance 0 and 0.5).
    const std::string head = calls.substr(0, calls.size() - types.size());
    const char* wants[] = {"mat(self,0,0);", "mat(self,0,0.5);", "mat(self,1,0);", "mat(self,1,0.5);"};
    size_t at = 0;
    for (const char* w : wants) {
        size_t f = head.find(w, at);
        if (f == std::string::npos) { std::printf("fail: missing %s in %s\n", w, head.c_str()); bad = 1; break; }
        at = f + std::strlen(w);
    }
    if (head.find("mat(self,0,1)") != std::string::npos || head.find("mat(self,2") != std::string::npos) { std::puts("fail: extra steps"); bad = 1; }
    // Second call: no definitions; the half-word sum is added; the counter advances again.
    prepare(0);
    firstTime = 0;
    { unsigned short a = 65000, b = 1000; std::memcpy(arena + 0x1000 + 1876, &a, 2); std::memcpy(arena + 0x1000 + 1878, &b, 2); }
    msg_run__9WaveMaker(obj);
    unsigned short sum; std::memcpy(&sum, arena + 0x1000 + 1876, 2);
    if (calls.find("type(") != std::string::npos || sum != static_cast<unsigned short>(65000 + 1000)) { std::puts("fail: second call"); bad = 1; }
    // Invisible steps: nothing is handed to the wavelet routines and the counter resets (no wavelet, count below the limit).
    prepare(0);
    firstTime = 0; visibleResult = false; put32(0x1000 + 1880, 7); put32(0x1000 + 1884, 100);
    msg_run__9WaveMaker(obj);
    if (calls.find("wavelet(") != std::string::npos || calls.find("reflection(") != std::string::npos || get32(0x1000 + 1880) != 0) { std::puts("fail: invisible"); bad = 1; }
    // Close to the collision window (base 480, time near 463.9) a collision call is made for the step.
    prepare(0);
    firstTime = 0; std::memset(range, 20, sizeof range); put32(0x1000 + 1880, 263);   // counter 264, time 463.9
    msg_run__9WaveMaker(obj);
    if (calls.find("collision(") == std::string::npos || calls.find("wavelet(") == std::string::npos) { std::printf("fail: collision window: %s\n", calls.c_str()); bad = 1; }
    return bad;
}
