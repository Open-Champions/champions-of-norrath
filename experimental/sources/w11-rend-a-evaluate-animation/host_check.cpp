#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../level-anim-load-common/host_check.hpp"
// Target addresses: records and states live in an arena at 0x100000; emergencyResult at 0x200000.
static unsigned char arena[0x20000];
extern "C" {
unsigned char* arList;
unsigned char* arListBack;
int drawFrameNm;
unsigned char emergencyResult[0x700];
unsigned char* norrath_host_pointer(unsigned a) { return a >= 0x200000u ? emergencyResult + (a - 0x200000u) : arena + (a - 0x100000u); }
unsigned norrath_host_address(const unsigned char* p) {
    if (p >= emergencyResult && p < emergencyResult + sizeof emergencyResult) return 0x200000u + unsigned(p - emergencyResult);
    return 0x100000u + unsigned(p - arena);
}
}
// Software model of the VU0 registers used.
static unsigned vf[32][4];
static int vi1;
struct Prog { int program; unsigned q[4], scale[4], pos[4]; int vi; };
static std::vector<Prog> progs;
static std::vector<int> events;
extern "C" {
void norrath_vu_lqc2(int reg, const void* p) { std::memcpy(vf[reg], p, 16); }
void norrath_vu_sqc2(int reg, void* p) { std::memcpy(p, vf[reg], 16); }
void norrath_vu_ctc2(int reg, int v) { if (reg == 1) vi1 = v; }
void norrath_vu_vcallms(int program) {
    Prog p; p.program = program; std::memcpy(p.q, vf[5], 16); std::memcpy(p.scale, vf[10], 16); std::memcpy(p.pos, vf[12], 16); p.vi = vi1;
    progs.push_back(p);
    for (int row = 0; row < 3; ++row) for (int j = 0; j < 4; ++j) vf[25 + row][j] = 0xabc00000u + (unsigned)(progs.size() * 256 + row * 16 + j);
}
void updateAnimation__FP13animComponent(unsigned char* layer) { events.push_back(1000 + (int)(unsigned)(layer - arena)); }
void extractRotation__FP4Quat(float* q) { events.push_back(2000); q[0] = q[0] + 100.0f; }
float evalHermite__Ffffff(float a, float, float, float, float) { events.push_back(3000); return a * 0.5f; }
void quatNormalize__FP4Quat(float*) { events.push_back(4000); }
void evaluateAnimation__FP14AnimationStateb(unsigned char* st, bool flag);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    unsigned char* st = arena + 0x100;
    unsigned char* hdr = arena + 0x1000;
    unsigned char* hdr2 = arena + 0x1100;
    unsigned char* data = arena + 0x2000;
    unsigned char* data2 = arena + 0x3000;
    unsigned char* scales = arena + 0x4000;
    unsigned char* parents = arena + 0x4400;
    unsigned char* rec0 = arena + 0x8000;   // oldest
    unsigned char* rec1 = arena + 0x9000;   // newest
    const int nb = 2;
    auto arm = [&] {
        progs.clear(); events.clear(); std::memset(vf, 0, sizeof vf); vi1 = 0;
        std::memset(arena, 0, sizeof arena); std::memset(emergencyResult, 0, sizeof emergencyResult);
        sw_(rec0 + 1560, A(rec1)); sw_(rec1 + 1556, A(rec0)); sw_(rec0 + 1564, 50); sw_(rec1 + 1564, 60);
        arListBack = rec0; arList = rec1; drawFrameNm = 100;
        st[4] = 1; sw_(st + 8, A(hdr)); sw_(st + 12, A(data));
        sf_(st + 16, 1.0f); sf_(st + 24, 1.0f); sf_(st + 32, 0.5f); sh_(st + 36, 0);
        sw_(hdr, nb); sw_(hdr + 12, A(scales)); sw_(hdr + 16, A(parents)); std::memset(hdr + 72, 0, 8);
        for (int b = 0; b < nb; ++b) {
            unsigned char* e = data + 16 + 60 * b;
            sh_(e + 14, 0); sh_(e + 12, 0);
            sf_(e + 40, 0.1f * (b + 1)); sf_(e + 44, 0.2f); sf_(e + 48, 0.3f); sf_(e + 52, 0.4f);
            sf_(e + 56, 1.0f); sf_(e + 60, 2.0f); sf_(e + 64, 3.0f); sf_(e + 68, 4.0f);
            sf_(e + 16, 5.0f + b); sf_(e + 20, 6.0f); sf_(e + 24, 7.0f); sf_(e + 28, 0.5f); sf_(e + 32, 1.0f); sf_(e + 36, 2.0f);
            for (int h = 0; h < 4; ++h) { scales[8 * b + 2 * h] = (unsigned char)(h + 1 + b); scales[8 * b + 2 * h + 1] = b ? 0xff : 0; }
            parents[b] = (unsigned char)(b == 0 ? 0xff : 0);
        }
    };
    // A record older than drawFrameNm - 3 is chosen (the oldest first).
    arm(); evaluateAnimation__FP14AnimationStateb(st, true);
    if (hw(st) != A(rec1) && hw(st) != A(rec0)) return 1;
    // First check: oldest 50 < 97 -> rec0 chosen; it is moved to the front and owned by st.
    if (hw(st) != A(rec0) || arList != rec0 || arListBack != rec1 || hw(rec0 + 1560) != 0 || hw(rec0 + 1556) != A(rec1) || hw(rec1 + 1560) != A(rec0) || hw(rec0 + 1552) != A(st)) return 2;
    if (hw(rec1 + 1556) != 0) return 3;
    // All records recent: flag set clears st+0, flag clear uses emergencyResult.
    arm(); sw_(rec0 + 1564, 98); sw_(rec1 + 1564, 99);
    sw_(st, 0x1234); evaluateAnimation__FP14AnimationStateb(st, true);
    if (hw(st) != 0 || !progs.empty()) return 4;
    arm(); sw_(rec0 + 1564, 98); sw_(rec1 + 1564, 99);
    evaluateAnimation__FP14AnimationStateb(st, false);
    if (hw(st) != 0x200000u || hw(emergencyResult + 1552) != A(st)) return 5;
    // Only one record (back == arList): same as none.
    arm(); arListBack = rec1; evaluateAnimation__FP14AnimationStateb(st, true); if (hw(st) != 0) return 6;
    // A middle record is chosen when the oldest is recent; the newest (arList) never is. Its owner word is cleared.
    arm(); unsigned char* rec2 = arena + 0xa000;
    sw_(rec0 + 1564, 99); sw_(rec0 + 1560, A(rec2)); sw_(rec2 + 1556, A(rec0)); sw_(rec2 + 1560, A(rec1)); sw_(rec1 + 1556, A(rec2));
    sw_(rec2 + 1564, 60); sw_(rec2 + 1552, A(arena + 0x6000)); sw_(arena + 0x6000, 0x77);
    evaluateAnimation__FP14AnimationStateb(st, true);
    if (hw(st) != A(rec2) || hw(arena + 0x6000) != 0 || arList != rec2 || arListBack != rec0 || hw(rec0 + 1560) != A(rec1)) return 7;
    if (hw(rec1 + 1556) != A(rec0) || hw(rec2 + 1556) != A(rec1) || hw(rec1 + 1560) != A(rec2) || hw(rec2 + 1560) != 0) return 7;
    // No layers: cleared.
    arm(); st[4] = 0; evaluateAnimation__FP14AnimationStateb(st, true); if (hw(st) != 0) return 8;
    // The single-layer base: bone quaternions = base + rate * weight(0.5) and positions; output rows follow.
    arm(); evaluateAnimation__FP14AnimationStateb(st, true);
    unsigned char* R = rec0;
    if (hw(R + 4) != ((((3 * nb + 1) << 16) | 0x6d008000u)) || (hw(R + 8) & 0xffff) != (unsigned)nb) return 9;
    if (progs.size() != 1 + nb || progs[0].program != 0x10 || progs[1].program != 0x20 || progs[2].program != 0x20) return 10;
    // bone 0: quaternion (0.1 + 0.5, 0.2 + 1.0, 0.3 + 1.5, 0.4 + 2.0); position (5 + 0.5*0.5, 6 + 1*0.5, 7 + 2*0.5)
    float q0[4]; std::memcpy(q0, progs[1].q, 16);
    float p0[4]; std::memcpy(p0, progs[1].pos, 16);
    // vf5 holds block 0 after quatNormalize (stubbed, so unchanged).
    if (!near(q0[0], 0.6f) || !near(q0[1], 1.2f) || !near(q0[2], 1.8f) || !near(q0[3], 2.4f)) return 11;
    if (!near(p0[0], 5.25f) || !near(p0[1], 6.5f) || !near(p0[2], 8.0f)) return 12;
    if (progs[1].vi != -3 || progs[2].vi != 0) return 13;       // parent index times 3
    // vf10: four halfwords of the bone's eight bytes shifted by 6 (sign extended).
    if ((int)progs[1].scale[0] != (1 << 6) || (int)progs[1].scale[3] != (4 << 6)) return 14;
    if ((int)progs[2].scale[0] != (int)(short)0xff02 * 64) return 15;
    // Bone 1's quaternion was renormalised after program 1 started: the vcall for bone 1 sees a unit vector.
    float q1[4]; std::memcpy(q1, progs[2].q, 16);
    const float len = std::sqrt(q1[0] * q1[0] + q1[1] * q1[1] + q1[2] * q1[2] + q1[3] * q1[3]);
    if (!near(len, 1.0f)) return 16;
    // Output rows are the low halfwords of the stub's rows: 3 rows x 4 halfwords per bone, then 24 zero bytes.
    for (int b = 0; b < nb; ++b)
        for (int row = 0; row < 3; ++row)
            for (int j = 0; j < 4; ++j) {
                const unsigned want = (0xabc00000u + (unsigned)((b + 2) * 256 + row * 16 + j)) & 0xffff;
                if ((hw(R + 16 + 24 * b + 8 * row + 2 * j) & 0xffff) != want) return 17;
            }
    for (int i = 0; i < 24; ++i) if (R[16 + 24 * nb + i] != 0) return 18;
    // The upper layer blends in with alpha = min(1.0, 1.0) clamped, hermite when 0x100 is set.
    arm(); st[4] = 2; sw_(st + 40, A(hdr2)); sw_(st + 44, A(data2));
    sf_(st + 48, 1.0f); sf_(st + 56, 1.0f); sf_(st + 64, 0.5f); sh_(st + 68, 0x100);
    sw_(hdr2, nb); sw_(hdr2 + 12, A(scales)); sw_(hdr2 + 16, A(parents));
    std::memset(hdr2 + 72, 0, 8); hdr2[72] = 2;     // bone 1 skipped in the upper layer
    // The base search: top layer mask is 2 (bone 1), base layer mask 0 -> base layer 0.
    sf_(st + 16, 1.0f); sf_(st + 24, 1.0f);
    for (int b = 0; b < nb; ++b) {
        unsigned char* e = data2 + 16 + 60 * b;
        sh_(e + 14, 0); sh_(e + 12, 0);
        sf_(e + 40, 1.0f); sf_(e + 44, 1.0f); sf_(e + 48, 1.0f); sf_(e + 52, 0.0f);
        sf_(e + 56, 0); sf_(e + 60, 0); sf_(e + 64, 0); sf_(e + 68, 0);
        sf_(e + 16, 11.0f); sf_(e + 20, 12.0f); sf_(e + 24, 13.0f); sf_(e + 28, 0); sf_(e + 32, 0); sf_(e + 36, 0);
    }
    evaluateAnimation__FP14AnimationStateb(st, true);
    // Hermite stub halves alpha: 1.0 -> 0.5; bone 0 blends 50/50 with (1,1,1,0) (negated if closer to the sum).
    bool hermite = false; for (int e : events) hermite |= e == 3000; if (!hermite) return 19;
    float b0[4]; std::memcpy(b0, progs[1].q, 16);
    // base q = (0.6, 1.2, 1.8, 2.4); upper q2 = (1, 1, 1, 0); sums: |q1 + q2|^2 vs |q1 - q2|^2 -> negate q2
    // when the sum is smaller: it is not here, so q2 stays; blend 0.5 * q1 + 0.5 * q2.
    if (!near(b0[0], 0.8f) || !near(b0[1], 1.1f) || !near(b0[2], 1.4f) || !near(b0[3], 1.2f)) return 20;
    float pos0[4]; std::memcpy(pos0, progs[1].pos, 16);
    if (!near(pos0[0], 0.5f * 5.25f + 0.5f * 11.0f) || !near(pos0[1], 0.5f * 6.5f + 0.5f * 12.0f) || !near(pos0[2], 0.5f * 8.0f + 0.5f * 13.0f)) return 21;
    // Bone 1 is skipped in the upper layer (mask bit 1): its block keeps the base quaternion, renormalised.
    float b1[4]; std::memcpy(b1, progs[2].q, 16);
    const float len1 = std::sqrt(b1[0] * b1[0] + b1[1] * b1[1] + b1[2] * b1[2] + b1[3] * b1[3]);
    if (!near(len1, 1.0f) || !near(b1[0] / b1[1], 0.7f / 1.2f)) return 22;
    return 0;
}
