// Host check for worldRunWater, built with -DNORRATH_HOST_WATERRUN: pause, retirement, the ripple, the
// waterStep call and both strip exchanges. Stored addresses are offsets inside a test arena.
#include <cstdio>
#include <cstring>
unsigned char* norrath_water_arena;
int nmWaterPatches, frameNm, worldWaterRainTimer, worldWaterBumpTimer, eRandState;
unsigned char* waterPatch;
float worldWaterBumpRadius, worldWaterBumpAmount;
static int paused, steps;
static void* step_args[3];
extern "C" int gameIsPaused__Fv() { return paused; }
extern "C" void waterStep(void* a, void* b, void* c) { ++steps; step_args[0] = a; step_args[1] = b; step_args[2] = c; }
extern "C" void worldRunWater__FP12_worldHeader(void* world);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static void setw(unsigned char* b, unsigned off, unsigned v) { memcpy(b + off, &v, 4); }
static unsigned getw(const unsigned char* b, unsigned off) { unsigned v; memcpy(&v, b + off, 4); return v; }
static void seth(unsigned char* b, unsigned off, unsigned v) { unsigned short h = (unsigned short)v; memcpy(b + off, &h, 2); }
static unsigned geth(const unsigned char* b, unsigned off) { unsigned short h; memcpy(&h, b + off, 2); return h; }
static short getsh(const unsigned char* b, unsigned off) { short h; memcpy(&h, b + off, 2); return h; }
int main() {
    static unsigned char arena[1 << 18];
    norrath_water_arena = arena;
    const unsigned PB = 0x1000, W = 0x20000, REC = 0x20100, LNK = 0x20800, OWN = 0x20900, BLK = 0x20a00, BLK2 = 0x20b00;
    unsigned char* p0 = arena + PB;
    unsigned char* p1 = arena + PB + 4784;
    waterPatch = p0; nmWaterPatches = 2; frameNm = 1000;
    worldWaterRainTimer = 5; worldWaterBumpTimer = 7; worldWaterBumpRadius = 2.0f; worldWaterBumpAmount = 100.0f;
    setw(arena, W + 36, REC);
    // patch 0 is live, owner indices point at records 0 and 1 whose blocks name patch 1
    setw(p0, 4752, W); setw(p0, 4756, 900); setw(p0, 4768, LNK);
    setw(arena, LNK, OWN); setw(arena, OWN + 8, 0x77);
    seth(arena, OWN, 0); seth(arena, OWN + 4, 1);
    setw(arena, REC + 0, BLK); setw(arena, REC + 60, BLK2);
    setw(arena, BLK + 8, PB + 4784); setw(arena, BLK2 + 8, PB + 4784);
    for (int k = 0; k < 32; ++k) {
        seth(p0, 82 + 80 * k, 0x1000 + k); seth(p1, 140 + 80 * k, 0x2000 + k);
        seth(p0, 2706 + 64 * k, 0x3000 + k); seth(p1, 2764 + 64 * k, 0x4000 + k);
        seth(p0, 160 + 2 * k, 0x5000 + k); seth(p1, 2480 + 2 * k, 0x6000 + k);
        seth(p0, 2768 + 2 * k, 0x7000 + k); seth(p1, 4624 + 2 * k, 0x7100 + k);
    }
    paused = 1;
    worldRunWater__FP12_worldHeader(0);
    CHECK(steps == 0 && worldWaterRainTimer == 5);
    paused = 0;
    seth(arena, OWN + 4, 0xffff);               // only the first index
    worldRunWater__FP12_worldHeader(0);
    CHECK(steps == 1 && step_args[0] == p0 + 80 && step_args[1] == p0 + 2704 && step_args[2] == arena + OWN + 16);
    CHECK(worldWaterRainTimer == 4 && eRandState == 0);   // no ripple while the timer is not negative
    CHECK(geth(p0, 80 + 80 * 4) == 0x2004 && geth(p1, 142 + 80 * 4) == 0x1004);
    CHECK(geth(p0, 2704 + 64 * 9) == 0x4009 && geth(p1, 2766 + 64 * 9) == 0x3009);
    CHECK(geth(p0, 80 + 2 * 3) != 0x6003 && geth(p1, 2560 + 2 * 3) != 0x5003);
    seth(arena, OWN, 0xffff); seth(arena, OWN + 4, 1);   // only the second index
    worldRunWater__FP12_worldHeader(0);
    CHECK(steps == 2 && worldWaterRainTimer == 3);
    CHECK(geth(p0, 80 + 2 * 3) == 0x6003 && geth(p1, 2560 + 2 * 3) == 0x5003);
    CHECK(geth(p0, 2704 + 2 * 3) == 0x7103 && geth(p1, 4688 + 2 * 3) == 0x7003);
    // retirement
    setw(p0, 4756, 800);
    worldRunWater__FP12_worldHeader(0);
    CHECK(getw(p0, 4768) == 0 && getw(arena, OWN + 8) == 0 && steps == 2);
    // ripple: with a negative timer one random draw sequence runs and heights near the centre grow
    memset(arena, 0, sizeof arena);
    nmWaterPatches = 1; waterPatch = p0; frameNm = 100; eRandState = 12345;
    setw(arena, W + 36, REC);
    setw(p0, 4752, W); setw(p0, 4756, 90); setw(p0, 4768, LNK);
    setw(arena, LNK, OWN); seth(arena, OWN, 0xffff); seth(arena, OWN + 4, 0xffff);
    worldWaterRainTimer = -1; worldWaterBumpTimer = 7; worldWaterBumpRadius = 2.0f; worldWaterBumpAmount = 100.0f;
    unsigned state = 12345;
    unsigned r[3];
    for (int i = 0; i < 3; ++i) { state = state * 0x19660du + 0x3c6ef35fu; r[i] = state >> 16; }
    int cx = r[0] % 20 + 6, cy = r[1] % 20 + 6;
    worldRunWater__FP12_worldHeader(0);
    CHECK((unsigned)eRandState == state && worldWaterRainTimer == (int)(r[2] % 7));
    CHECK(getsh(p0, 2704 + 2 * (32 * cy + cx)) == 100);                       // centre: full amount
    CHECK(getsh(p0, 2704 + 2 * (32 * cy + cx + 1)) == 100 - (int)(1.0f * (100.0f / 4.0f)));  // distance 1
    CHECK(getsh(p0, 2704 + 2 * (32 * cy + cx + 3)) == 0);                      // outside the radius
    if (fails) return 1;
    puts("worldRunWater ok");
    return 0;
}
