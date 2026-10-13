#include "../w11-actor-c-common/c.hpp"
// Observed start__3WebG6Point3P8Creaturefi at 0x0020dc68, 380 bytes. Arguments: the web w, a Point3 (passed
// by address) P, a creature c, a float f and an int n. In the order of the image:
//   P is copied to a local first; when the word at w+28 is nonzero, listDestroy__FP8ListHead(w+28) runs and
//   the word is cleared; listOnewayLink__FP8ListHeadT0Pv(w+28, c, c) links the creature to the web's list head.
//   Q = the Point3 at c+16 with Q.z increased by (float at c+32) * 0.25; D = Q - P is normalized with one
//   reciprocal square root (rsqrt.s, numerator 1.0, of the sum x*x + y*y + z*z, added in that order) and
//   then multiplied by f; start__3WebG6Point3T1i(w, P, D, n) is called. Nothing is returned.
// Inferred: a web is shot from P toward the point a quarter of the creature's height value above its
// position, at speed f, and the creature is remembered as the web's target. The float at c+32 is only
// known as that scale input. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void start__3WebG6Point3T1i(void* web, Point3 from, Point3 velocity, int n);
}

static inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}

extern "C" void start__3WebG6Point3P8Creaturefi(void* self, Point3 from, void* creature, float speed, int n) {
    using namespace w11c;
    Point3 p = from;
    if (word(self, 28) != 0u) {
        listDestroy__FP8ListHead(bytes(self) + 28);
        word(self, 28) = 0u;
    }
    listOnewayLink__FP8ListHeadT0Pv(bytes(self) + 28, creature, creature);
    const float lift = real(creature, 32) * 0.25f;
    Point3 q = {real(creature, 16), real(creature, 20), real(creature, 24)};
    q.z = q.z + lift;
    Point3 d = {q.x - p.x, q.y - p.y, q.z - p.z};
    const float scale = reciprocal_sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
    d.x = d.x * scale;
    d.y = d.y * scale;
    d.z = d.z * scale;
    d.x = d.x * speed;
    d.y = d.y * speed;
    d.z = d.z * speed;
    start__3WebG6Point3T1i(self, p, d, n);
}
