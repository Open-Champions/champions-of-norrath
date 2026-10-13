// Observed findClearPath__8CreatureG6Point3scf at 0x0010bd98, 1064 bytes.
// Arguments: the creature, a Point3 origin passed by address, a signed halfword angle, a signed
// byte direction flag and a single length (f12). The result is the halfword angle chosen (left in
// the return register already sign-extended).
// Setup: r is the creature's single at +28 and L = length * r. The angle step is +2048 when the flag
// byte is nonzero and -2048 when it is zero. The best clearance starts at 0.0 and the best angle at
// the argument. Up to 32 candidate angles a (the argument, then stepping by the step, each
// truncated to 16 bits) are tried:
//   * the side points are origin + r * (icos(a - 16384), isin(a - 16384), 0) and
//     origin + r * (icos(a + 16384), isin(a + 16384), 0);
//   * three probes follow in order: from the origin, from the first side point and from the
//     second side point. Each is worldHitscanWithObjects(world, from, (icos(a) * L, isin(a) * L, 0),
//     L, creature, null, 3, 0) with the z of from raised by 13.0. A probe's clearance is 1.0 when
//     the hit result is below 0.0 or above 1.0, else the hit result itself;
//   * the candidate's clearance is the minimum of the probes, but a later probe is only made when
//     the running minimum is still above the best clearance so far;
//   * when the candidate's clearance exceeds the best, it becomes the best and a becomes the chosen
//     angle.
// The loop ends early when the best clearance is exactly 1.0.
// Inferred: the result is the heading offset that gives a clear lane for a body of radius r.
// Unknown: the last two ints of the hitscan, the meaning of 13.0, and the use of the result by
// callers.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length, void* ignore, void** found, int a, int b);
}

namespace {
inline float clearance(void* self, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length) {
    from.z = from.z + 13.0f;
    const float hit = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(world, from, direction, length, self, nullptr, 3, 0);
    if (hit < 0.0f) return 1.0f;
    if (1.0f < hit) return 1.0f;
    return hit;
}
}  // namespace

extern "C" short findClearPath__8CreatureG6Point3scf(void* self, w11_creature_a::Point3 origin, short angle, signed char flag, float length) {
    using namespace w11_creature_a;
    const int step = flag != 0 ? 2048 : -2048;
    float best = 0.0f;
    short chosen = angle;
    for (int count = 0; count < 32; ++count) {
        Point3 side_a = origin;
        Point3 side_b = origin;
        side_a.x = origin.x + icos__Fi(angle - 16384) * at<float>(self, 28);
        side_a.y = origin.y + isin__Fi(angle - 16384) * at<float>(self, 28);
        side_b.x = origin.x + icos__Fi(angle + 16384) * at<float>(self, 28);
        side_b.y = origin.y + isin__Fi(angle + 16384) * at<float>(self, 28);
        const float reach = length * at<float>(self, 28);
        Point3 direction;
        direction.x = icos__Fi(angle) * reach;
        direction.y = isin__Fi(angle) * reach;
        direction.z = 0.0f;
        float lane = clearance(self, origin, direction, reach);
        if (best < lane) {
            const float reach_a = length * at<float>(self, 28);
            direction.x = icos__Fi(angle) * reach_a;
            direction.y = isin__Fi(angle) * reach_a;
            const float other_a = clearance(self, side_a, direction, reach_a);
            if (other_a < lane) lane = other_a;
            if (best < lane) {
                const float reach_b = length * at<float>(self, 28);
                direction.x = icos__Fi(angle) * reach_b;
                direction.y = isin__Fi(angle) * reach_b;
                const float other_b = clearance(self, side_b, direction, reach_b);
                if (other_b < lane) lane = other_b;
                if (best < lane) {
                    best = lane;
                    chosen = angle;
                }
            }
        }
        if (best == 1.0f) break;
        angle = static_cast<short>(angle + step);
    }
    return chosen;
}
