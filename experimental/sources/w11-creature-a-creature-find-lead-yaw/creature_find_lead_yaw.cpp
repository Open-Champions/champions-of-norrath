// Observed creatureFindLeadYaw__8CreatureP10GameObjectf at 0x00113a30, 272 bytes.
// Arguments: the creature, another object (may be null) and a single divisor. A null object gives
// 0. Otherwise the object's point (+16) is copied. When bit 0x40 of the object's word at +12 is
// set, the point is moved along the object's velocity: with t = sqrt(dx*dx + dy*dy) / divisor,
// where dx and dy are the point's x and y minus the creature's x and y, the singles at object+816
// and +820 times t are added to the point's x and y. The angle is then
// 16384 - iatan2__Fff(x - creature x, y - creature y), truncated to 16 bits; eRand__10GameObject
// is called on the creature and (eRand mod 1800, taken unsigned) minus 900 is added; the result is
// truncated to a signed halfword and returned.
// Inferred: the singles at +816 and +820 are the object's x/y velocity (they are only used that way
// here) and the result is a yaw with a random aim error of +-900 units. Unknown: the angle unit and
// what bit 0x40 of the +12 word means.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
int iatan2__Fff(float x, float y);
int eRand__10GameObject(void* object);
}

extern "C" short creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* other, float divisor) {
    using namespace w11_creature_a;
    if (other == nullptr) return 0;
    Point3 point = at<Point3>(other, 16);
    if ((at<unsigned>(other, 12) & 0x40u) != 0u) {
        const float dx = point.x - at<float>(self, 16);
        const float dy = point.y - at<float>(self, 20);
        const float t = __builtin_sqrtf(dx * dx + dy * dy) / divisor;
        point.y = point.y + at<float>(other, 820) * t;
        point.x = point.x + at<float>(other, 816) * t;
    }
    const short yaw = static_cast<short>(16384 - iatan2__Fff(point.x - at<float>(self, 16), point.y - at<float>(self, 20)));
    const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 1800u;
    return static_cast<short>(yaw + static_cast<int>(roll) - 900);
}
