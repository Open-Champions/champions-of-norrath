// Host contract for bounce__13FireBombletJr; not an EE input. Stubs record the allocation and the constructor call. The authored
// function was also compared with the original in a private R5900 run (200 scenarios with random positions, velocities, flags,
// frame numbers around multiples of 37, cutMode values and random-state words, with the allocation succeeding and failing; every
// original word covered).
#include "bounce.cpp"
#include <cstring>
#include <string>
signed char cutMode;
int frameNm;
unsigned syncRandState;
static std::string trace;
static bool fail_alloc;
static unsigned char block[1192];
static float spot_seen[3], a_seen, c_seen;
static int b_seen;
extern "C" {
void* blockAlloc__Fi(int size) { trace += "alloc" + std::to_string(size) + ";"; return fail_alloc ? nullptr : block; }
void __10StaticFireG6Point3fif(void* self, Point3 position, float a, int b, float c) {
    trace += self == block ? "fire;" : "fire?;";
    spot_seen[0] = position.x; spot_seen[1] = position.y; spot_seen[2] = position.z; a_seen = a; b_seen = b; c_seen = c;
}
}
static void setup(unsigned char* o, signed char flag) {
    std::memset(o, 0, 128);
    float v[6] = {8.0f, -4.0f, -6.0f, 1.0f, 2.0f, 3.0f};
    std::memcpy(o + 76, v, 12);
    std::memcpy(o + 16, v + 3, 12);
    o[88] = 9; o[89] = static_cast<unsigned char>(flag);
}
int main() {
    alignas(16) unsigned char o[128];
    float vel[3];
    // flag clear: counts up, halves the velocity, makes the new vertical speed positive, spawns nothing
    setup(o, 0); trace.clear(); cutMode = 1; frameNm = 74;
    bounce__13FireBombletJr(o);
    std::memcpy(vel, o + 76, 12);
    if (o[88] != 10 || vel[0] != 4.0f || vel[1] != -2.0f || vel[2] != 3.0f || !trace.empty()) return 1;
    // flag set, cutMode clear and frame not a multiple of 37: no spawn
    setup(o, 1); trace.clear(); cutMode = 0; frameNm = 38; syncRandState = 12345;
    bounce__13FireBombletJr(o);
    if (!trace.empty() || syncRandState != 12345u) return 2;
    // frame a multiple of 37 (also the negative one): spawn
    for (int frame : {0, 37, -74}) {
        setup(o, 1); trace.clear(); cutMode = 0; frameNm = frame; syncRandState = 0; fail_alloc = false;
        bounce__13FireBombletJr(o);
        if (trace != "alloc1192;fire;") return 3;
        // five steps of the generator from zero: the fifth draw selects the mode
        unsigned s = 0;
        unsigned r[5];
        for (int i = 0; i < 5; ++i) { s = s * 0x19660du + 0x3c6ef35fu; r[i] = s >> 16; }
        if (syncRandState != s || b_seen != static_cast<int>(120 + 60 * (r[4] % 3))) return 4;
        const float k = __builtin_bit_cast(float, 0x37800000u);
        const float x = 1.0f + (static_cast<float>(r[0]) * k - 0.5f) * (static_cast<float>(r[1]) * k * 40.0f + 10.0f);
        const float y = 2.0f + (static_cast<float>(r[2]) * k - 0.5f) * (static_cast<float>(r[3]) * k * 40.0f + 10.0f);
        if (spot_seen[0] != x || spot_seen[1] != y || spot_seen[2] != 3.0f || a_seen != 240.0f || c_seen != 0.5f) return 5;
    }
    // cutMode set: spawn regardless of the frame; a failed allocation spends four draws only
    setup(o, 1); trace.clear(); cutMode = -1; frameNm = 5; syncRandState = 1; fail_alloc = true;
    bounce__13FireBombletJr(o);
    unsigned s = 1;
    for (int i = 0; i < 4; ++i) s = s * 0x19660du + 0x3c6ef35fu;
    return trace == "alloc1192;" && syncRandState == s ? 0 : 6;
}
