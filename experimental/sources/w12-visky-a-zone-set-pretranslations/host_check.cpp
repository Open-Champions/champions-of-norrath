// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// The vector array pointer is a 32-bit word mapped onto an arena (base 0x100000).
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_pretranslations.cpp"

static std::string trace;
static int result;
static float seen[40];
extern "C" int SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(unsigned char* raster, int count, unsigned char* out, const unsigned char* eye) {
    trace += "set:" + std::to_string(raster - arena) + "," + std::to_string(count) + "," + std::to_string(out - arena) + "," + std::to_string(eye - arena) + ";";
    std::memcpy(seen, out, sizeof seen);
    return result;
}

static void putf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static float getf(unsigned off) { float f; std::memcpy(&f, arena + off, 4); return f; }
static void putw(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
// zone at 0x100 (count +84, array pointer +88, output +208), camera at 0x800 (eye at +28),
// zone vectors at 0x2000, raster at 0x3000.
static void arm(int count) {
    std::memset(arena, 0x2e, sizeof arena);
    putw(0x100 + 84, count); putw(0x100 + 88, static_cast<int>(arena_base + 0x2000));
    for (int i = 0; i < 4; ++i) { putf(0x2000 + 12 * i, 10.0f * (i + 1)); putf(0x2004 + 12 * i, 20.0f + i); putf(0x2008 + 12 * i, -5.5f * i); }
    putf(0x800 + 28, 1.0f); putf(0x800 + 32, 2.0f); putf(0x800 + 36, 0.5f);
    trace.clear();
}

int main() {
    arm(3); result = 0;
    if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(arena + 0x100, arena + 0x3000, arena + 0x800) != 0) return 1;
    if (trace != "set:12288,3,464,2076;") return 2;
    for (int i = 0; i < 3; ++i)
        if (getf(0x100 + 208 + 12 * i) != 10.0f * (i + 1) - 1.0f || getf(0x100 + 212 + 12 * i) != (20.0f + i) - 2.0f ||
            getf(0x100 + 216 + 12 * i) != -5.5f * i - 0.5f) return 3;
    if (std::memcmp(seen, arena + 0x100 + 208, sizeof seen) != 0) return 4;
    // The fourth slot and the bytes before the array are untouched.
    if (arena[0x100 + 208 + 36] != 0x2e || arena[0x100 + 207] != 0x2e) return 5;
    // A negative callee result is returned as -1; any other as 0.
    arm(1); result = -9;
    if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(arena + 0x100, arena + 0x3000, arena + 0x800) != -1) return 6;
    arm(1); result = 17;
    if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(arena + 0x100, arena + 0x3000, arena + 0x800) != 0) return 7;
    // Zero and negative counts: no vector is written, the call is still made with the count.
    arm(0); result = 0;
    if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(arena + 0x100, arena + 0x3000, arena + 0x800) != 0 || trace != "set:12288,0,464,2076;") return 8;
    if (arena[0x100 + 208] != 0x2e) return 9;
    arm(-2); result = 0;
    SetPretranslations__6VIZoneR8VIRasterR8VICamera(arena + 0x100, arena + 0x3000, arena + 0x800);
    if (trace != "set:12288,-2,464,2076;" || arena[0x100 + 208] != 0x2e) return 10;
    return 0;
}
