// Host check only; not a build input. The matrix stubs are identity-like so the expected box can be computed by hand.
#include "recompute_static_bbox.cpp"
#include <cmath>
#include <cstring>
static int rot_calls, mul_calls, dtor_flag;
static float last_angles[3];
extern "C" void* RotateHPR__10VIMatrix44RC7VIVect3(void* m, const void* a) { rot_calls++; std::memcpy(last_angles, a, 12); return m; }
extern "C" void* Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void*, const void* in, void* out) { mul_calls++; std::memcpy(out, in, 12); return out; }
extern "C" void _$_10VIMatrix44(void*, int f) { dtor_flag = f; }
static void wr(unsigned char* o, int off, float f) { std::memcpy(o + off, &f, 4); }
static float rd(unsigned char* o, int off) { float f; std::memcpy(&f, o + off, 4); return f; }
int main() {
    alignas(16) static unsigned char e[1024];
    // all stages off: V2 = stage1 vector * K, V1 = 0 on every iteration; box = [min(0, V2) - H, max(0, V2) + H]
    wr(e, 200, 2.0f); wr(e, 204, -1.0f); wr(e, 208, 1.0f); wr(e, 212, 1.0f);        // K = (2+1) * (1+1) = 6
    wr(e, 228, 1.0f); wr(e, 232, -1.0f); wr(e, 236, 3.0f); wr(e, 240, -1.0f);       // H = 0.5 * max(4, 2) = 2
    wr(e, 848, 1.0f); wr(e, 852, -2.0f); wr(e, 856, 0.0f);
    for (int o = 916; o < 940; o += 4) wr(e, o, 77.0f);
    RecomputeStaticBBox__17VIParticleEmitter(e);
    if (rot_calls || mul_calls || dtor_flag != 2) return 1;
    const float expect_low[3] = {0.0f - 2.0f, -12.0f - 2.0f, 0.0f - 2.0f};
    const float expect_high[3] = {6.0f + 2.0f, 0.0f + 2.0f, 0.0f + 2.0f};
    for (int i = 0; i < 3; i++) if (rd(e, 916 + 4 * i) != expect_low[i] || rd(e, 928 + 4 * i) != expect_high[i]) return 2;
    // stage 1 on: 12 rotations and multiplications
    e[888] = 1; rot_calls = mul_calls = 0;
    RecomputeStaticBBox__17VIParticleEmitter(e);
    if (rot_calls != 12 || mul_calls != 12) return 3;
    // the last iteration is i = 11: m = 2, phase 3, angle = min(pi, E[868]) * 1.0 in component 2
    wr(e, 868, 9.0f); rot_calls = 0;
    RecomputeStaticBBox__17VIParticleEmitter(e);
    if (last_angles[0] != 0.0f || last_angles[1] != 0.0f || std::fabs(last_angles[2] - 3.1415927f) > 1e-6f) return 4;
    // stage 2 adds a translation: V1 = M * (0 + (1, 1, 1)) = (1, 1, 1) with identity Mul; stage 3 repeats with its own vector
    e[888] = 0; e[884] = 1; wr(e, 824, 1.0f); wr(e, 828, 1.0f); wr(e, 832, 1.0f);
    rot_calls = mul_calls = 0;
    RecomputeStaticBBox__17VIParticleEmitter(e);
    if (rot_calls != 12 || mul_calls != 24) return 5;
    // V1 = (1,1,1) and V2 = (6,-12,0) go through the stub unchanged: C = V1 + V2 = (7,-11,1) widens the box and V1 does too
    if (rd(e, 916) != 0.0f - 2.0f || rd(e, 928) != 7.0f + 2.0f || rd(e, 932) != 1.0f + 2.0f) return 6;
    return 0;
}
