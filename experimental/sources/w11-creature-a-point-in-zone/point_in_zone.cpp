// Observed pointInZone__8CreatureG6Point3ffss at 0x0010a510, 456 bytes.
// Arguments: the creature, a Point3 passed by address, singles near and far, halfwords centre and
// width. With d = point minus the creature position (+16):
//   1. the heading to the point, 16384 - iatan2__Fff(d.x, d.y) truncated to 16 bits, minus centre
//      (truncated to 16 bits again), must have an absolute value (taken as a 32-bit int) below
//      width, else the result is 0;
//   2. the horizontal distance sqrt(dx*dx + dy*dy) between the creature and the point must satisfy
//      near <= distance < far, else 0;
//   3. a hitscan worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, d, 1.1, 3) is made from
//      the creature position with its z raised by the single at creature+32. A result below 0.0 or
//      above 1.0 gives 1; a result from 0.0 to 1.0 inclusive gives 0.
// The 1.1 literal is the image's value 0x3f8ccccc (stored at gp-32336 as a single).
// Inferred: the hitscan result is a hit fraction along d, so 1 means line of sight is clear.
// Unknown: the hitscan's final int and the meaning of the creature field at +32.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
int iatan2__Fff(float x, float y);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length, int mask);
}

extern "C" int pointInZone__8CreatureG6Point3ffss(void* self, w11_creature_a::Point3 point, float near_limit, float far_limit, short centre, short width) {
    using namespace w11_creature_a;
    const Point3 position = at<Point3>(self, 16);
    Point3 direction;
    direction.x = point.x - position.x;
    direction.y = point.y - position.y;
    direction.z = point.z - position.z;
    const short heading = static_cast<short>(16384 - iatan2__Fff(direction.x, direction.y));
    int difference = static_cast<short>(heading - centre);
    if (difference < 0) difference = -difference;
    if (!(difference < width)) return 0;
    const float dx = position.x - point.x;
    const float dy = position.y - point.y;
    const float distance = __builtin_sqrtf(dx * dx + dy * dy);
    if (!(distance < far_limit)) return 0;
    if (!(near_limit <= distance)) return 0;
    Point3 from = at<Point3>(self, 16);
    from.z = from.z + at<float>(self, 32);
    const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, __builtin_bit_cast(float, 0x3f8ccccc), 3);
    if (hit < 0.0f) return 1;
    if (1.0f < hit) return 1;
    return 0;
}
