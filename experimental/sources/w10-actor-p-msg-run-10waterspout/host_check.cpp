// Host contract for msg_run__10WaterSpout; not an EE input. Includes the source so its private
// particle definitions and flags can be inspected. The authored function was also compared with the
// original in a private R5900 run (120 randomized scenarios, including the first-call construction).
#define W10_ACTOR_P_HOST 1
unsigned char* w10_actor_p_host_base;
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* world = reinterpret_cast<unsigned char*>(0x1234);
int frameNm;
static std::string calls;
static unsigned rollValue = 32768;      // 2 * (32768 * 2^-16) - 1 == 0: every ball point is the origin
static int rolls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string which(void* d) { return d == splashParticle ? "splash" : d == waterParticle ? "water" : d == foamParticle ? "foam" : "bad"; }
static std::string pt(const Point3* p) { return num(p->x) + ":" + num(p->y) + ":" + num(p->z); }
extern "C" {
unsigned eRand__10GameObject(void*) { ++rolls; return rollValue; }
void particledef_construct(void* s, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    calls += "def(" + which(s) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(x) +
             "," + num(y) + "," + num(z) + "," + num(i) + "," + num(j) + "," + num(k) + (cb ? ",cb" : ",null") + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float t) {
    calls += "add(" + which(d) + "," + pt(p) + "," + pt(v) + "," + num(t) + ");";
}
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* w, Point3 p, float a, float b) {
    calls += std::string("perturb(") + (w == world ? "world" : "bad") + "," + pt(&p) + "," + num(a) + "," + num(b) + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}

int main() {
    int bad = 0;
    alignas(16) unsigned char object[128] = {};
    float v[5] = {10, 20, 30};
    std::memcpy(object + 16, v, 12);
    float drift[2] = {2.0f, -1.0f};
    std::memcpy(object + 76, drift, 8);
    object[88] = 1; object[89] = 30;

    frameNm = 2; calls.clear(); rolls = 0;
    msg_run__10WaterSpout(object);
    const std::string defs =
        "def(splash,100,110,100,113,30,40,30,20,82.538,156.578,2,2,0,2,null);"
        "def(water,70,83,74,59,31,43,31,55,21,81,-5.8,2,0,2,null);"
        "def(foam,90,102,93,59,31,40,30,55,-10.96,60.497,-5.88,2,1,1,null);";
    const std::string splash = "add(splash,18:16:-26,0:0:0,0);";      // P + 4 * (2, -1) = (18, 16), z = 30 - 56
    const std::string water = "add(water,10:20:30,2:-1:0,0);";
    const std::string foam = "add(foam,10:20:30,2:-1:0,0);";
    std::string want = defs + splash + splash + water + water + foam;
    if (calls != want || rolls != 30 || object[89] != 31 || object[88] != 1) {
        // ball points of three draws each: splash 2 * 2, water 2 * 2, foam 2
        std::printf("fail first call (rolls=%d)\n  want %s\n  got  %s\n", rolls, want.c_str(), calls.c_str()); bad = 1;
    }
    // second call: no construction; odd frame: no foam; counter 31 removes the object from the run list
    frameNm = 3; calls.clear();
    msg_run__10WaterSpout(object);
    want = splash + splash + water + water + "remove;";
    if (calls != want || object[88] != 0 || object[89] != 32) { std::printf("fail second call\n  want %s\n  got  %s\n", want.c_str(), calls.c_str()); bad = 1; }
    // every 32nd frame: perturb at P + 4 * drift, z - 56
    frameNm = 64; calls.clear(); object[88] = 1; object[89] = 0;
    msg_run__10WaterSpout(object);
    want = splash + splash + water + water + foam + "perturb(world,18:16:-26,1043,24);";
    if (calls != want || object[88] != 1 || object[89] != 1) { std::printf("fail third call\n  want %s\n  got  %s\n", want.c_str(), calls.c_str()); bad = 1; }
    // a byte of 127 wraps to -128 on increment and is below 31 afterwards
    frameNm = 5; calls.clear(); object[89] = 127;
    msg_run__10WaterSpout(object);
    if (calls.find("remove") == std::string::npos || object[89] != 128) { std::puts("fail: byte 127"); bad = 1; }
    frameNm = 5; calls.clear(); object[89] = 128;
    msg_run__10WaterSpout(object);
    if (calls.find("remove") != std::string::npos || object[89] != 129) { std::puts("fail: byte 128"); bad = 1; }
    return bad;
}
