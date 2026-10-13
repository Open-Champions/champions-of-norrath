#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../level-anim-load-common/host_check.hpp"
// Stubs record every callee; matrices are 12 floats (3x3 rotation then a translation row).
struct Call { int kind; float v[9]; int n[4]; };
static std::vector<Call> calls;
enum { ATT, IDENT, MTY, MUL, TRAIL, ADDFX, SWOOSH, START, PART, NM, SHPOS, ISIN, SPRITE, XFORM, LIVE, SLIGHT, MLIGHT };
static int shadowCount, liveResult, particlesBlood, particlesEcto, particlesSlime, particlesLightning, particlesIce, particlesFlame;
static const void* lastParticle;
extern "C" {
int levelFlags, frameNm, eRandState, g_BloodLevel;
unsigned char* haloTexture2;
unsigned char lightningSwordParticle[80], iceSwordParticle[80], newIceSwordParticle1[80], newIceSwordParticle2[80];
unsigned char newFlameSwordParticle1[80], newFlameSwordParticle2[80], newFlameSwordParticle3[80];
unsigned char particleWeaponBlood[80], particleWeaponEcto[80], particleWeaponSlime[80];
static void note(int kind, float a = 0, float b = 0, float c = 0, int n0 = 0, int n1 = 0, int n2 = 0, int n3 = 0) {
    Call k = {kind, {a, b, c, 0, 0, 0, 0, 0, 0}, {n0, n1, n2, n3}};
    calls.push_back(k);
}
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(unsigned char*, unsigned char*, int attach, float* m, float scale) {
    std::memset(m, 0, 48); m[0] = m[4] = m[8] = 1.0f; m[9] = scale; m[10] = 2 * scale; m[11] = 3 * scale;
    note(ATT, scale, 0, 0, attach);
}
void matIdent__FP8Matrix34(float* m) { std::memset(m, 0, 48); m[0] = m[4] = m[8] = 1.0f; note(IDENT); }
void matMakeTransYaw__FP8Matrix34G6Point3s(float* m, const float* p, short yaw) {
    std::memset(m, 0, 48); m[0] = m[4] = m[8] = 1.0f; m[9] = p[0]; m[10] = p[1]; m[11] = p[2]; note(MTY, p[0], p[1], p[2], yaw);
}
void matMul__FP8Matrix34N20(float* d, float* a, float* b) {
    float r[12]; for (int i = 0; i < 9; ++i) r[i] = a[i]; for (int k = 0; k < 3; ++k) r[9 + k] = a[9 + k] + b[9 + k];
    std::memcpy(d, r, 48); note(MUL);
}
void modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(unsigned char*, float* a, float* b) {
    a[0] = 0; a[1] = 0.5f; a[2] = 0; b[0] = 0; b[1] = 0.25f; b[2] = 0; note(TRAIL);
}
void addEffect__FiG6Point3(int kind, const float* p) { note(ADDFX, p[0], p[1], p[2], kind); }
void P_ExtendSwoosh__FiG6Point3T1f(int id, const float* a, const float* b, float w) {
    Call k = {SWOOSH, {a[0], a[1], a[2], b[0], b[1], b[2], w, 0, 0}, {id, 0, 0, 0}}; calls.push_back(k);
}
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    note(START, c, 0, 0, a, b, d, e + f + g + h); return 9;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* def, float* p, float* v, float f) {
    if (def == particleWeaponBlood) ++particlesBlood; else if (def == particleWeaponEcto) ++particlesEcto;
    else if (def == particleWeaponSlime) ++particlesSlime; else if (def == lightningSwordParticle) ++particlesLightning;
    else if (def == iceSwordParticle) ++particlesIce; else ++particlesFlame;
    lastParticle = def; note(PART, f, p[2], v[2]);
}
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char*) { note(NM); return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char*, float* m, int index, float* pos, float* h) {
    pos[0] = (float)index; pos[1] = 0; pos[2] = m[11]; *h = 10.0f; note(SHPOS, 0, 0, 0, index);
}
float isin__Fi(int a) { note(ISIN, 0, 0, 0, a); return 2.0f; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const float* p, float a, float b, int c, int d, int e, int f, unsigned char*, short g, int h) {
    Call k = {SPRITE, {p[0], a, b, (float)c, (float)d, (float)e, (float)f, (float)g, (float)h}, {0, 0, 0, 0}}; calls.push_back(k);
}
void matTransform__FP6Point3P8Matrix34T0(float* out, float* m, float* in) {
    float r[3] = {in[0] + m[9], in[1] + m[10], in[2] + m[11]}; std::memcpy(out, r, 12); note(XFORM);
}
int P_LightningIsLive__Fi(int id) { note(LIVE, 0, 0, 0, id); return liveResult; }
int P_StartLightning__FG6Point3T0iiffUi(const float* a, const float* b, int c, int d, float e, float f, unsigned g) {
    note(SLIGHT, a[0], b[0], e, c, d, (int)g); return 55;
}
void P_MoveLightning__FiG6Point3T1ii(int id, const float* a, const float* b, int c, int d) { note(MLIGHT, a[0], b[0], 0, id, c, d); }
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    unsigned char* desc, unsigned char* wm, unsigned char* st, const float* position, short yaw, unsigned char* am,
    unsigned char* anim, float scaleA, int i1, int bone, float scaleB, const float* mtx);
}
#define RUN(...) fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(__VA_ARGS__)
static bool near(float a, float b, float t = 1e-4f) { return std::fabs(a - b) <= t; }
static int count(int kind) { int n = 0; for (auto& c : calls) n += c.kind == kind; return n; }
int main() {
    alignas(8) unsigned char desc[128], wm[64], st[96], am[16], anim[16];
    const float point[3] = {10, 20, 30};
    auto arm = [&] {
        calls.clear(); std::memset(desc, 0, sizeof desc); std::memset(wm, 0, sizeof wm); std::memset(st, 0, sizeof st);
        levelFlags = 0; frameNm = 100; eRandState = 1; g_BloodLevel = 0; shadowCount = 3; liveResult = 0;
        particlesBlood = particlesEcto = particlesSlime = particlesLightning = particlesIce = particlesFlame = 0;
        wm[19] = 5;
    };
    auto setMod = [&](int k, int type, int value) { sh_(desc + 36 + 8 * k, type); sh_(desc + 38 + 8 * k, value); };
    // Early exits leave everything alone.
    arm(); levelFlags = 0x10000000; st[60] = 7;
    RUN(desc, wm, st, point, 0, nullptr, anim, 1.0f, 1, 0, 1.0f, nullptr); if (!calls.empty() || st[60] != 7) return 1;
    arm(); RUN(desc, nullptr, st, point, 0, nullptr, anim, 1.0f, 1, 0, 1.0f, nullptr); if (!calls.empty()) return 2;
    // Nothing active: identity matrix, st[60] cleared, previous matrix replaced.
    arm(); st[60] = 9; sf_(st, 5.0f);
    RUN(desc, wm, st, point, 0, nullptr, anim, 1.0f, 0, 0, 1.0f, nullptr);
    if (st[60] != 0 || calls.size() != 1 || calls[0].kind != IDENT || lf_(st) != 1.0f || lf_(st + 36) != 0.0f) return 3;
    // st[60] counts up to 100 with i1 set (signed compare).
    arm(); st[60] = 98;
    RUN(desc, wm, st, point, 0, am, anim, 0.0f, 1, 2, 1.0f, nullptr); if (st[60] != 99) return 4;
    RUN(desc, wm, st, point, 0, am, anim, 0.0f, 1, 2, 1.0f, nullptr); if (st[60] != 100) return 5;
    RUN(desc, wm, st, point, 0, am, anim, 0.0f, 1, 2, 1.0f, nullptr); if (st[60] != 100) return 6;
    arm(); st[60] = 0xff;     // signed -1: increments to zero
    RUN(desc, wm, st, point, 0, am, anim, 0.0f, 1, 2, 1.0f, nullptr); if (st[60] != 0) return 7;
    // Attached matrix: attachment index = (signed char)wm[19] + bone, translation scaleB * (1,2,3) + point.
    arm(); wm[19] = 0xfe;
    RUN(desc, wm, st, point, 100, am, anim, 0.0f, 1, 4, 2.0f, nullptr);
    if (calls[0].kind != ATT || calls[0].n[0] != 2 || calls[1].kind != MTY || calls[1].n[0] != 100 + 0x4000 || calls[2].kind != MUL) return 8;
    if (!near(lf_(st + 36), 12.0f) || !near(lf_(st + 40), 24.0f) || !near(lf_(st + 44), 36.0f)) return 9;
    // A given matrix replaces the computed one (and clears the trail id in non-X mode).
    arm(); float given[12]; for (int i = 0; i < 12; ++i) given[i] = 0.5f * i; sw_(st + 56, 9); st[61] = 2; st[62] = 3;
    RUN(desc, wm, st, point, 0, am, anim, 0.0f, 1, 0, 1.0f, given);
    if (lf_(st + 20) != 2.5f || hw(st + 56) != 0 || st[61] != 0 || st[62] != 0) return 10;
    // Trail start: st[56] is zero -> P_StartTrail, then P_ExtendSwoosh(9, H2, H, w) where H = A + B * 0.5,
    // H2 = A + B * 0.25, B = M[3..5] (given: 1.5, 2.0, 2.5), A = M[9..11].
    arm(); for (int i = 0; i < 12; ++i) given[i] = 0.0f;
    sf_(st + 12, 0.0f); sf_(st + 16, 0.0f); sf_(st + 20, 0.0f);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    // identity M: row1 = (0,1,0)? matIdent writes m[3..5] = 0, 1, 0 (index 4); A = point (via matMul with the yaw matrix).
    if (count(START) != 1 || count(SWOOSH) != 1) return 11;
    for (auto& c : calls) if (c.kind == START && (!near(c.v[0], 0x1.999998p-3f) || c.n[0] != 3 || c.n[1] != 1000 || c.n[2] != 2 || c.n[3] != 384)) return 12;
    for (auto& c : calls) if (c.kind == SWOOSH) {
        // A is the translation: identity + translation point (10,20,30); B = (0,1,0).
        if (c.n[0] != 9 || !near(c.v[0], 10) || !near(c.v[1], 20.25f) || !near(c.v[2], 30) || !near(c.v[3], 10) || !near(c.v[4], 20.5f)) return 13;
        // w = min(clamp(|B - C| * 0.68) * 0.3, st[60] * 0.05): |B - C| = 1, so 0.68 * 0.3 = 0.204; cap 0.05.
        if (!near(c.v[6], 0.05f)) return 14;
    }
    // Continuing trail (st[56] set and st[48] == frameNm - 1): an extra extension first, with the same id.
    arm(); sw_(st + 56, 4); sw_(st + 48, 99); st[60] = 99;
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (count(START) != 0 || count(SWOOSH) != 2 || hw(st + 56) != 4) return 15;
    // Not continuing when st[48] is anything else.
    arm(); sw_(st + 56, 4); sw_(st + 48, 98);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (count(START) != 1 || hw(st + 56) != 9) return 16;
    // Blood spray: kind 1 scatters 0 + 1 + 2 = 3 .. for countdown 2 -> iterations i = 0, 1 give 0 and 2 particles.
    arm(); g_BloodLevel = 1; st[62] = 2; st[61] = 1; sw_(st + 56, 4); sw_(st + 48, 99);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (particlesBlood != 2 || st[62] != 1) return 17;
    arm(); g_BloodLevel = 1; st[62] = 3; st[61] = 2; sw_(st + 56, 4); sw_(st + 48, 99);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (particlesEcto != 0 + 2 + 4 || st[62] != 2) return 18;
    arm(); g_BloodLevel = 1; st[62] = 2; st[61] = 3; sw_(st + 56, 4); sw_(st + 48, 99);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (particlesSlime != 2 || st[62] != 1) return 19;
    arm(); g_BloodLevel = 1; st[62] = 2; st[61] = 4; RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (particlesBlood + particlesEcto + particlesSlime != 0 || st[62] != 1) return 20;
    arm(); g_BloodLevel = 0; st[62] = 5; st[61] = 1; RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (particlesBlood != 0 || st[62] != 4) return 21;
    // Halo sprites: one per shadow point; the colour quad then decays by 2 toward zero.
    arm(); sh_(desc + 16, 5); sh_(desc + 18, (unsigned short)-5); sh_(desc + 20, 1); sh_(desc + 22, 2);
    sh_(desc + 24, 100); sh_(desc + 26, (unsigned short)-100); frameNm = 0x40d;
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 0, 0, 1.0f, nullptr);
    if (count(SPRITE) != 3) return 22;
    for (auto& c : calls) if (c.kind == SPRITE) {
        if (!near(c.v[1], -0x1.9b645ap+3f) || !near(c.v[2], 2.0f * 250.0f + 10000.0f - 2013.0f) || c.v[3] != 5 || c.v[4] != -5 || c.v[5] != 1 || c.v[6] != 2) return 23;
        if (c.v[7] != (float)(short)((0x40d & 0x3ff) << 6) || c.v[8] != 3) return 24;
    }
    {
        short q[4]; std::memcpy(q, desc + 16, 8);
        if (q[0] != 3 || q[1] != -3 || q[2] != 0 || q[3] != 0) return 25;
        std::memcpy(q, desc + 24, 8);
        if (q[0] != 98 || q[1] != -98 || q[2] != 0) return 26;
    }
    // Lightning: mod 24 with scaleA > 0 and the previous matrix = identity (so the sweep length is the
    // translation distance).
    arm(); setMod(2, 24, 1); st[0] = 0; sf_(st, 1.0f); sf_(st + 16, 1.0f); sf_(st + 32, 1.0f); liveResult = 0;
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.5f, 0, 0, 1.0f, nullptr);
    if (count(SLIGHT) != 1 || count(MLIGHT) != 0 || hw(st + 52) != 55) return 27;
    if (particlesLightning < 8) return 28;
    for (auto& c : calls) if (c.kind == SLIGHT && (c.n[0] != 107 || c.n[1] != 4 || !near(c.v[2], 0x1.f1eb84p+2f) || (unsigned)c.n[2] != 0x5e808080u)) return 29;
    arm(); setMod(0, 24, 1); sw_(st + 52, 7); liveResult = 1;
    RUN(desc, wm, st, point, 0, nullptr, anim, 1.0f, 0, 0, 1.0f, nullptr);
    if (count(MLIGHT) != 1 || count(SLIGHT) != 0 || hw(st + 52) != 7) return 30;
    for (auto& c : calls) if (c.kind == MLIGHT && (c.n[0] != 7 || c.n[1] != 94 || c.n[2] != -1)) return 31;
    arm(); setMod(0, 24, 1); sw_(st + 52, 7);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 0, 0, 1.0f, nullptr);
    if (hw(st + 52) != 0 || count(SLIGHT) + count(MLIGHT) != 0) return 32;
    // Ice and flame: counts follow the sweep length formula (translation distance 36-unit z-offset cancels).
    arm(); setMod(1, 22, 1); sf_(st, 1.0f); sf_(st + 16, 1.0f); sf_(st + 32, 1.0f); sf_(st + 36, 0.0f);
    RUN(desc, wm, st, point, 0, nullptr, anim, 1.0f, 0, 0, 1.0f, nullptr);
    {
        const float len = std::sqrt(10.0f * 10 + 20 * 20 + 30 * 30) > 30.0f ? 30.0f : 0.0f;
        const int steps = (int)((float)((int)(len * 0x1.a428f4p+3f) + 26) * 1.0f);
        if (particlesIce != steps) { std::printf("ice %d want %d\n", particlesIce, steps); return 33; }
    }
    arm(); setMod(1, 21, 1); sf_(st, 1.0f); sf_(st + 16, 1.0f); sf_(st + 32, 1.0f);
    RUN(desc, wm, st, point, 0, nullptr, anim, 1.0f, 0, 0, 1.0f, nullptr);
    {
        const int steps = (int)((float)((int)(30.0f * 0x1.a428f4p+3f) + 16) * 1.0f);
        if (particlesFlame < steps + steps / 16 * 0 || particlesFlame != steps + 2 * ((steps + 15) / 16)) { std::printf("flame %d steps %d\n", particlesFlame, steps); return 34; }
    }
    // Tail: with i1 set the frame number is recorded.
    arm(); RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr); if (hw(st + 48) != 100) return 35;
    arm(); sw_(st + 48, 5); RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 0, 0, 1.0f, nullptr); if (hw(st + 48) != 5) return 36;
    // Mode X: the effect fires when the random remainder is zero.
    arm(); levelFlags = 0x100;
    unsigned x = 1;
    for (; x < 100000; ++x) { unsigned s = x * 0x19660du + 0x3c6ef35fu; if ((s >> 16) % 6u == 0) break; }
    sf_(st + 12, 0.0f); sf_(st + 16, 1.0f); sf_(st + 20, 0.0f);    // C = B = (0,1,0): length 0 -> k = 0 -> mod 6
    eRandState = (int)x;
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (count(ADDFX) != 1) return 37;
    for (auto& c : calls) if (c.kind == ADDFX && (c.n[0] != 17 || !near(c.v[0], 10) || !near(c.v[1], 20.25f) || !near(c.v[2], 30))) return 38;
    arm(); levelFlags = 0x100; eRandState = 1; sf_(st + 16, 1.0f);
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 1, 0, 1.0f, nullptr);
    if (count(ADDFX) != 0 || (unsigned)eRandState != 1u * 0x19660du + 0x3c6ef35fu) return 39;
    arm(); levelFlags = 0x100; sw_(st + 56, 4); st[61] = 2; st[62] = 3;
    RUN(desc, wm, st, point, 0, nullptr, anim, 0.0f, 0, 0, 1.0f, nullptr);
    if (hw(st + 56) != 0 || st[61] != 0 || st[62] != 0) return 40;
    return 0;
}
