// Observed msg_draw__17UndeadKnightClone at 0x0023db88, 1380 bytes. R is the block at the word
// object+36, "anim" the animation state at +288, pos the point at +16, h56 the signed halfword at +56,
// s the float at +32 and len the float at +28. Part 1 runs for i = 0 to 63; E is the point at
// +960 + 12 i and W the point at +1728 + 12 i. A "ball point" is made from the global generator
// asyncRandState (state = state * 0x19660d + 0x3c6ef35f for every draw, each component being
// (float)(state >> 16) * 2^-16, doubled, minus 1.0; three draws in x, y, z order, repeated while
// 1.0 < x*x + y*y + z*z; the state is read once and written back once per i, before the sprite call):
//  1. v = ball point times 1.5 on every axis.
//  2. When (len * 0.8333329f * 6.25f) * len < E.x*E.x + E.y*E.y + E.z*E.z: v += E * -0.02f
//     (-0.019999999f, the word 0xbca3d70a).
//  3. W = (W + v) * 0.8f (word 0x3f4ccccc); E += W.
//  4. P_FaceSprite(at, 30.0, 500.0, 0, 0, 0, 200, haloTexture5, 0, 0) where at = E + pos with s *
//     0.75 added to its z.
// Part 2, for the word at +2556 non-zero and then (with index 1 and R[196], R[204]) for the word at
// +2696 non-zero: model = the table entry at (word +72) + 128 called; modelGetCharAttachmentPos(model,
// copy of pos, h56, anim, index, &A, null, the float at +224); modelGetCharAttachmentMatrix(R[12], copy
// of pos, h56, anim, index, &M, the float at +224); modelDraw(R[180], R[188], 1, A, M, anim, the
// doubleword at +552, null, 0) (index 0, R[180], R[188] for the first block).
// Unknown: what the 64 points are (they look like drifting particles around the object) and what the
// two blocks draw. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* position, float a, float b, int c, int d, int e, int f, void* texture, short g, int h);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 point, short yaw, void* state, int index, Point3* first, Point3* second, float scale);
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
    void* model, Point3 point, short yaw, void* state, int index, void* matrix, float scale);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 point, void* matrix, void* animation, unsigned long long flags, void* record, int last);
extern int asyncRandState;
extern void* haloTexture5;
}

namespace {
inline float component(unsigned& state) {
    state = state * 0x19660du + 0x3c6ef35fu;
    const float v = static_cast<float>(static_cast<int>(state >> 16)) * w10o::bits(0x37800000u);
    return v + v - 1.0f;
}
}  // namespace

extern "C" void msg_draw__17UndeadKnightClone(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    for (int i = 0; i < 64; ++i) {
        unsigned state = static_cast<unsigned>(asyncRandState);
        Point3 v;
        do {
            v.x = component(state);
            v.y = component(state);
            v.z = component(state);
        } while (1.0f < v.x * v.x + v.y * v.y + v.z * v.z);
        asyncRandState = static_cast<int>(state);
        float* const e = reinterpret_cast<float*>(o + 960 + 12 * i);
        float* const w = reinterpret_cast<float*>(o + 1728 + 12 * i);
        const float len = real(o, 28);
        const float limit = len * bits(0x3f55554fu) * 6.25f * len;
        const float magnitude = e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        v.x = v.x * 1.5f;
        v.y = v.y * 1.5f;
        v.z = v.z * 1.5f;
        if (limit < magnitude) {
            const Point3 pull = {e[0] * bits(0xbca3d70au), e[1] * bits(0xbca3d70au), e[2] * bits(0xbca3d70au)};
            v.x = v.x + pull.x;
            v.y = v.y + pull.y;
            v.z = v.z + pull.z;
        }
        w[0] = (w[0] + v.x) * bits(0x3f4cccccu);
        w[1] = (w[1] + v.y) * bits(0x3f4cccccu);
        w[2] = (w[2] + v.z) * bits(0x3f4cccccu);
        e[0] = e[0] + w[0];
        e[1] = e[1] + w[1];
        e[2] = e[2] + w[2];
        Point3 at;
        at.x = e[0] + real(o, 16);
        at.y = e[1] + real(o, 20);
        at.z = e[2] + real(o, 24);
        at.z = at.z + real(o, 32) * bits(0x3f400000u);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&at, 30.0f, 500.0f, 0, 0, 0, 200, haloTexture5, 0, 0);
    }
    void* const anim = o + 288;
    for (int index = 0; index < 2; ++index) {
        if (word(o, index == 0 ? 2556 : 2696) == 0) continue;
        void* const model = from_word(call_entry(o, 128));
        const Point3 first_point = *reinterpret_cast<Point3*>(o + 16);
        Point3 first;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            model, first_point, half(o, 56), anim, index, &first, nullptr, real(o, 224));
        unsigned char matrix[48];
        const Point3 second_point = *reinterpret_cast<Point3*>(o + 16);
        modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
            from_word(word(from_word(word(o, 36)), 12)), second_point, half(o, 56), anim, index, matrix, real(o, 224));
        unsigned char* const block = from_word(word(o, 36));
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            from_word(word(block, index == 0 ? 180 : 196)), from_word(word(block, index == 0 ? 188 : 204)), 1, first, matrix, anim, dword(o, 552), nullptr, 0);
    }
}
