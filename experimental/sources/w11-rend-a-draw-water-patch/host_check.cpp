#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned long long dword(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static int smoothCalls, paused, addCalls, perturbCalls;
static unsigned char* smoothWorld; static unsigned char* smoothData;
static std::vector<unsigned char*> particleDefs;
static std::vector<float> particleZ;
static float perturbA, perturbB, perturbPoint[3], heightArg, heightPoint[3], heightResult;
static void* listArg; static int kindArg; static unsigned char* texArg; static unsigned char* recArg; static int extraArg;
static float matIn[12];
extern "C" {
int worldWaterAlpha, worldWaterAlpha2, worldWaterColor, worldWaterColor2;
float worldWaterWavyness, worldWaterWavyness2;
unsigned char* waterTexture;
unsigned char* waterList;
unsigned char* waterListBack;
unsigned char* glistp;
int frameNm, levelFlags, eRandState;
float worldCharViewMatrix[16];
unsigned char rainSplashParticle[80];
unsigned char droplet[80];
void smoothPatch__FP12_worldHeaderP16WaterPolysetData(unsigned char* world, unsigned char* data) { ++smoothCalls; smoothWorld = world; smoothData = data; }
void matMul__FP8Matrix44P8Matrix34T0(float* out, const float* a, const float* b) {
    std::memcpy(matIn, a, sizeof matIn);
    for (int i = 0; i < 16; ++i) out[i] = 100.0f + i + b[i];
}
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* texture, unsigned char* record, int extra) {
    listArg = list; kindArg = kind; texArg = texture; recArg = record; extraArg = extra;
}
int gameIsPaused__Fv() { return paused; }
void worldPerturbWater__FP12_worldHeaderG6Point3ff(unsigned char*, const float* p, float a, float b) {
    ++perturbCalls; perturbA = a; perturbB = b; std::memcpy(perturbPoint, p, 12);
}
float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char*, const float* p, float f) {
    heightArg = f; std::memcpy(heightPoint, p, 12); return heightResult;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* def, float* p, float* v, float f) {
    if (v[0] != 0 || v[1] != 0 || v[2] != 0 || f != 0.0f) std::printf("bad particle args\n");
    particleDefs.push_back(def); particleZ.push_back(p[2]);
}
float floorf(float x) { return std::floor(x); }
void drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* ph, int layer);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
static unsigned char* rec(int k) { return arena_at(0x10000 + 0x2000 * k); }
int main() {
    unsigned char* world = arena_at(0x100);
    unsigned char* ph = arena_at(0x200);
    unsigned char* h = arena_at(0x400);
    unsigned char* g = arena_at(0x900) + 4;   // low four bits 4
    unsigned char* ownerPs = arena_at(0x600);
    unsigned char* ownerH = arena_at(0x700);
    auto arm = [&] {
        std::memset(norrath_arena + 0x100, 0, 0x2000);
        std::memset(g - 4, 0xee, 0x400);
        for (int k = 0; k < 4; ++k) {
            std::memset(rec(k), 0, 4800);
            sw_(rec(k) + 4764, k > 0 ? A(rec(k - 1)) : 0);   // newer
            sw_(rec(k) + 4760, k < 3 ? A(rec(k + 1)) : 0);   // older
            sw_(rec(k) + 4768, 0x5000 + k);
        }
        sw_(rec(3) + 4768, A(ownerPs));
        sw_(ownerPs, A(ownerH)); sw_(ownerH + 8, 0x1234);
        waterList = rec(0); waterListBack = rec(3);
        sw_(ph, A(h)); sw_(ph + 36, 0); sf_(ph + 8, 10.2f); sf_(ph + 12, 20.7f); sf_(ph + 16, 5.0f);
        sf_(ph + 20, 15.2f); sf_(ph + 24, 30.0f); sf_(ph + 28, 9.0f); sw_(ph + 48, 41);
        glistp = g; frameNm = 777; levelFlags = 0; paused = 0; smoothCalls = 0; particleDefs.clear(); particleZ.clear();
        worldWaterAlpha = 98; worldWaterAlpha2 = -50; worldWaterColor = 0x808080; worldWaterColor2 = -2;
        worldWaterWavyness = 0.5f; worldWaterWavyness2 = 0.25f; waterTexture = arena_at(0x30); perturbCalls = 0;
        for (int i = 0; i < 16; ++i) worldCharViewMatrix[i] = (float)i;
        eRandState = 1; addCalls = 0; listArg = nullptr;
    };
    // Recycling the oldest record: it becomes the newest, its old owner loses its link.
    arm();
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 9);
    if (hw(h + 8) != A(rec(3)) || waterList != rec(3) || waterListBack != rec(2)) return 1;
    if (hw(rec(2) + 4760) != 0 || hw(rec(3) + 4760) != A(rec(0)) || hw(rec(0) + 4764) != A(rec(3)) || hw(rec(3) + 4764) != 0) return 2;
    if (hw(rec(3) + 4752) != A(world) || hw(rec(3) + 4756) != 777 || hw(rec(3) + 4768) != A(ph) || hw(ownerH + 8) != 0) return 3;
    if (smoothCalls != 1 || smoothWorld != world || smoothData != h) return 4;
    // Packet: g is padded so the cursor has low bits 12 (g+8 holds 0x11000000), and glistp moves past
    // the tag quadwords.
    if (hw(g + 8) != 0x11000000u || listArg != g || kindArg != 3 || texArg != waterTexture || recArg != nullptr || extraArg != 0) return 5;
    unsigned char* c = g + 12;
    while ((A(c) & 15) != 12) ++c;
    if (c == g + 12) return 6;                       // g was chosen so that padding is needed
    for (unsigned char* p = g + 12; p < c; ++p) if (*p != 0) return 6;
    if (dword(c + 4) != (0x1000000000008000ull | 6) || hw(c) != 0x50000007u) return 7;
    if (dword(c + 12) != 14 || dword(c + 28) != 63 || dword(c + 44) != 6 || dword(c + 52) != 0x71001 || dword(c + 60) != 71) return 8;
    if (dword(c + 68) != ((98ull << 32) | 100) || dword(c + 76) != 66 || dword(c + 84) != 97 || dword(c + 92) != 20) return 9;
    if (dword(c + 100) != 0x808080 || dword(c + 108) != 1 || hw(c + 116) != (9u | 0x04000000u) || hw(c + 120) != 0x6c054000u) return 10;
    // Matrix input: scale 0.5 at [2], 1.0 at [4] and [6], translation floor(10.7) = 10, floor(21.2) = 21, 41 / 4.
    if (matIn[2] != 0.5f || matIn[4] != 1.0f || matIn[6] != 1.0f || matIn[0] != 0 || matIn[9] != 10.0f || matIn[10] != 21.0f || matIn[11] != 10.25f || matIn[8] != 0) return 11;
    unsigned char* m = c + 124;
    for (int i = 0; i < 16; ++i) if (lf_(m + 4 * i) != 100.0f + i + i) return 12;
    if (hw(m + 64) != 0x803eu || hw(m + 68) != 0x20ae4000u || hw(m + 72) != 67 || hw(m + 80) != 0x14000036u) return 13;
    if (lf_(m + 76) != std::floor(5.0f + 0.5f) * 0x1.111110p-5f) return 14;
    unsigned char* e = m + 84;
    while ((A(e) & 15) != 0) { if (*e != 0) return 15; ++e; }
    if (dword(e) != (((unsigned long long)(A(rec(3)) + 64) << 32) | 0x300000a1ull)) return 15;
    if (dword(e + 8) != 0 || dword(e + 16) != 0x60000000ull || dword(e + 24) != 0 || glistp != e + 32) return 16;
    if (dword(g) != (((unsigned long long)(e - g) >> 3) - 2) / 2 + 0x10000000ull) return 17;
    // Moving to the front: record 1 sits in the middle of the list.
    arm(); sw_(h + 8, A(rec(1)));
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (waterList != rec(1) || waterListBack != rec(3) || hw(rec(1) + 4764) != 0 || hw(rec(1) + 4760) != A(rec(0)) || hw(rec(0) + 4764) != A(rec(1))) return 19;
    if (hw(rec(0) + 4760) != A(rec(2)) || hw(rec(2) + 4764) != A(rec(0)) || hw(rec(2) + 4760) != A(rec(3)) || hw(rec(1) + 4756) != 777 || smoothCalls != 0) return 20;
    // Moving the oldest to the front changes the back of the list.
    arm(); sw_(h + 8, A(rec(3)));
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (waterList != rec(3) || waterListBack != rec(2) || hw(rec(2) + 4760) != 0 || hw(rec(3) + 4760) != A(rec(0))) return 21;
    // The newest stays where it is and only gets the frame number.
    arm(); sw_(h + 8, A(rec(0)));
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (waterList != rec(0) || hw(rec(0) + 4756) != 777 || hw(rec(0) + 4764) != 0 || waterListBack != rec(3)) return 22;
    // Flag 0x1000 selects the second alpha (negative: 104 and the magnitude), colour and wavyness.
    arm(); sw_(h + 8, A(rec(0))); sw_(ph + 36, 0x1000);
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    c = g + 12; while ((A(c) & 15) != 12) ++c;
    if (dword(c + 68) != ((50ull << 32) | 104) || dword(c + 100) != 0xfffffffffffffffeull || matIn[2] != 0.25f) return 23;
    // Rain: find a state whose next value is a multiple of 300 in the top half.
    unsigned x = 1;
    for (;; ++x) { unsigned s = x * 0x19660du + 0x3c6ef35fu; if ((s >> 16) % 300u == 0) break; }
    arm(); sw_(h + 8, A(rec(0))); levelFlags = 0x10; eRandState = (int)x; heightResult = 3.5f;
    drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    unsigned s1 = x * 0x19660du + 0x3c6ef35fu, s2 = s1 * 0x19660du + 0x3c6ef35fu, s3 = s2 * 0x19660du + 0x3c6ef35fu;
    if (particleDefs.size() != 2 || particleDefs[0] != rainSplashParticle || particleDefs[1] != droplet) return 24;
    if (perturbCalls != 1 || perturbA != 1500.0f || perturbB != 6.0f || heightArg != 6.0f) return 25;
    float u = (float)(s2 >> 16) * 0x1p-16f;
    if (std::fabs(perturbPoint[0] - (10.2f + (15.2f - 10.2f) * u)) > 1e-4f || std::fabs(perturbPoint[1] - (20.7f + (30.0f - 20.7f) * u)) > 1e-4f || perturbPoint[2] != 5.0f) return 26;
    if (particleZ[0] != 3.5f || std::fabs(particleZ[1] - (3.5f + (float)(s3 >> 16) * 0x1p-16f * 80.0f + 24.0f)) > 1e-3f) return 27;
    if ((unsigned)eRandState != s3) return 28;
    // A nonzero remainder or a paused game stops after the first step.
    arm(); sw_(h + 8, A(rec(0))); levelFlags = 0x10; eRandState = 1; drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (!particleDefs.empty() || (unsigned)eRandState != 1u * 0x19660du + 0x3c6ef35fu) return 29;
    arm(); sw_(h + 8, A(rec(0))); levelFlags = 0x10; eRandState = (int)x; paused = 1; drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, ph, 0);
    if (!particleDefs.empty() || (unsigned)eRandState != (unsigned)x) return 30;
    return 0;
}
