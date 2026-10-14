#define NORRATH_HOST_ARENA 1
#include "process_lavastorm.cpp"
#include <string.h>
#include <string>

unsigned char _vt$13WeatherFilter[8];
static unsigned char arena[0x20000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return a == 0 ? nullptr : arena + a; }
extern "C" unsigned norrath_host_word(const void* p) {
    if (p == (const void*)_vt$13WeatherFilter) return 0xbeef0;
    return p ? (unsigned)((const unsigned char*)p - arena) : 0;
}
static std::string tr; static int rolls[8], nroll, c1_ret, c2_ret, count_ret; static unsigned char* free_node; static float dt_ret;
static float hit1[3], hit2[3]; static unsigned filter_vt_seen;
static std::string o(const void* p) { return p ? std::to_string((const unsigned char*)p - arena) : "0"; }
static std::string v3(const float* v) { char b[96]; snprintf(b, sizeof b, "(%g,%g,%g)", v[0], v[1], v[2]); return b; }
static double asDouble(unsigned long long b) { double d; memcpy(&d, &b, 8); return d; }
static unsigned long long asBits(double d) { unsigned long long b; memcpy(&b, &d, 8); return b; }
extern "C" unsigned long long fptodp(float v) { return asBits(v); }
extern "C" int dpcmp(unsigned long long a, unsigned long long b) { double x = asDouble(a), y = asDouble(b); return x < y ? -1 : (x > y ? 1 : 0); }
extern "C" unsigned long long dpsub(unsigned long long a, unsigned long long b) { return asBits(asDouble(a) - asDouble(b)); }
extern "C" int rand(void) { return rolls[nroll++ % 8]; }
extern "C" void __9VICollRay(unsigned char*) { tr += "cr;"; }
extern "C" void __13VISceneFilter(unsigned char*) { tr += "sf;"; }
extern "C" void __8VIEnvRay(unsigned char*) { tr += "er;"; }
extern "C" void Init__9VICollRayRC7VIVect3T1(unsigned char*, const float* a, const float* b) { tr += "ci" + v3(a) + v3(b) + ";"; }
extern "C" void Init__8VIEnvRayRC7VIVect3T1Ui(unsigned char*, const float* a, const float* b, unsigned f) { tr += "ei" + v3(a) + v3(b) + std::to_string(f) + ";"; }
extern "C" int Collide__7VISceneR9VICollRayR13VISceneFilter(unsigned char*, unsigned char*, unsigned char* f) { filter_vt_seen = *(unsigned*)(f + 16); tr += "k1;"; return c1_ret; }
extern "C" int Collide__7VISceneR8VIEnvRay(unsigned char*, unsigned char*) { tr += "k2;"; return c2_ret; }
extern "C" void GetHitPoint__9VICollRay(float* out, unsigned char*) { memcpy(out, hit1, 12); tr += "h1;"; }
extern "C" void GetHitPoint__C8VIEnvRay(float* out, unsigned char*) { memcpy(out, hit2, 12); tr += "h2;"; }
extern "C" void* RemHead__7VILList(void* l) { tr += "rh" + o(l) + ";"; return free_node; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { tr += "at" + o(l) + "," + o(n) + ";"; }
extern "C" void* ParticleDefinition__16VIParticleSystemi(void*, int id) { tr += "pd" + std::to_string(id) + ";"; return arena + 0x1f000 + id; }
extern "C" void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* e, void*, void* d) { tr += "cre" + o(e) + "," + std::to_string((unsigned char*)d - arena - 0x1f000) + ";"; }
extern "C" void SetLocation__17VIParticleEmitterRC7VIVect3(void* e, const float* v) { tr += "sl" + o(e) + v3(v) + ";"; }
extern "C" int ParticleCount__C17VIParticleEmitter(const void* e) { tr += "pc" + o(e) + ";"; return count_ret; }
extern "C" float DeltaTime__C16VIParticleSystem(const void*) { tr += "dt;"; return dt_ret; }
extern "C" void Update__17VIParticleEmitter(void* e) { tr += "up" + o(e) + ";"; }
extern "C" void Destroy__17VIParticleEmitter(void* e) { tr += "de" + o(e) + ";"; }
extern "C" void Remove__7VILNode(void* n) { tr += "rm" + o(n) + ";"; }
static float fb(unsigned b) { float f; memcpy(&f, &b, 4); return f; }

enum { SELF = 0x100, CAMERA = 0x2000, N0 = 0x4000, N1 = 0x6000, TAIL = 0x8000, FREEN = 0xa000 };
static unsigned char* self = arena + SELF; static unsigned char* cam = arena + CAMERA;
static void setup(float intensity, int live) {
    memset(arena, 0, sizeof arena); tr.clear(); nroll = 0; c1_ret = 1; c2_ret = 0; count_ret = 0; dt_ret = 0.5f; free_node = arena + FREEN;
    for (int i = 0; i < 8; ++i) rolls[i] = 0;
    *(unsigned*)self = 0x1f800; *(float*)(self + 76) = intensity; *(int*)(self + 4460) = live; *(int*)(self + 4480) = 7; *(int*)(self + 4484) = 9;
    *(unsigned*)(self + 4448) = TAIL;                      // empty active list: head word is the tail node
    *(float*)(cam + 28) = 100.0f; *(float*)(cam + 32) = 200.0f; *(float*)(cam + 36) = 300.0f;
    hit1[0] = 1.0f; hit1[1] = 10.0f; hit1[2] = 3.0f; hit2[0] = 4.0f; hit2[1] = 20.0f; hit2[2] = 6.0f;
}
static void run() { ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(self, arena + 0x3000, cam); }

int main() {
    // no spawn when the live count is not below (int)(intensity * 32)
    setup(1.0f, 32); run(); if (tr != "") return 1;
    setup(0.0f, 0); run(); if (tr != "") return 2;
    setup(0.99f, 31); run(); if (tr != "") return 3;          // (int)(31.68) = 31
    // spawn with both roll pairs equal: x = z = 0, so both are pushed to +30 and the camera is added
    setup(1.0f, 0); c1_ret = 0; run();
    if (tr != "cr;sf;er;ci(130,5200,330)(130,-4800,330);ei(130,5200,330)(130,-4800,330)15;k1;") return 4;
    if (filter_vt_seen != 0xbeef0) return 5;
    // negative x and z are pushed by -30; large values are left alone
    setup(1.0f, 0); c1_ret = 0; rolls[0] = 0; rolls[1] = 0x7fffffff; rolls[2] = 0; rolls[3] = 0x7fffffff; run();   // (0 - 2^31) * 2^-31 * 50 = -50: not pushed
    if (tr.find("ci(50,5200,250)") == std::string::npos) return 6;
    setup(1.0f, 0); c1_ret = 0; rolls[0] = 0; rolls[1] = 0x10000000; rolls[2] = 0x10000000; rolls[3] = 0; run();       // x = -6.25, z = +6.25: pushed -30 / +30
    if (tr.find("ci(63.75,5200,336.25)") == std::string::npos) return 7;
    setup(1.0f, 0); c1_ret = 0; rolls[0] = 0x7fffffff; rolls[1] = 0; run();                                             // x = 50 (>= 10): neither is pushed; z = 0
    if (tr.find("ci(150,5200,300)") == std::string::npos) return 8;
    setup(1.0f, 0); c1_ret = 0; rolls[0] = 0x10000000; rolls[1] = 0; rolls[2] = 0x7fffffff; rolls[3] = 0; run();       // x = 6.25 (<10), z = 50 (>=10): no push
    if (tr.find("ci(106.25,5200,350)") == std::string::npos) return 9;
    // a hit: the free node is taken and filled in; the env ray hit replaces the first only when higher
    setup(1.0f, 4); free_node = arena + FREEN; c2_ret = 1; hit2[1] = 5.0f; run();      // H.y 10 >= 5: keep H
    std::string base = "cr;sf;er;ci(130,5200,330)(130,-4800,330);ei(130,5200,330)(130,-4800,330)15;k1;h1;k2;h2;rh" + std::to_string(SELF + 4464) + ";at" + std::to_string(SELF + 4448) + "," + std::to_string(FREEN) + ";";
    if (tr.compare(0, base.size(), base) != 0) return 10;
    unsigned char* nd = arena + FREEN;
    if (*(float*)(nd + 28) != 1.0f || *(float*)(nd + 32) != 10.0f || *(float*)(nd + 36) != 3.0f) return 11;
    if (*(float*)(nd + 16) != 1.0f || *(float*)(nd + 20) != 50.0f || *(float*)(nd + 24) != 3.0f) return 12;
    if (*(int*)(nd + 40) != 0 || *(int*)(nd + 48) != 0 || *(int*)(self + 4460) != 5) return 13;
    // speed = ((131 * 0 & 255) * S * 0.6 + 0.7) * 2.5 with the 5th roll 0
    if (*(float*)(nd + 44) != (0.0f * fb(0x3b808080) * fb(0x3f199999) + fb(0x3f333333)) * 2.5f) return 14;
    std::string tail = "pd7;cre" + std::to_string(FREEN + 64) + ",7;sl" + std::to_string(FREEN + 64) + "(1,50,3);sl" + std::to_string(FREEN + 64) + "(1,50,3);pd9;cre" + std::to_string(FREEN + 1056) + ",9;sl" + std::to_string(FREEN + 1056) + "(1,10,3);sl" + std::to_string(FREEN + 1056) + "(1,10,3);";
    if (tr.substr(base.size(), tail.size()) != tail) return 15;
    // the env hit is higher: it replaces the whole hit point
    setup(1.0f, 4); c2_ret = 1; hit2[1] = 20.0f; run();
    if (*(float*)(nd + 28) != 4.0f || *(float*)(nd + 32) != 20.0f || *(float*)(nd + 36) != 6.0f) return 16;
    // no free node: nothing else happens in the spawn
    setup(1.0f, 4); free_node = nullptr; run();
    if (tr.find("at") != std::string::npos || *(int*)(self + 4460) != 4) return 17;
    // update: one node (N0, then the tail) with progress < 1 interpolates start -> target
    setup(0.0f, 1); *(unsigned*)(self + 4448) = N0; *(unsigned*)(arena + N0) = TAIL;
    *(float*)(arena + N0 + 16) = 10.0f; *(float*)(arena + N0 + 20) = 20.0f; *(float*)(arena + N0 + 24) = 30.0f;
    *(float*)(arena + N0 + 28) = 20.0f; *(float*)(arena + N0 + 32) = 60.0f; *(float*)(arena + N0 + 36) = 50.0f;
    *(float*)(arena + N0 + 40) = 0.25f; *(float*)(arena + N0 + 44) = 2.0f; dt_ret = 1.0f;
    run();
    // progress = 0.25 + 1.0 * 0.15 * 2.0 = 0.55; position = start + (target - start) * 0.55
    float pr = 0.25f + 1.0f * fb(0x3e199999) * 2.0f;
    if (*(float*)(arena + N0 + 40) != pr) return 18;
    float lx = (20.0f - 10.0f) * pr + 10.0f, ly = (60.0f - 20.0f) * pr + 20.0f, lz = (50.0f - 30.0f) * pr + 30.0f;
    float loc[3] = {lx, ly, lz};
    if (tr != "pc" + std::to_string(N0 + 64) + ";dt;sl" + std::to_string(N0 + 64) + v3(loc) + ";up" + std::to_string(N0 + 64) + ";") return 19;
    // progress >= 1 with state 0: the impact emitter is updated once and the state set
    setup(0.0f, 1); *(unsigned*)(self + 4448) = N0; *(unsigned*)(arena + N0) = TAIL; *(float*)(arena + N0 + 40) = 0.9f; *(float*)(arena + N0 + 44) = 4.0f; dt_ret = 1.0f;
    run();
    if (tr != "pc" + std::to_string(N0 + 64) + ";dt;up" + std::to_string(N0 + 1056) + ";" || *(int*)(arena + N0 + 48) != 1) return 20;
    // state 1 and the second emitter still has particles: nothing more
    setup(0.0f, 1); *(unsigned*)(self + 4448) = N0; *(unsigned*)(arena + N0) = TAIL; *(float*)(arena + N0 + 40) = 0.9f; *(float*)(arena + N0 + 44) = 4.0f; *(int*)(arena + N0 + 48) = 1; count_ret = 5; dt_ret = 1.0f;
    run();
    if (tr != "pc" + std::to_string(N0 + 64) + ";dt;pc" + std::to_string(N0 + 1056) + ";" || *(int*)(self + 4460) != 1) return 21;
    // state 1, no particles left: both emitters are destroyed and the node goes back to the free list
    count_ret = 0; tr.clear(); *(unsigned*)(self + 4448) = N0; *(float*)(arena + N0 + 40) = 0.9f;
    run();
    if (tr != "pc" + std::to_string(N0 + 64) + ";dt;pc" + std::to_string(N0 + 1056) + ";de" + std::to_string(N0 + 64) + ";de" + std::to_string(N0 + 1056) + ";rm" + std::to_string(N0) + ";at" + std::to_string(SELF + 4464) + "," + std::to_string(N0) + ";" || *(int*)(self + 4460) != 0) return 22;
    // two nodes: the last (tail) one is not visited
    setup(0.0f, 2); *(unsigned*)(self + 4448) = N0; *(unsigned*)(arena + N0) = N1; *(unsigned*)(arena + N1) = TAIL; dt_ret = 0.0f; *(float*)(arena + N0 + 44) = 1.0f; *(float*)(arena + N1 + 44) = 1.0f;
    run();
    if (tr.find("pc" + std::to_string(N0 + 64)) == std::string::npos || tr.find("pc" + std::to_string(N1 + 64)) == std::string::npos || tr.find("pc" + std::to_string(TAIL + 64)) != std::string::npos) return 23;
    return 0;
}
