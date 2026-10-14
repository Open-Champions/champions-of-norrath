// Host contract for Lamp msg_run; not an EE input. The EE pointer word and rsqrt.s are replaced by
// host equivalents (see the #if in msg_run.cpp); the numeric results are checked against an
// independent recomputation of the same single-precision steps.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" int frameNm = 0;
extern "C" unsigned char lampParticle[128] = {};
#include "msg_run.cpp"

static std::vector<int> rolls; static size_t rollIndex;
static float noiseValue; static int noiseArg; static std::string calls;
struct Part { unsigned char* position; float velocity[3]; float extra; void* def; };
static std::vector<Part> parts; static void* removed;
extern "C" {
float ef1Noise__Fi(int index) { calls += "noise;"; noiseArg = index; return noiseValue; }
int eRand__10GameObject(void*) { calls += "r"; return rolls.at(rollIndex++); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, void* p, void* v, float e) {
    calls += "|part;"; Part q; q.def = d; q.position = static_cast<unsigned char*>(p); std::memcpy(q.velocity, v, 12); q.extra = e; parts.push_back(q); }
void objectRemoveFromRunList__FP10GameObject(void* o) { calls += "remove;"; removed = o; }
}
static float comp(int r) { float v = static_cast<float>(r) * 0x1.0p-16f; v = v + v; return v - 1.0f; }
int main() {
    int bad = 0;
    alignas(16) unsigned char object[160];
    // first draw (60000,60000,60000) is outside the unit sphere and is rejected; the second is accepted
    const int pattern[] = {60000, 60000, 60000, 40000, 30000, 36000, 20000, 50000, 33000, 65536, 0, 32768};
    const unsigned char ages[] = {0, 5, 30, 31, 32, 127, 128, 255};
    const float noises[] = {0.0f, 0.5f, 1.0f, -0.25f};
    for (unsigned char age : ages) for (float n : noises) {
        std::memset(object, 0x6b, sizeof object); object[137] = age; object[136] = 1;
        rolls.assign(pattern, pattern + 12); rollIndex = 0; noiseValue = n; calls.clear(); parts.clear(); removed = nullptr; frameNm = 3;
        msg_run__4Lamp(object);
        const unsigned low = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object));
        if (noiseArg != static_cast<int>(0x007efe41u + low * 0x19660du + 48u)) { std::puts("fail: noise argument"); bad = 1; }
        const float size = static_cast<float>(static_cast<int>(n * 22.0f) + 216);
        float f28, f32, f36, f40; std::memcpy(&f28, object + 104, 4); std::memcpy(&f32, object + 108, 4); std::memcpy(&f36, object + 112, 4); std::memcpy(&f40, object + 116, 4);
        if (f28 != size || f36 != size || f32 != 1.0f / (size * size) || f40 != f32) { std::puts("fail: light floats"); bad = 1; }
        // rolls consumed: one rejected draw (3), then two accepted draws (3 + 3)
        if (parts.size() != 2 || rollIndex != 9) {
            std::printf("fail: particle count %zu or rolls %zu\n", parts.size(), rollIndex); bad = 1;
        }
        if (parts.size() == 2) {
            const int firstAccepted[3] = {40000, 30000, 36000}, second[3] = {20000, 50000, 33000};
            const int* sets[2] = {firstAccepted, second};
            for (int i = 0; i < 2; ++i) {
                float x = comp(sets[i][0]), y = comp(sets[i][1]), z = comp(sets[i][2]);
                float len2 = (x * x + y * y) + z * z;
                float scale = 1.0f / __builtin_sqrtf(len2);
                const float speed = 0x1.6c49bap+1f;
                float vx = x * scale * speed, vy = y * scale * speed, vz = z * scale * speed;
                if (std::fabs(parts[i].velocity[0] - vx) > 1e-5f || std::fabs(parts[i].velocity[1] - vy) > 1e-5f || std::fabs(parts[i].velocity[2] - vz) > 1e-5f) { std::puts("fail: velocity"); bad = 1; }
                if (parts[i].position != object + 16 || parts[i].extra != 0.0f || parts[i].def != lampParticle) { std::puts("fail: particle arguments"); bad = 1; }
            }
        }
        const bool remove = static_cast<signed char>(age) >= 31;
        if ((removed == object) != remove) { std::puts("fail: removal decision"); bad = 1; }
        if (object[136] != (remove ? 0 : 1)) { std::puts("fail: byte 136"); bad = 1; }
        if (object[137] != static_cast<unsigned char>(age + 1)) { std::puts("fail: byte 137"); bad = 1; }
    }
    if (!bad) std::puts("lamp msg_run ok");
    return bad;
}
