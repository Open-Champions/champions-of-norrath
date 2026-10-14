// Observed msg_run__19ParticleMorphEffect at 0x00152e30, 1952 bytes. "A" is the object pointed to
// by the word at +84 and "B" the one at +88; frameNm is the global frame counter. K0..K6 are the
// float constants of the small-data pool read by the body (K0 = K3 = K5 bits 0x42c7ffff, K4 bits
// 0x4247ffff, K1 = K6 bits 0x3ca3d70b, K2 bits 0x3e89b9f9). "vcall128(X)" is the table entry at
// (word X+72) + 128 called on X adjusted by the entry's signed halfword, returning a model
// header; nPoints(h) is modelGetNmShadowPoints__FP12_modelHeader. "step" is a 32-bit
// congruential step on the word at object+68, seed*0x19660d + 0x3c6ef35f, yielding seed>>16.
// "shadowPos(X, i, out)" is modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
// called with (vcall128(X), X+288, the three floats at X+16 by value, halfword X+56, i, out, null,
// null, float X+224).
//  1. When frameNm is below the word at +76 the phases below run, else (when the object pointer
//     is non-null) the table entry at (word +72) + 8 is called with 3 and the routine ends.
//  2. e = frameNm - word(+80). When e as float is below K0 (phase one): t = min(1, e*K1);
//     A's halfwords are set: +152 and +144 to 0, +140 and +148 to int(t*220), +142 and +150 to
//     int(t*20), +146 and +154 to int(t*250). Then nA = nPoints(vcall128(A)), nB for B, and 20
//     times: p1 = shadowPos(A, step % nA), p2 = shadowPos(B, step % nB) and
//     P_AddParticle(&morpheffectParticle1, &p1, &((p2 - p1) * 0.268997), 0.0).
//  3. Then e is taken again: when e is below K3 and above K4 (phase two): objectUpdateInGrid(B),
//     byte(B+570) &= 0xf7, nA and nB are fetched again and 20 times: p1, p2 as above (the
//     points are written to different locals), a point u is drawn from the unit ball by rejection
//     (three steps give each component as 2*(step * 2^-16) - 1, tried again while the squared
//     length exceeds 1), and P_AddParticle(&morpheffectParticle2, &p1, &v, 0.0) and
//     P_AddParticle(&morpheffectParticle2, &p2, &v, 0.0) are called with v = u * 16. The seed word
//     is stored after the accepted draw. Afterwards the halfwords of A and B at +140/+148 are 220,
//     +142/+150 are 20, +146/+154 are 250 and +144/+152 are 0, and e is taken again.
//  4. When e as float is above K5 (phase three): s = max(0, 1 - (e - K5)*K6); B's halfwords are set
//     like phase one with s in place of t, objectRemoveFromGrid(A) runs and byte(A+570) |= 8.
// Phase one runs only when e is below K0. Phases two and three are reached from the end of
// phase one, and phase three is also reached directly when e is not below K0. Nothing here ran on
// the EE; what the fields mean is unknown.
#include "../w10-actor-k-common/access.hpp"

extern "C" {
extern int frameNm;
int modelGetNmShadowPoints__FP12_modelHeader(void* header);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* header, void* animation, Point3 position, short angle, int index, Point3* out, float* f, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectRemoveFromGrid__FP10GameObject(void* object);
extern unsigned char morpheffectParticle1[] __asm__("morpheffectParticle1");
extern unsigned char morpheffectParticle2[] __asm__("morpheffectParticle2");
}

namespace {
constexpr float bits(unsigned value) { return __builtin_bit_cast(float, value); }
using w10k::half;
using w10k::real;
using w10k::word;

const float K0 = bits(0x42c7ffffu);
const float K1 = bits(0x3ca3d70bu);
const float K2 = bits(0x3e89b9f9u);
const float K3 = bits(0x42c7ffffu);
const float K4 = bits(0x4247ffffu);
const float K5 = bits(0x42c7ffffu);
const float K6 = bits(0x3ca3d70bu);

unsigned char* pointer(unsigned char* o, unsigned offset) { return w10k::from_word(word(o, offset)); }

void* header_of(unsigned char* x) { return w10k::from_word(w10k::call_entry(x, 128)); }

unsigned step(unsigned char* o) {
    const unsigned w = word(o, 68) * 0x19660du + 0x3c6ef35fu;
    word(o, 68) = w;
    return w >> 16;
}

void shadow_pos(unsigned char* x, unsigned r, int count, Point3* out) {
    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
        header_of(x), x + 288, *reinterpret_cast<Point3*>(x + 16), half(x, 56), static_cast<int>(r) % count, out, nullptr, nullptr, real(x, 224));
}

void set_colour(unsigned char* x, float t) {
    half(x, 152) = 0;
    half(x, 144) = 0;
    const int r = static_cast<int>(t * 220.0f);
    const int g = static_cast<int>(t * 20.0f);
    const int b = static_cast<int>(t * 250.0f);
    half(x, 140) = static_cast<short>(r);
    half(x, 142) = static_cast<short>(g);
    half(x, 148) = static_cast<short>(r);
    half(x, 150) = static_cast<short>(g);
    half(x, 146) = static_cast<short>(b);
    half(x, 154) = static_cast<short>(b);
}

void set_final_colour(unsigned char* x) {
    half(x, 150) = 20;
    half(x, 142) = 20;
    half(x, 148) = 220;
    half(x, 146) = 250;
    half(x, 140) = 220;
    half(x, 152) = 0;
    half(x, 144) = 0;
    half(x, 154) = 250;
}
}  // namespace

extern "C" void msg_run__19ParticleMorphEffect(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(frameNm < w10k::sword(o, 76))) {
        if (o) w10k::call_entry(o, 8, 3);
        return;
    }
    int e = frameNm - w10k::sword(o, 80);
    if (static_cast<float>(e) < K0) {
        float t = static_cast<float>(e) * K1;
        if (1.0f < t) t = 1.0f;
        set_colour(pointer(o, 84), t);
        int nA = modelGetNmShadowPoints__FP12_modelHeader(header_of(pointer(o, 84)));
        int nB = modelGetNmShadowPoints__FP12_modelHeader(header_of(pointer(o, 88)));
        for (int i = 0; i < 20; ++i) {
            Point3 p1, p2;
            shadow_pos(pointer(o, 84), step(o), nA, &p1);
            shadow_pos(pointer(o, 88), step(o), nB, &p2);
            Point3 d;
            d.x = p2.x - p1.x;
            d.y = p2.y - p1.y;
            d.z = p2.z - p1.z;
            d.x *= K2;
            d.y *= K2;
            d.z *= K2;
            P_AddParticle__FP11ParticleDefP6Point3T1f(morpheffectParticle1, &p1, &d, 0.0f);
        }
        e = frameNm - w10k::sword(o, 80);
        if (static_cast<float>(e) < K3 && K4 < static_cast<float>(e)) {
            objectUpdateInGrid__FP10GameObject(pointer(o, 88));
            pointer(o, 88)[570] &= 0xf7;
            nA = modelGetNmShadowPoints__FP12_modelHeader(header_of(pointer(o, 84)));
            nB = modelGetNmShadowPoints__FP12_modelHeader(header_of(pointer(o, 88)));
            for (int i = 0; i < 20; ++i) {
                Point3 p1, p2;
                shadow_pos(pointer(o, 84), step(o), nA, &p1);
                shadow_pos(pointer(o, 88), step(o), nB, &p2);
                Point3 u;
                unsigned seed = word(o, 68);
                float x, y, z;
                do {
                    seed = seed * 0x19660du + 0x3c6ef35fu;
                    x = static_cast<float>(seed >> 16) * bits(0x37800000u);
                    seed = seed * 0x19660du + 0x3c6ef35fu;
                    y = static_cast<float>(seed >> 16) * bits(0x37800000u);
                    seed = seed * 0x19660du + 0x3c6ef35fu;
                    z = static_cast<float>(seed >> 16) * bits(0x37800000u);
                    x = x + x - 1.0f;
                    y = y + y - 1.0f;
                    z = z + z - 1.0f;
                } while (1.0f < x * x + y * y + z * z);
                word(o, 68) = seed;
                u.x = x;
                u.y = y;
                u.z = z;
                Point3 v;
                v.x = u.x * 16.0f;
                v.y = u.y * 16.0f;
                v.z = u.z * 16.0f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(morpheffectParticle2, &p1, &v, 0.0f);
                P_AddParticle__FP11ParticleDefP6Point3T1f(morpheffectParticle2, &p2, &v, 0.0f);
            }
            set_final_colour(pointer(o, 84));
            set_final_colour(pointer(o, 88));
            e = frameNm - w10k::sword(o, 80);
        }
    }
    if (K5 < static_cast<float>(e)) {
        float s = 1.0f - (static_cast<float>(e) - K5) * K6;
        if (s < 0.0f) s = 0.0f;
        set_colour(pointer(o, 88), s);
        objectRemoveFromGrid__FP10GameObject(pointer(o, 84));
        pointer(o, 84)[570] |= 8;
    }
}
