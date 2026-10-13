// Observed objectIsCrowded__8CreatureP10GameObjectffi at 0x0010afb8, 524 bytes.
// Arguments: the creature, another object, singles range (f12) and length (f13), and an int flag.
//   * A null object, or a signed byte at creature+569 that does not match frameNm in the low three
//     bits, returns the flag unchanged.
//   * n = numInRegion__8creatureG6Point3f(the object's position (+16), the object's single at +28
//     times 1.75 (0x3fe00000)).
//   * objectInZone__8CreatureP10GameObjectfs(creature, object, range, 16384) returning 0 gives 0.
//   * A probe vector is built: h = (short)(16384 - iatan2__Fff(object.x - creature.x,
//     object.y - creature.y)), vector (icos(h) * length, isin(h) * length, 0).
//   * worldHitscanWithObjects(world, from, vector, 1.0, creature, &found, 3, 0) is made from the
//     creature position with z raised by 13.0; found starts null.
//   * The result is 1 when n is at least 3 (flag nonzero) or 4 (flag zero), the hitscan result is
//     above 0.0 and below 1.0, and found differs from the object; otherwise 0.
// Inferred: the answer says whether the object is surrounded and not directly the one in the way.
// Unknown: the last two ints of the hitscan and what 13.0 and 1.75 stand for.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
extern int frameNm;
int numInRegion__8creatureG6Point3f(w11_creature_a::Point3 position, float radius);
int objectInZone__8CreatureP10GameObjectfs(void* self, void* other, float range, short zone);
int iatan2__Fff(float x, float y);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length, void* ignore, void** found, int a, int b);
}

extern "C" int objectIsCrowded__8CreatureP10GameObjectffi(void* self, void* other, float range, float length, int flag) {
    using namespace w11_creature_a;
    if (other == nullptr) return flag;
    if (((frameNm ^ at<signed char>(self, 569)) & 7) != 0) return flag;
    const int count = numInRegion__8creatureG6Point3f(at<Point3>(other, 16), at<float>(other, 28) * __builtin_bit_cast(float, 0x3fe00000));
    if (objectInZone__8CreatureP10GameObjectfs(self, other, range, 16384) == 0) return 0;
    const int heading = static_cast<short>(16384 - iatan2__Fff(at<float>(other, 16) - at<float>(self, 16), at<float>(other, 20) - at<float>(self, 20)));
    Point3 direction;
    direction.x = icos__Fi(heading) * length;
    direction.y = isin__Fi(heading) * length;
    direction.z = 0.0f;
    Point3 from = at<Point3>(self, 16);
    void* found = nullptr;
    from.z = from.z + 13.0f;
    const float hit = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(world, from, direction, 1.0f, self, &found, 3, 0);
    if (count < (flag != 0 ? 3 : 4)) return 0;
    if (!(0.0f < hit)) return 0;
    if (!(hit < 1.0f)) return 0;
    return found != other;
}
