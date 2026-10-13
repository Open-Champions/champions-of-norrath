// Observed TurnToFaceEdge__14WoodElfSoldier at 0x00249780, 788 bytes: a WoodElfSoldier method taking only the object.
// Steps, in order. Let P be the position at +16, H(Q) be worldFindStandHeight__FP12_worldHeaderG6Point3f(world, Q, 10.0)
// and "low" mean H(Q) < (float at +24) - 100.0 (the float at +24 is read after the call). For an angle A, D(A) =
// (icos__Fi(A) * 30.0, isin__Fi(A) * 30.0) with icos called before isin.
//  1. Q = P (read before the first icos call); Q.x += D(0).x and Q.y += D(0).y (z stays P.z). If low, Q is the answer.
//  2. Otherwise Q = P re-read from +16 with D(16384) added to x and y. If low, Q is the answer.
//  3. Otherwise D(32767) is added to the current Q (not to P). If low, Q is the answer.
//  4. Otherwise D(-16384) is added to the current Q. If low, Q is the answer; if not, the answer is not written.
//  When a probe was low, its Q is stored at +272 (three floats). In every case the entry at byte 176 of the table at
//  +72 is then called (the slot after Creature's updateEnemy, whose function body is turnToFace__8CreatureiG6Point3
//  in the base class): the halfword at its start adjusts the object and the function word at +4 is called with
//  (adjusted object, 0, the three floats now at +272 by value).
// Inferred: the probes look for a drop of more than 100 units in the ground ahead and the soldier then turns toward that
// point. The additive probes in steps 3 and 4 are the observed behavior and may be unintended. Unknown: the angle unit
// (the values 0, 16384, 32767, -16384 read as quarter turns of a 16-bit angle) and what radius 10.0 means to the
// ground search. Nothing here ran on the EE.
#include "../w10-actor-m-common/access.hpp"

extern "C" {
extern void* world;
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, Point3 point, float radius);
void TurnToFaceEdge__14WoodElfSoldier(void* self);
}

namespace {
inline Point3 step(int angle) {
    Point3 d;
    d.x = icos__Fi(angle) * 30.0f;
    d.y = isin__Fi(angle) * 30.0f;
    d.z = 0.0f;
    return d;
}
inline bool low(unsigned char* o, Point3 q) { return worldFindStandHeight__FP12_worldHeaderG6Point3f(world, q, 10.0f) < w10m::real(o, 24) - 100.0f; }
}

extern "C" void TurnToFaceEdge__14WoodElfSoldier(void* self) {
    using namespace w10m;
    unsigned char* o = static_cast<unsigned char*>(self);
    Point3 q = *reinterpret_cast<Point3*>(o + 16);
    bool found = false;
    Point3 d = step(0);
    q.x = q.x + d.x;
    q.y = q.y + d.y;
    if (low(o, q)) {
        found = true;
    } else {
        d = step(16384);
        const Point3 here = *reinterpret_cast<Point3*>(o + 16);
        q.x = here.x + d.x;
        q.y = here.y + d.y;
        q.z = here.z;
        if (low(o, q)) {
            found = true;
        } else {
            d = step(32767);
            q.x = q.x + d.x;
            q.y = q.y + d.y;
            if (low(o, q)) {
                found = true;
            } else {
                d = step(-16384);
                q.x = q.x + d.x;
                q.y = q.y + d.y;
                found = low(o, q);
            }
        }
    }
    if (found) *reinterpret_cast<Point3*>(o + 272) = q;
    const unsigned char* entry = from_word(word(o, 72)) + 176;
    const Point3 target = *reinterpret_cast<Point3*>(o + 272);
    unsigned char* adjusted = o + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W10_ACTOR_M_HOST)
    w10_actor_m_host_call(function, adjusted, 2, 0, reinterpret_cast<unsigned long>(&target));
#else
    reinterpret_cast<void (*)(void*, int, Point3)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, 0, target);
#endif
}
