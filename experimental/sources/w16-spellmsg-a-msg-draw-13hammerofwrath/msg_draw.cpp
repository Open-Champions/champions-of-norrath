// Observed msg_draw__13HammerOfWrath at 0x002b4bf0, 1236 bytes. O is the object, P the point (three singles) at O+16, D the item description
// at O+88 and frameNm-like state is not used. G is the 16-byte-record table _4item$globalItem (0x0031f540, 1696 bytes); asyncRandState (0x00483e60)
// is a generator word: one draw is state = state * 0x19660d + 0x3c6ef35f followed by (state >> 16) taken as an integer. Pool constants with
// no symbols (bits): a = 0x3c088888 (about 1/120), b = 0x3727c5ac (about 1e-5), c = 0x420d6666 (35.35). The particle definition is an 80-byte
// bss object of the image (file-local symbol hammerofwrathParticle1 0x008d7a40), imported through an address-named view. Steps in order:
//  1. id = getProxyMeshId__4itemPQ24item11description(D) is used twice (the same argument): the model M is the word at G + 16 * id + 8 (read
//     after the first call) and the texture X the word at G + 16 * id2 + 12 (read after the second call and before the matrix call).
//  2. A local matrix is built with matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(m, copy of P, (short)(int)(the single at O+172),
//     (short)(int)(O+176), (short)(int)(O+180), 1.0). A second local matrix holds the twelve singles (0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0);
//     matMul__FP8Matrix34N20(second, second, first) multiplies them into the second.
//  3. modelSetHalo__FP7HaloDefi runs with a 16-byte record of four words 0x64a80000, 0x43c80000, 0x64003c3c, 0x42c80000 and the argument 2;
//     modelDraw runs with (M, X, 3, a copy of P, the second matrix, null animation, flags 0, null draw record, 0).
//  4. n = modelGetNmShadowPoints__FP12_modelHeader(M). With t = (float)(the signed word at O+168) * a limited to at most 1.0: the burst count
//     is 2 and the velocity scale k is 0.0, unless t < 1.0, when k = (1.0 - t) * 50.0 and the count is (int)((1.0 - t) * 100.0) + 2; and when
//     the word at O+168 equals 1 the count is 3000 and k = 100.0.
//  5. For each of the count bursts: one draw gives idx = (state >> 16) % n (signed); modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(M,
//     second matrix, idx, &point, &size) fills a point and a single; S = (size * b) * c; three further draws give point += ((r - 0.5) * S) with r the
//     draw times 2^-16, x then y then z; then a unit-ball vector is drawn with the generator (three draws per attempt, each 2 * (draw * 2^-16) - 1,
//     repeated while 1.0 < x*x + y*y + z*z; the state after the accepted attempt is stored); the velocity is that vector times k and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(definition, point, velocity, 0.0) runs.
// Inferred: a flaring hammer model that sheds particles from its shadow points, more of them as a counter at O+168 counts down. Unknown: the
// meaning of O+168, O+172..O+180 and the 16-byte records of G. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
extern unsigned char globalItem[] __asm__("_4item$globalItem");
extern unsigned asyncRandState;
extern unsigned char norrath_w16_spellmsg_a_view_8d7a40[80];
int getProxyMeshId__4itemPQ24item11description(void* description);
void matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(Matrix34* m, Point3 p, short yaw, short pitch, short roll, float scale);
void matMul__FP8Matrix34N20(Matrix34* out, Matrix34* a, Matrix34* b);
void modelSetHalo__FP7HaloDefi(void* halo, int mode);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(void* model, Matrix34* m, int index, Point3* out, float* size);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
inline unsigned advance(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }
inline float unit(unsigned state) { return static_cast<float>(state >> 16) * w12c::bits(0x37800000u); }
}

extern "C" void msg_draw__13HammerOfWrath(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int id = getProxyMeshId__4itemPQ24item11description(o + 88);
    int count = 2;
    void* const model = from_word(word(globalItem + 16 * id, 8));
    const int id2 = getProxyMeshId__4itemPQ24item11description(o + 88);
    Matrix34 first;
    Matrix34 second;
    for (int i = 0; i < 12; ++i) second.f[i] = 0.0f;
    second.f[1] = 1.0f;
    second.f[3] = 1.0f;
    second.f[8] = 1.0f;
    const Point3 yprAt = *reinterpret_cast<Point3*>(o + 16);
    void* const texture = from_word(word(globalItem + 16 * id2, 12));
    matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(&first, yprAt, static_cast<short>(static_cast<int>(real(o, 172))),
                                                          static_cast<short>(static_cast<int>(real(o, 176))), static_cast<short>(static_cast<int>(real(o, 180))), 1.0f);
    matMul__FP8Matrix34N20(&second, &second, &first);
    unsigned halo[4];
    halo[0] = 0x64a80000u;
    halo[1] = 0x43c80000u;
    halo[2] = 0x64003c3cu;
    halo[3] = 0x42c80000u;
    modelSetHalo__FP7HaloDefi(halo, 2);
    const Point3 drawAt = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(model, texture, 3, drawAt, &second, nullptr, 0ul, nullptr, 0);
    const int n = modelGetNmShadowPoints__FP12_modelHeader(model);
    float t = static_cast<float>(sword(o, 168)) * bits(0x3c088888u);
    if (1.0f < t) t = 1.0f;
    float k = 0.0f;
    if (t < 1.0f) {
        const float rest = 1.0f - t;
        k = rest * 50.0f;
        count = static_cast<int>(rest * 100.0f) + 2;
        if (sword(o, 168) == 1) {
            count = 3000;
            k = 100.0f;
        }
    }
    if (count <= 0) return;
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    for (int i = 0; i < count; ++i) {
        unsigned state = advance(asyncRandState);
        asyncRandState = state;
        Point3 point;
        float size;
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(model, &second, static_cast<int>(state >> 16) % n, &point, &size);
        const float scale = (size * bits(0x3727c5acu)) * bits(0x420d6666u);
        state = advance(asyncRandState);
        const float rx = unit(state);
        state = advance(state);
        const float ry = unit(state);
        state = advance(state);
        const float rz = unit(state);
        const Point3 offset = {(rx - 0.5f) * scale, (ry - 0.5f) * scale, (rz - 0.5f) * scale};
        point.x = point.x + offset.x;
        point.y = point.y + offset.y;
        point.z = point.z + offset.z;
        Point3 u;
        do {
            state = advance(state);
            const float ux = unit(state);
            state = advance(state);
            const float uy = unit(state);
            state = advance(state);
            const float uz = unit(state);
            u.x = (ux + ux) - 1.0f;
            u.y = (uy + uy) - 1.0f;
            u.z = (uz + uz) - 1.0f;
        } while (1.0f < u.x * u.x + u.y * u.y + u.z * u.z);
        asyncRandState = state;
        Point3 velocity = {u.x * k, u.y * k, u.z * k};
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d7a40, &point, &velocity, zero);
    }
}
