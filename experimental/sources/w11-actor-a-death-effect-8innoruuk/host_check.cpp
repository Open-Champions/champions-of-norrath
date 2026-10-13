// Host contract for deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff; not an EE
// input. eRand returns scripted values, the model and trig functions are stubs, and P_AddParticle records its
// arguments. The expected particles are recomputed here from the rule in death_effect.cpp. The authored function
// was also compared with the original in a private R5900 run. Build with -ffp-contract=off. Returns non-zero
// when wrong.
#include "death_effect.cpp"
#include <cstdio>
#include <cstring>
#include <vector>

unsigned char disintoInnoruukParticle[80];
static std::vector<int> rolls;
static size_t next_roll;
struct Particle { void* def; Point3 p, v; float age; };
static std::vector<Particle> added;
static int shadowCount;
static float values[8];
static Point3 outs[8];
static std::vector<int> posCalls;
extern "C" {
unsigned eRand__10GameObject(void*) { return static_cast<unsigned>(rolls[next_roll++ % rolls.size()]); }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, Point3, short, int i, Point3* out, float* v, void* m, float s) {
    posCalls.push_back(i); *out = outs[i]; *v = values[i]; if (m != nullptr || s != 2.5f) posCalls.push_back(-99);
}
float icos__Fi(int a) { return 0.001f * static_cast<float>(a % 1000); }
float isin__Fi(int a) { return -0.002f * static_cast<float>(a % 700); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) { added.push_back({d, *p, *v, age}); }
}
static float R(int r) { return static_cast<float>(r) * w11a::bits(0x37800000u); }
int main() {
    int bad = 0;
    alignas(16) static unsigned char self[64], other[64];
    for (int variant = 0; variant < 6; ++variant) {
        const float b = (variant & 1) ? 3.0f : 0.0f;
        const int heading = variant < 3 ? 70000 : 123;  // truncated to a short inside the function
        shadowCount = variant == 2 ? 1 : 6 - (variant == 4);
        const int n = shadowCount / 2;
        rolls.clear();
        for (int i = 0; i < 400; ++i) rolls.push_back(i * 104729 - 20000000);
        const float vs[3] = {0.0f, 21000.0f, 30000.0f};
        for (int i = 0; i < 8; ++i) { values[i] = vs[(i + variant) % 3]; outs[i] = Point3{float(i), float(2 * i), float(3 * i)}; }
        next_roll = 0; added.clear(); posCalls.clear();
        std::memset(other, 0, sizeof other);
        deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff(self, other, (variant == 5) ? nullptr : self, nullptr, Point3{1, 2, 3}, heading, 2.5f, b);
        if (variant == 5) {
            if (!added.empty() || other[12] != 0) { std::puts("fail: null model"); bad = 1; }
            continue;
        }
        if (other[12] != 2) { std::puts("fail: object bit"); bad = 1; }
        // Expected.
        std::vector<Particle> want;
        size_t at = 0;
        float total = 0;
        const short h = static_cast<short>(heading);
        for (int i = 0; i < n; ++i) {
            total += values[i] * values[i] * values[i];
            while (0.0f < total) {
                const float spread = values[i] * w11a::bits(0x3727c5acu) * w11a::bits(0x41759999u);
                total -= w11a::bits(0x54da475au);
                Point3 p{(R(rolls[at]) - 0.5f) * spread, (R(rolls[at + 1]) - 0.5f) * spread, (R(rolls[at + 2]) - 0.5f) * spread};
                p.x += outs[i].x; p.y += outs[i].y; p.z += outs[i].z;
                const float k = w11a::bits(0x40847ae1u);
                Point3 v{R(rolls[at + 3]) * k, R(rolls[at + 4]) * k, R(rolls[at + 5]) * k};
                if (b != 0.0f) { p.x += (0.001f * static_cast<float>(h % 1000)) * b; p.y += (-0.002f * static_cast<float>(h % 700)) * b; p.z += 0.0f; }
                float age = R(rolls[at + 6]) * w11a::bits(0x3ecccccc);
                at += 7;
                want.push_back({disintoInnoruukParticle, p, v, age});
            }
        }
        if (posCalls.size() != static_cast<size_t>(n)) { std::printf("fail: shadow call count variant %d\n", variant); bad = 1; }
        for (size_t i = 0; i < posCalls.size(); ++i) if (posCalls[i] != static_cast<int>(i)) { std::puts("fail: shadow call order"); bad = 1; }
        if (added.size() != want.size() || next_roll != at) { std::printf("fail: count variant %d got %zu want %zu\n", variant, added.size(), want.size()); bad = 1; continue; }
        for (size_t i = 0; i < want.size(); ++i) {
            const Particle &q = added[i], &w = want[i];
            if (q.def != w.def || q.p.x != w.p.x || q.p.y != w.p.y || q.p.z != w.p.z || q.v.x != w.v.x || q.v.y != w.v.y || q.v.z != w.v.z || q.age != w.age) {
                std::printf("fail: particle %zu variant %d\n", i, variant); bad = 1; break;
            }
        }
        if (variant == 0 && want.empty()) { std::puts("fail: no particles in the scripted case"); bad = 1; }
    }
    return bad;
}
