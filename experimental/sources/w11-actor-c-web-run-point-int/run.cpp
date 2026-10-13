#include "../w11-actor-c-common/c.hpp"
// Observed run__3WebG6Point3i at 0x0020e038, 1112 bytes. Arguments: the web w, a Point3 P (passed by address
// and copied first) and an int t (a time). Fields of w: int at +0, float at +4, int at +8, word at +12 (a
// mode flag: zero while the web is in flight), Point3 pos at +16, node pointer word at +28 (the list node
// start__3Web... links; its word at +12 is the held creature c), Point3 vel at +32, int at +44 (end time),
// int at +52, int at +56. Fields of c used: flags word +12, float +28 (a radius), float +32, Point3 +16,
// halfword +748, float +756, word +1400.
//   t < w[44] (active):
//     w[12] == 0 (flying): three steps, each in this order: pos += vel * 0.0166666657; the height
//       h = worldFindHeight__FP12_worldHeaderG6Point3f(world, copy of pos, 5.0); then
//       - pos.z <= h: blockAlloc(160) and, when it returns memory, the WebTrap constructor
//         (__7WebTrapG6Point3T1) on it with P and a copy of pos; breakWeb; the loop ends;
//       - no node at w+28: breakWeb, next step;
//       - c without bit 5 (0x20) in its flags word: the same WebTrap creation, breakWeb, loop ends;
//       - 0.0 < c[756], or (flags & 0x28) == 0x28: breakWeb, next step;
//       - otherwise T = c's Point3 with T.z += c[32] * 0.5; when sqrt((T.x-pos.x)^2 + (T.y-pos.y)^2) < c[28]:
//         w[12] = 1, w[44] = w[52] + t, w[0] = 5, w[4] = 1.0 (the web catches the creature); next step.
//     then, when w[12] != 0 (also entered directly when it already was): the held creature is steered:
//       no node, or c without bit 0x20: breakWeb and stop. Else w[4] grows by 0.5 while below c[28];
//       c[756] = 160.0; c's halfword at +748 = iatan2(c.y - P.y, c.x - P.x) - 32767; pos = c's Point3;
//       pos.z += c[32] * 0.5; and when w[56] < t, c's flags have bit 6 (0x40) and c[1400] == 1: w[0] is
//       decremented, breakWeb when it reaches zero, and w[56] = t + 30.
//   t >= w[44] (expired or never started): when w[44] == t, breakWeb and stop. Otherwise w[44] becomes 0
//       unless it was 1; w[12] = 0; a positive w[8] is reduced by 3 and clamped at zero; a nonzero node word
//       at +28 is released with listDestroy__FP8ListHead(w+28) and cleared.
// Inferred: a spider web projectile that flies, snags a creature within its radius, then holds and turns
// it. All meanings of the numeric fields are guesses from use; none is established. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern unsigned char* world;
void* blockAlloc__Fi(int size);
void __7WebTrapG6Point3T1(void* memory, Point3 a, Point3 b);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
int iatan2__Fff(float y, float x);
void breakWeb__3Web(void* web);
void listDestroy__FP8ListHead(void* head);
}

namespace {
void make_trap(const Point3& from, const float* at) {
    void* memory = blockAlloc__Fi(160);
    if (memory != nullptr) {
        Point3 where = {at[0], at[1], at[2]};
        __7WebTrapG6Point3T1(memory, from, where);
    }
}
}  // namespace

extern "C" void run__3WebG6Point3i(void* self, Point3 point, int now) {
    using namespace w11c;
    Point3 p = point;
    const int until = static_cast<int>(word(self, 44));
    if (now < until) {
        if (word(self, 12) == 0u) {
            for (int step = 0; step < 3; ++step) {
                real(self, 16) = real(self, 16) + real(self, 32) * 0.0166666657f;
                real(self, 20) = real(self, 20) + real(self, 36) * 0.0166666657f;
                real(self, 24) = real(self, 24) + real(self, 40) * 0.0166666657f;
                Point3 probe = {real(self, 16), real(self, 20), real(self, 24)};
                const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 5.0f);
                if (real(self, 24) <= height) {
                    make_trap(p, &real(self, 16));
                    breakWeb__3Web(self);
                    break;
                }
                if (word(self, 28) == 0u) {
                    breakWeb__3Web(self);
                    continue;
                }
                unsigned char* c = link(link(self, 28), 12);
                if ((word(c, 12) & 0x20u) == 0u) {
                    make_trap(p, &real(self, 16));
                    breakWeb__3Web(self);
                    break;
                }
                if (0.0f < real(c, 756) || (word(c, 12) & 0x28u) == 0x28u) {
                    breakWeb__3Web(self);
                    continue;
                }
                Point3 target = {real(c, 16), real(c, 20), real(c, 24)};
                target.z = target.z + real(c, 32) * 0.5f;
                const float dx = target.x - real(self, 16);
                const float dy = target.y - real(self, 20);
                const float distance = __builtin_sqrtf(dx * dx + dy * dy);
                if (distance < real(c, 28)) {
                    word(self, 12) = 1u;
                    word(self, 44) = word(self, 52) + static_cast<unsigned>(now);
                    word(self, 0) = 5u;
                    real(self, 4) = 1.0f;
                }
            }
            if (word(self, 12) == 0u) return;
        }
        if (word(self, 28) == 0u || (word(link(link(self, 28), 12), 12) & 0x20u) == 0u) {
            breakWeb__3Web(self);
            return;
        }
        unsigned char* c = link(link(self, 28), 12);
        if (real(self, 4) < real(c, 28)) real(self, 4) = real(self, 4) + 0.5f;
        real(c, 756) = 160.0f;
        const int angle = iatan2__Fff(real(c, 20) - p.y, real(c, 16) - p.x);
        shalf(c, 748) = static_cast<short>(angle - 32767);
        real(self, 16) = real(c, 16);
        real(self, 20) = real(c, 20);
        real(self, 24) = real(c, 24);
        real(self, 24) = real(self, 24) + real(c, 32) * 0.5f;
        if (static_cast<int>(word(self, 56)) < now && (word(c, 12) & 0x40u) != 0u && word(c, 1400) == 1u) {
            word(self, 0) = word(self, 0) - 1u;
            if (word(self, 0) == 0u) breakWeb__3Web(self);
            word(self, 56) = static_cast<unsigned>(now + 30);
        }
        return;
    }
    if (until == now) {
        breakWeb__3Web(self);
        return;
    }
    if (until != 1) word(self, 44) = 0u;
    int length = static_cast<int>(word(self, 8));
    word(self, 12) = 0u;
    if (length > 0) {
        length = length - 3;
        word(self, 8) = static_cast<unsigned>(length);
    }
    if (length < 0) word(self, 8) = 0u;
    if (word(self, 28) != 0u) {
        listDestroy__FP8ListHead(bytes(self) + 28);
        word(self, 28) = 0u;
    }
}
