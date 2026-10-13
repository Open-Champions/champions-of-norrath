// Observed enemyInZone2__8Creatureffssfi at 0x0010a878, 584 bytes.
// Arguments: the creature, singles near (f12) and far (f13), halfwords centre and width, a single
// lead (f14) and an int flag. When the list head word at +576 is null the result is 0. Otherwise
// the target is the object in the head node's word at +12 and p is its position (+16). When lead
// is not 0.0 and the target's word at +12 has bit 0x20 set, p.x and p.y are advanced by the
// target's singles at +816 and +820 times lead. With d = p minus the creature position (+16):
//   1. the heading iatan2__Fff(d.y, d.x) minus centre, truncated to 16 bits, must have an
//      absolute value (as a 32-bit int) below width, else 0;
//   2. the horizontal distance between the creature and p must satisfy
//      near - r <= distance < far + r, where r is the target's single at +28, else 0;
//   3. a hitscan worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, d, 1.1, flag ? 0 : 3)
//      is made from the creature position with z raised by half the creature's single at +32.
//      A result below 0.0 or above 1.0 gives 1; a result from 0.0 to 1.0 inclusive gives 0.
// The 1.1 literal is the image's value 0x3f8ccccc (three separate copies exist near gp-32336).
// Inferred: r is the target's radius and bit 0x20 marks a moving target whose velocity is at
// +816; the hitscan result is a hit fraction, so 1 means the line is clear.
// Unknown: the hitscan's final int, and the field at creature+32 (half of it is the eye raise).
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
int iatan2__Fff(float x, float y);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length, int mask);
}

extern "C" int enemyInZone2__8Creatureffssfi(void* self, float near_limit, float far_limit, short centre, short width, float lead, int flag) {
    using namespace w11_creature_a;
    void* head = at<void*>(self, 576);
    if (head == nullptr) return 0;
    void* target = at<void*>(head, 12);
    Point3 p = at<Point3>(target, 16);
    if (lead != 0.0f && (at<unsigned>(target, 12) & 0x20u) != 0) {
        p.x = p.x + at<float>(target, 816) * lead;
        p.y = p.y + at<float>(target, 820) * lead;
    }
    const Point3 position = at<Point3>(self, 16);
    Point3 direction;
    direction.x = p.x - position.x;
    direction.y = p.y - position.y;
    direction.z = p.z - position.z;
    int difference = static_cast<short>(iatan2__Fff(direction.y, direction.x) - centre);
    if (difference < 0) difference = -difference;
    if (!(difference < width)) return 0;
    const float dx = position.x - p.x;
    const float dy = position.y - p.y;
    const float radius = at<float>(at<void*>(at<void*>(self, 576), 12), 28);
    const float distance = __builtin_sqrtf(dx * dx + dy * dy);
    if (!(distance < far_limit + radius)) return 0;
    if (!(near_limit - radius <= distance)) return 0;
    Point3 from = at<Point3>(self, 16);
    from.z = from.z + at<float>(self, 32) * 0.5f;
    float hit;
    if (flag != 0) hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, __builtin_bit_cast(float, 0x3f8ccccc), 0);
    else hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, __builtin_bit_cast(float, 0x3f8ccccc), 3);
    if (hit < 0.0f) return 1;
    if (1.0f < hit) return 1;
    return 0;
}
