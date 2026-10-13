// Host contract for generateDust__7CyclopsG6Point3; not an EE input. eRand returns scripted 16-bit rolls, icos and
// isin return known values, and P_AddParticle records its arguments. The expected particles are computed here
// from the rule in generate_dust.cpp. The authored function was also compared with the original in a private R5900
// run. Build with -ffp-contract=off so the host does not fuse multiply-adds. Returns non-zero when wrong.
#include "generate_dust.cpp"
#include <cmath>
#include <cstdio>
#include <vector>

unsigned char hitParticleDust2[80];
static std::vector<unsigned> rolls;
static size_t next_roll;
struct Particle { void* def; Point3 p, v; float age; };
static std::vector<Particle> added;
static std::vector<int> angles;
extern "C" {
unsigned eRand__10GameObject(void*) { return rolls[next_roll++ % rolls.size()]; }
float icos__Fi(int a) { angles.push_back(a); return 0.001f * static_cast<float>(a % 1000); }
float isin__Fi(int a) { angles.push_back(a); return -0.002f * static_cast<float>(a % 700); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) { added.push_back({d, *p, *v, age}); }
}
static float comp(unsigned r) { float f = static_cast<float>(static_cast<int>(r)) * w11a::bits(0x37800000u); return f + f - 1.0f; }
int main() {
    int bad = 0;
    // Each iteration: one rejected triple (1,1,1 corner: all rolls 65535 -> components just under 1), one accepted
    // triple, then angle, x, y, z rolls.
    const unsigned accepted[3] = {20000, 40000, 33000};
    for (int variant = 0; variant < 3; ++variant) {
        rolls.clear();
        for (int i = 0; i < 50; ++i) {
            if (variant != 1) { rolls.push_back(65535); rolls.push_back(65535); rolls.push_back(65535); }
            rolls.push_back(accepted[0] + i * 7 + variant); rolls.push_back(accepted[1] - i * 5); rolls.push_back(accepted[2] + i);
            rolls.push_back(70000 + i * 4099 * (variant + 1));  // angle: truncated to short
            rolls.push_back(123 + i * 13); rolls.push_back(999 + i * 17); rolls.push_back(55 + i * 3);
        }
        next_roll = 0; added.clear(); angles.clear();
        alignas(16) static unsigned char object[64];
        Point3 pos{10.5f, -20.25f, 7.0f};
        generateDust__7CyclopsG6Point3(object, pos);
        if (added.size() != 50 || next_roll != rolls.size()) { std::printf("fail: count %zu rolls %zu/%zu\n", added.size(), next_roll, rolls.size()); return 1; }
        size_t at = 0;
        for (int i = 0; i < 50; ++i) {
            if (variant != 1) at += 3;
            const float k = w11a::bits(0x405ccccc);
            Point3 d{comp(rolls[at]) * k, comp(rolls[at + 1]) * k, comp(rolls[at + 2]) * k};
            at += 3;
            const short a = static_cast<short>(rolls[at++]);
            float x = static_cast<float>(rolls[at++] % 50u) * (0.001f * static_cast<float>(a % 1000)) + pos.x;
            float y = static_cast<float>(rolls[at++] % 50u) * (-0.002f * static_cast<float>(a % 700)) + pos.y;
            float z = static_cast<float>(static_cast<int>(rolls[at++] % 20u) - 10) + pos.z;
            const Particle& q = added[i];
            if (q.def != hitParticleDust2 || q.age != 0.0f || q.p.x != x || q.p.y != y || q.p.z != z || q.v.x != d.x || q.v.y != d.y || q.v.z != d.z) {
                std::printf("fail: particle %d variant %d\n", i, variant); bad = 1; break;
            }
        }
    }
    return bad;
}
