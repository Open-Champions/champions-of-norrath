// Host contract for RandomTeleportSpell; not an EE input.
#include "random_teleport_spell.cpp"
#include <cstdio>
#include <cstring>
int syncRandState;
void* world;
static int hits, updates, atans;
static float hit_script[64];
static float height_script[64];
static int heights;
float icos__Fi(int a) { return a == 0 ? 1.0f : 0.0f; }
float isin__Fi(int a) { return a == 0 ? 0.0f : 1.0f; }
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, const float*, const float* d, float max_distance, int mode) {
    if (max_distance != 1200.0f || mode != 0 || d[2] != 0.0f) return -9.0f;
    return hit_script[hits++];
}
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, const float*, float) { return height_script[heights++]; }
void objectUpdateInGrid__FP10GameObject(void*) { ++updates; }
int iatan2__Fff(float y, float x) { ++atans; return (int)(y * 100 + x); }
int main() {
    int bad = 0;
    alignas(8) unsigned char obj[64];
    std::memset(obj, 0, sizeof obj);
    float* pos = reinterpret_cast<float*>(obj + 16);
    pos[0] = 100; pos[1] = 200; pos[2] = 5;
    *reinterpret_cast<short*>(obj + 56) = 0;
    // first probe clear and level: lands at full reach along angle 0
    hit_script[0] = 1.0f; height_script[0] = 5.0f;
    RandomTeleportSpell__FP10GameObjectP6Point3f(obj, 0, 50.0f);
    if (pos[0] != 150.0f || pos[1] != 200.0f || pos[2] != 5.0f || hits != 1 || updates != 1 || atans != 0) { std::puts("fail: clear"); bad = 1; }
    // first probe 0.5 and level (kept), later probes blocked (0), then loop ends after 32: best stays 0.5
    hits = 0; heights = 0; pos[0] = 100; pos[1] = 200;
    hit_script[0] = 0.5f; height_script[0] = 5.0f;
    for (int i = 1; i < 40; ++i) hit_script[i] = 0.25f;
    for (int i = 1; i < 40; ++i) height_script[i] = 5.0f;
    RandomTeleportSpell__FP10GameObjectP6Point3f(obj, 0, 50.0f);
    // probes after the first with a lower fraction are not kept; the reach 0.25 < 0.5; no 1.0 -> 32 probes
    if (hits != 32 || pos[0] != 125.0f || pos[1] != 200.0f) { std::printf("fail: partial %d %f %f\n", hits, pos[0], pos[1]); bad = 1; }
    // height outside 12 of z rejects; fraction out of range counts as 1.0; target sets heading
    hits = 0; heights = 0; pos[0] = 100; pos[1] = 200;
    hit_script[0] = 7.0f; height_script[0] = 17.0f; // 17 == z+12 is rejected
    hit_script[1] = -1.0f; height_script[1] = -6.99f; // -1 -> 1.0, kept: height within (-7, 17)
    float target[3] = {110, 230, 0};
    RandomTeleportSpell__FP10GameObjectP6Point3f(obj, target, 10.0f);
    if (hits != 2 || heights != 2) { std::puts("fail: height window"); bad = 1; }
    if (atans != 1 || *reinterpret_cast<short*>(obj + 56) != (short)((230 - pos[1]) * 100 + (110 - pos[0]))) { std::puts("fail: heading"); bad = 1; }
    if (!bad) std::puts("RandomTeleportSpell ok");
    return bad;
}
