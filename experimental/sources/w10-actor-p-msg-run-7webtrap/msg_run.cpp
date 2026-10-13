// Observed msg_run__7WebTrap at 0x0020ebd0, 1984 bytes. Field names are not established; offsets are
// used. Points: P (+16), Q (+84), C (+96), A (+108), B (+120). Words: S (+76), T (+80), L (+136, the
// first word of an embedded ListHead; the block it points to holds an object pointer at offset 12, K
// below), +144, +148 and +152 are integers, +156 a float, +132 a signed byte counter.
//
// 1. When S is zero: a ray starts at (P.x, P.y, P.z + 24) with direction Q - P. Repeatedly (ignoring
//    the object hit last, at first the trap itself) worldHitscanWithObjects is called with
//    (world, start, direction, 1200.0, ignored object, &hit, 0, 0) and returns a fraction. When it
//    hit an object whose flag word (+12) has bit 5 or 6 (mask 0x60) set, the object's run-time type
//    (called through slot 0 of its table, compared with __eq__C9type_infoRC9type_info) is checked
//    against BlackWidow and SpiderQueen; for any other type the trap sticks: S = 1, C = P +
//    direction * fraction, the word at +152 = 4, the object is linked into the list at +136, and the
//    loop ends. Otherwise start moves to start + direction * fraction, direction becomes Q - start,
//    and the loop repeats while an object was hit.
// 2. When L's block exists (even with S zero), with K its object: if the float at +156 is below the
//    float at K+28, +156 grows by 0.5. With the signed byte at +132 equal to 50 and bit 5 of K's flag
//    word set, K+756 = 3 * hypot(K.x - C.x, K.y - C.y) and the halfword at K+748 = -16384 -
//    iatan2(K.x - C.x, K.y - C.y). With the byte equal to 49 and the same bit set, K+756 = 0.
//    When the word at +148 is below frameNm, bit 6 (0x40) of K's flag word is set and the word at
//    K+1400 is 1: the word at +152 is decremented, or T is zeroed when it was below 2, and +148 =
//    frameNm + 17. When S is nonzero and L's block is missing, T = 0 instead.
// 3. When frameNm < T or T == -1: A = K's position (if S and L's block) else the midpoint of P and Q,
//    and B = A. Otherwise A and B are each pulled towards P / Q by 900 / 60 units along the
//    normalised direction from P to A (A) and from Q to B (B).
// 4. The byte at +132 is decremented when positive. When frameNm < T or T == -1: +132 = 50 and +144
//    grows by 2 while below 70; otherwise +144 shrinks by 2 (to 0 at most). When the byte is not
//    positive the object is destroyed through table byte offset 8 with flag 3.
// Inferred: a web that sticks to what it hits (K) and fades. Unknown: the field meanings, the
// 1/60 and 900 values (read as literals), and whether the missing-type branch (null hit) can occur.
// Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
extern unsigned char* world;
extern int frameNm;
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* world, Point3 start, Point3 direction, float range,
                                                                                         void* ignore, void** hit, int a, int b);
void* __tf10BlackWidow();
void* __tf11SpiderQueen();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
int iatan2__Fff(float y, float x);
}

namespace {
// Slot 0 of a table is called with no arguments and returns the run-time type record.
void* type_of(unsigned char* object) {
    const unsigned fn = *reinterpret_cast<unsigned*>(w10p::table_entry(object, 0) + 4);
#if defined(W10_ACTOR_P_HOST)
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w10_actor_p_host_call(fn, nullptr, 0, 0, 0)));
#else
    return reinterpret_cast<void* (*)()>(static_cast<__UINTPTR_TYPE__>(fn))();
#endif
}

void copy_point(Point3& to, const Point3& from) { to = from; }
}  // namespace

extern "C" void msg_run__7WebTrap(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3& p = *reinterpret_cast<Point3*>(o + 16);
    Point3& q = *reinterpret_cast<Point3*>(o + 84);
    Point3& c = *reinterpret_cast<Point3*>(o + 96);
    Point3& a = *reinterpret_cast<Point3*>(o + 108);
    Point3& b = *reinterpret_cast<Point3*>(o + 120);
    unsigned node;

    if (word(o, 76) == 0u) {
        Point3 start = {p.x, p.y, p.z + 24.0f};
        Point3 direction = {q.x - p.x, q.y - p.y, q.z - p.z};
        void* hit = self;
        void* ignore = self;
        for (;;) {
            const float fraction = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
                world, start, direction, 1200.0f, ignore, &hit, 0, 0);
            if (hit != nullptr && (word(static_cast<unsigned char*>(hit), 12) & 0x60u) != 0u) {
                void* type = type_of(static_cast<unsigned char*>(hit));
                void* known = __tf10BlackWidow();
                if ((__eq__C9type_infoRC9type_info(type, known) ^ 1) != 0) {
                    if (hit == nullptr) {
                        __throw_bad_typeid();
                    } else {
                        type = type_of(static_cast<unsigned char*>(hit));
                    }
                    known = __tf11SpiderQueen();
                    if ((__eq__C9type_infoRC9type_info(type, known) ^ 1) != 0) {
                        const float dx = direction.x * fraction;
                        const float dy = direction.y * fraction;
                        const float dz = direction.z * fraction;
                        const Point3 spot = {p.x + dx, p.y + dy, p.z + dz};
                        word(o, 76) = 1u;
                        copy_point(c, spot);
                        sword(o, 152) = 4;
                        listOnewayLink__FP8ListHeadT0Pv(o + 136, hit, hit);
                        hit = nullptr;
                    }
                }
            }
            const Point3 step = {direction.x * fraction, direction.y * fraction, direction.z * fraction};
            const Point3 moved = {start.x + step.x, start.y + step.y, start.z + step.z};
            copy_point(start, moved);
            const Point3 remaining = {q.x - start.x, q.y - start.y, q.z - start.z};
            copy_point(direction, remaining);
            if (hit == nullptr) break;
            ignore = hit;
        }
    }

    node = word(o, 136);
    if (node == 0u) {
        if (word(o, 76) != 0u) word(o, 80) = 0u;
        goto after_attachment;
    }
    {
        unsigned char* k = from_word(word(from_word(node), 12));
        if (real(o, 156) < real(k, 28)) real(o, 156) = real(o, 156) + 0.5f;
        const int counter = static_cast<signed char>(o[132]);
        if (counter == 50 && (word(from_word(word(from_word(node), 12)), 12) & 0x20u) != 0u) {
            unsigned char* const pos = from_word(word(from_word(node), 12)) + 16;
            const float ex = real(pos, 0) - c.x;
            const float ey = real(pos, 4) - c.y;
            real(from_word(word(from_word(node), 12)), 756) = __builtin_sqrtf(ex * ex + ey * ey) * 3.0f;
            const int angle = iatan2__Fff(real(pos, 0) - c.x, real(from_word(word(from_word(node), 12)), 20) - c.y);
            node = word(o, 136);
            half(from_word(word(from_word(node), 12)), 748) = static_cast<short>(-16384 - angle);
        } else if (counter == 49) {
            unsigned char* const target = from_word(word(from_word(node), 12));
            if ((word(target, 12) & 0x20u) != 0u) word(target, 756) = 0u;
        }
        if (sword(o, 148) < frameNm) {
            unsigned char* const target = from_word(word(from_word(node), 12));
            if ((word(target, 12) & 0x40u) != 0u && sword(target, 1400) == 1) {
                if (sword(o, 152) < 2) {
                    word(o, 80) = 0u;
                } else {
                    sword(o, 152) = sword(o, 152) - 1;
                }
                sword(o, 148) = frameNm + 17;
            }
        }
    }
after_attachment:
    {
        const int timer = sword(o, 80);
        if (frameNm < timer || timer == -1) {
            if (word(o, 76) != 0u && node != 0u) {
                copy_point(a, *reinterpret_cast<Point3*>(from_word(word(from_word(node), 12)) + 16));
            } else {
                const Point3 mid = {(p.x + q.x) * 0.5f, (p.y + q.y) * 0.5f, (p.z + q.z) * 0.5f};
                copy_point(a, mid);
            }
            copy_point(b, a);
        } else {
            Point3 d = {a.x - p.x, a.y - p.y, a.z - p.z};
            float scale = reciprocal_sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
            d.x *= scale; d.y *= scale; d.z *= scale;
            d.x *= 900.0f; d.y *= 900.0f; d.z *= 900.0f;
            a.x = a.x - d.x * 0.016666666f;
            a.y = a.y - d.y * 0.016666666f;
            a.z = a.z - d.z * 0.016666666f;
            const Point3 e = {b.x - q.x, b.y - q.y, b.z - q.z};
            copy_point(d, e);
            scale = reciprocal_sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
            d.x *= scale; d.y *= scale; d.z *= scale;
            d.x *= 900.0f; d.y *= 900.0f; d.z *= 900.0f;
            b.x = b.x - d.x * 0.016666666f;
            b.y = b.y - d.y * 0.016666666f;
            b.z = b.z - d.z * 0.016666666f;
        }
        if (static_cast<signed char>(o[132]) > 0) o[132] = static_cast<unsigned char>(o[132] - 1);
        if (frameNm < timer || timer == -1) {
            o[132] = 50;
            if (sword(o, 144) < 70) sword(o, 144) = sword(o, 144) + 2;
        } else if (sword(o, 144) <= 0) {
            sword(o, 144) = 0;
        } else {
            sword(o, 144) = sword(o, 144) - 2;
        }
    }
    if (static_cast<signed char>(o[132]) <= 0) call_entry(o, 8, 3);
}
