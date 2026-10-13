#include "../w11-rend-a-common/rend.hpp"
// Observed evaluateAnimation__FP14AnimationStateb at 0x00161df0, 2476 bytes. Arguments: an animation
// state st and a flag. State words: +0 pointer to a result record R, signed byte +4 (the layer count n),
// and n layers of 32 bytes from +8. Layer words: +0 header pointer (header+0 is the bone count, +12 a
// pointer to eight bytes per bone, +16 a pointer to one signed byte per bone, +72 a 64-bit bone mask),
// +4 data pointer D (bone entries of 60 bytes from D+16), floats +8, +16 and +24, halfword +28 (flags
// 0x100, 0x800, 0x4000). Result records (1568 bytes or more) hold: output from +16, 24 bytes per bone;
// word +4 and halfword +8 (a DMA-style header), +1552 owner state, +1556 and +1560 list links, +1564 the
// frame number. Globals: arList and arListBack (newest and oldest record), drawFrameNm, emergencyResult.
//  1. Record choice: if arListBack equals arList, or no record from the back up to (not including) arList
//     has a frame number below drawFrameNm - 3, then flag set clears st+0 and returns; flag clear uses the
//     emergencyResult record. Otherwise st+0 is the first such record.
//  2. Base layer: from the top layer k = n - 1 downward, layers with min(f8, f16) >= 1.0 and f24 >= 0.0
//     AND their header mask into an accumulator; the first layer at which it reaches zero is the base. No
//     such layer (including n <= 0) clears st+0 and returns.
//  3. The record is detached from its old owner (the owner's st+0 is cleared), owned by st, and moved to
//     the front of the list.
//  4. updateAnimation__FP13animComponent(layer) runs on the base layer. For each bone b (60-byte entry E):
//     t = f24 - (float)(short)E[14]; quaternion (x,y,z,w) = E[40..52] + E[56..68] * t, stored at block b
//     (32 bytes; block+16..24 is the position E[16..24] + E[28..36] * (f24 - (float)(short)E[12])). Bone
//     0 gets extractRotation__FP4Quat when flag 0x4000 is set and its position x and y are zero when
//     flag 0x800 is set.
//  5. Each upper layer with f24 >= 0: alpha = clamp(min(f8, f16), 0, 1), through evalHermite__Ffffff(alpha,
//     0, 1, 0, 0) when flag 0x100 is set, skipped when below 0x1.0624dcp-10. After updateAnimation, each
//     bone whose bit is clear in the layer header's mask blends its own quaternion (negated when the sum
//     is closer than the difference; w wrapped by 2 pi into a half-turn window) and position into the
//     block with weights (1 - alpha, alpha).
//  6. The record gets header word ((3n + 1) << 16) | 0x6d008000 at +4 and n at +8; the VU0 program 0x10
//     is called, quatNormalize__FP4Quat(block 0) runs and for each bone the program 0x20 is called with
//     the block quaternion and position (vf5, vf12), a scale vector of the header's eight bytes (four
//     halfwords times 64) and parent index * 3; the next block's quaternion is renormalised meanwhile and
//     three rows of four halfwords are stored at R+16+24b. With n < 64, 24 zero bytes follow.
// Nothing is returned.
// Inferred: pose blending of animation layers into a matrix record for the VU. Unknown: what the VU0
// programs compute (they are not executed or known here), the rest of the layouts, and the units of
// the angle window. Nothing here was executed on the EE.
extern "C" {
extern unsigned char* arList;
extern unsigned char* arListBack;
extern int drawFrameNm;
extern unsigned char emergencyResult[];
void updateAnimation__FP13animComponent(unsigned char* layer);
void extractRotation__FP4Quat(float* quat);
float evalHermite__Ffffff(float a, float b, float c, float d, float e);
void quatNormalize__FP4Quat(float* quat);
}
// COP2 (VU0 macro mode) access. On the EE these are single instructions; a host check supplies a software
// model with the same call order.
#ifdef NORRATH_HOST_ARENA
extern "C" {
void norrath_vu_lqc2(int reg, const void* quad);
void norrath_vu_sqc2(int reg, void* quad);
void norrath_vu_ctc2(int reg, int value);
void norrath_vu_vcallms(int program);
}
#define VU_LQC2(REG, ADDRESS) norrath_vu_lqc2(REG, ADDRESS)
#define VU_SQC2(REG, ADDRESS) norrath_vu_sqc2(REG, ADDRESS)
#define VU_CTC2_VI1(VALUE) norrath_vu_ctc2(1, VALUE)
#define VU_VCALLMS(PROGRAM) norrath_vu_vcallms(PROGRAM)
#else
#define VU_LQC2(REG, ADDRESS) __asm__ volatile("lqc2 $vf" #REG ",0(%0)" ::"r"(ADDRESS) : "memory")
#define VU_SQC2(REG, ADDRESS) __asm__ volatile("sqc2 $vf" #REG ",0(%0)" ::"r"(ADDRESS) : "memory")
#define VU_CTC2_VI1(VALUE) __asm__ volatile("ctc2 %0,$vi1" ::"r"(VALUE))
#define VU_VCALLMS(PROGRAM) __asm__ volatile("vcallms " #PROGRAM ::: "memory")
#endif
namespace {
using namespace rend;
typedef unsigned long long __attribute__((may_alias)) u64;
}  // namespace
extern "C" void evaluateAnimation__FP14AnimationStateb(unsigned char* st, bool flag) {
    unsigned char* record = nullptr;
    unsigned char* back = arListBack;
    if (back != arList) {
        const int limit = drawFrameNm - 3;
        record = back;
        if (!(at<i32>(record, 1564) < limit)) {
            for (;;) {
                record = ptr(record, 1560);
                if (record == arList) {
                    record = nullptr;
                    break;
                }
                if (at<i32>(record, 1564) < limit) break;
            }
        }
    }
    if (record != nullptr) {
        set_ptr(st, 0, record);
    } else {
        if (flag) {
            at<u32>(st, 0) = 0;
            return;
        }
        set_ptr(st, 0, emergencyResult);
    }
    int k = at<i8>(st, 4) - 1;
    if (k < 0) {
        at<u32>(st, 0) = 0;
        return;
    }
    u64 mask = ~0ull;
    for (;;) {
        unsigned char* layer = st + 8 + 32 * k;
        const float a = at<f32>(layer, 8);
        const float b = at<f32>(layer, 16);
        const float m = a < b ? a : b;
        if (1.0f <= m) {
            if (0.0f <= at<f32>(layer, 24)) {
                mask = mask & at<u64>(ptr(layer, 0), 72);
                if (mask == 0) break;
            }
        }
        --k;
        if (k < 0) {
            at<u32>(st, 0) = 0;
            return;
        }
    }
    unsigned char* R = ptr(st, 0);
    unsigned char* owner = ptr(R, 1552);
    if (owner != nullptr) {
        at<u32>(owner, 0) = 0;
        R = ptr(st, 0);
    }
    unsigned char* following = ptr(R, 1560);
    set_ptr(R, 1552, st);
    if (following != nullptr) {
        unsigned char* previous = ptr(R, 1556);
        if (previous == nullptr) arListBack = following;
        else set_ptr(previous, 1560, following);
        unsigned char* head = arList;
        unsigned char* newer = ptr(R, 1560);
        unsigned char* older = ptr(R, 1556);
        set_ptr(newer, 1556, older);
        set_ptr(R, 1556, head);
        set_ptr(head, 1560, R);
        arList = R;
        at<u32>(R, 1560) = 0;
    }
    float blocks[64 * 8 + 8] __attribute__((aligned(16)));
    unsigned char* layer = st + 8 + 32 * k;
    const int bones = at<i32>(ptr(layer, 0), 0);
    updateAnimation__FP13animComponent(layer);
    unsigned char* data = ptr(layer, 4);
    if (bones > 0) {
        const float weight = at<f32>(layer, 24);
        for (int b = 0; b < bones; ++b) {
            unsigned char* E = data + 16 + 60 * b;
            float* block = blocks + 8 * b;
            const float t = weight - static_cast<float>(at<i16>(E, 14));
            const float qx = at<f32>(E, 56) * t;
            const float qy = at<f32>(E, 60) * t;
            const float qz = at<f32>(E, 64) * t;
            const float qw = at<f32>(E, 68) * t;
            block[0] = at<f32>(E, 40) + qx;
            block[1] = at<f32>(E, 44) + qy;
            block[2] = at<f32>(E, 48) + qz;
            block[3] = at<f32>(E, 52) + qw;
            if (b == 0 && (at<u16>(layer, 28) & 0x4000) != 0) extractRotation__FP4Quat(block);
            const float u = weight - static_cast<float>(at<i16>(E, 12));
            if (b == 0 && (at<u16>(layer, 28) & 0x800) != 0) {
                block[4] = 0.0f;
                block[5] = 0.0f;
            } else {
                block[4] = at<f32>(E, 16) + at<f32>(E, 28) * u;
                block[5] = at<f32>(E, 20) + at<f32>(E, 32) * u;
            }
            block[6] = at<f32>(E, 24) + at<f32>(E, 36) * u;
        }
    }
    unsigned char* top = layer;
    for (++k; k < at<i8>(st, 4); ++k) {
        top = st + 8 + 32 * k;
        const float weight = at<f32>(top, 24);
        if (weight < 0.0f) continue;
        float alpha = at<f32>(top, 8) < at<f32>(top, 16) ? at<f32>(top, 8) : at<f32>(top, 16);
        if (alpha < 0.0f) alpha = 0.0f;
        if (1.0f < alpha) alpha = 1.0f;
        if ((at<u16>(top, 28) & 0x100) != 0) alpha = evalHermite__Ffffff(alpha, 0.0f, 1.0f, 0.0f, 0.0f);
        if (alpha < 0x1.0624dcp-10f) continue;
        updateAnimation__FP13animComponent(top);
        unsigned char* data2 = ptr(top, 4);
        for (int b = 0; b < bones; ++b) {
            if (((at<u64>(ptr(top, 0), 72) >> b) & 1) != 0) continue;
            unsigned char* E = data2 + 16 + 60 * b;
            float* block = blocks + 8 * b;
            const float t = weight - static_cast<float>(at<i16>(E, 14));
            const float rx = at<f32>(E, 56) * t;
            const float ry = at<f32>(E, 60) * t;
            const float rz = at<f32>(E, 64) * t;
            const float rw = at<f32>(E, 68) * t;
            float q2[4];
            q2[0] = at<f32>(E, 40) + rx;
            q2[1] = at<f32>(E, 44) + ry;
            q2[2] = at<f32>(E, 48) + rz;
            q2[3] = at<f32>(E, 52) + rw;
            if (b == 0 && (at<u16>(top, 28) & 0x4000) != 0) extractRotation__FP4Quat(q2);
            const float x1 = block[0], y1 = block[1], z1 = block[2], w1 = block[3];
            float x2 = q2[0], y2 = q2[1], z2 = q2[2], w2 = q2[3];
            const float dminus = ((w1 - w2) * (w1 - w2) + (x1 - x2) * (x1 - x2)) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
            const float dplus = ((w1 + w2) * (w1 + w2) + (x1 + x2) * (x1 + x2)) + (y1 + y2) * (y1 + y2) + (z1 + z2) * (z1 + z2);
            if (dplus < dminus) {
                x2 = -x2;
                y2 = -y2;
                z2 = -z2;
                w2 = -w2;
            }
            while (0x1.921fb4p+1f < w2 - w1) w2 = w2 - 0x1.921fb4p+2f;
            while (w1 - w2 < -0x1.921fb4p+1f) w2 = w2 + 0x1.921fb4p+2f;
            const float inv = 1.0f - alpha;
            const float u = weight - static_cast<float>(at<i16>(E, 12));
            block[3] = w1 * inv + w2 * alpha;
            block[0] = x1 * inv + x2 * alpha;
            block[1] = y1 * inv + y2 * alpha;
            block[2] = z1 * inv + z2 * alpha;
            if (b == 0 && (at<u16>(top, 28) & 0x800) != 0) {
                block[4] = block[4] * inv;
                block[5] = block[5] * inv;
            } else {
                block[4] = block[4] * inv + (at<f32>(E, 16) + at<f32>(E, 28) * u) * alpha;
                block[5] = block[5] * inv + (at<f32>(E, 20) + at<f32>(E, 32) * u) * alpha;
            }
            block[6] = block[6] * inv + (at<f32>(E, 24) + at<f32>(E, 36) * u) * alpha;
        }
    }
    unsigned char* R2 = ptr(st, 0);
    unsigned char* hdr = ptr(top, 0);
    unsigned char* scales = ptr(hdr, 12);
    unsigned char* parents = ptr(hdr, 16);
    at<u32>(R2, 4) = static_cast<unsigned>((3 * bones + 1) << 16) | 0x01000000u | 0x60000000u | 0x0c008000u;
    at<u16>(R2, 8) = static_cast<unsigned short>(bones);
    VU_VCALLMS(0x10);
    quatNormalize__FP4Quat(blocks);
    unsigned char* out = R2 + 16;
    int b = 0;
    for (; b < bones; ++b) {
        float* block = blocks + 8 * b;
        const int parent3 = 3 * at<i8>(parents, b);
        const u64 h = at<u64>(scales, 8 * b);
        int words[4] __attribute__((aligned(16)));
        for (int i = 0; i < 4; ++i) words[i] = static_cast<int>(static_cast<short>(h >> (16 * i))) << 6;
        VU_LQC2(5, block);
        VU_LQC2(12, block + 4);
        VU_LQC2(10, words);
        VU_CTC2_VI1(parent3);
        VU_VCALLMS(0x20);
        float* next = block + 8;
        const float nx = next[0], ny = next[1], nz = next[2], nw = next[3];
        const float r = 1.0f / __builtin_sqrtf(nw * nw + nx * nx + ny * ny + nz * nz);
        next[2] = nz * r;
        next[3] = nw * r;
        next[0] = nx * r;
        next[1] = ny * r;
        unsigned rows[3][4] __attribute__((aligned(16)));
        VU_SQC2(25, rows[0]);
        VU_SQC2(26, rows[1]);
        VU_SQC2(27, rows[2]);
        for (int row = 0; row < 3; ++row)
            for (int i = 0; i < 4; ++i) at<u16>(out, 24 * b + 8 * row + 2 * i) = static_cast<unsigned short>(rows[row][i]);
    }
    if (b < 64) {
        unsigned char* tail = out + 24 * b;
        for (int i = 0; i < 12; ++i) at<u16>(tail, 2 * i) = 0;
    }
}
