// Observed flee2__8CreatureP10GameObject at 0x00111088, 1032 bytes.
// Argument: the creature and the object to flee from. A null object does nothing. g is the Point3
// stored at creature+260 (the goal) and (-1, -1) in x and y is its "unset" value; the route
// helpers use x == -1.0 as an end marker.
//   1. The goal is replaced by getSmartRandomDestination__8Creaturei(creature, 300) when g.x and g.y
//      both equal -1.0, or when the horizontal distance from the creature position (+16) to g is
//      below the creature's single at +28. Otherwise, when the squared 3-D distance from the
//      creature to the Point3 at +776 exceeds the creature's single at +788 squared minus 400.0,
//      the goal becomes that Point3.
//   2. When the signed byte at +569 matches frameNm in the low three bits, the Point3 at
//      +760 becomes wallAndObjectSteering(creature, r + 10.0, r + r, null), r being the single at +28.
//   3. When the singles at +760 and +764 are both nonzero, the aim point is that Point3 plus the
//      creature position. Otherwise the aim point is a route point: p = getCurrentRoutePoint, then up
//      to 16 times, while p.x is -1.0 or the horizontal distance from the creature to p is below
//      r, p is advanced with getNextRoutePoint__8CreatureP6Point3G6Point3ffi(creature, &p, the
//      object's position (+16), 0.0, creature +788, 1) and the loop stops at once if p.x became
//      -1.0. When p.x is -1.0 afterwards the goal replaces p.
//   4. The virtual function at byte 4 of the table entry at (table at +72) + 176 is called on the
//      creature adjusted by the entry's signed halfword with the arguments 0 and the aim point
//      address.
//   5. The speed is the creature's single at +176; when the unsigned byte at +800 lies in 1..60 it
//      is multiplied by (60 - that byte as signed) times 1/60 (0x3c888888). The creature moves with
//      move2__8CreatureG6Point3 by (icos(h) * speed, isin(h) * speed, 0), h being the halfword at +56.
// Inferred: the function steers a creature away from the object along a path.
// Unknown: the meaning of the singles at +776/+788 and of the bytes +569 and +800.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern int frameNm;
float icos__Fi(int angle);
float isin__Fi(int angle);
void move2__8CreatureG6Point3(void* self, w11_creature_a::Point3 movement);
void getSmartRandomDestination__8Creaturei(w11_creature_a::Point3* result, void* self, int range);
void getCurrentRoutePoint__8Creature(w11_creature_a::Point3* result, void* self);
void getNextRoutePoint__8CreatureP6Point3G6Point3ffi(void* self, w11_creature_a::Point3* route, w11_creature_a::Point3 target, float a, float b, int c);
w11_creature_a::Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(w11_creature_a::Point3* out, void* creature, float reach, float second, void* ignore);
}

extern "C" void flee2__8CreatureP10GameObject(void* self, void* threat) {
    using namespace w11_creature_a;
    if (threat == nullptr) return;
    Point3& goal = at<Point3>(self, 260);
    const Point3 position = at<Point3>(self, 16);
    bool pick = goal.x == -1.0f && goal.y == -1.0f;
    if (!pick) {
        const float gx = goal.x - position.x;
        const float gy = goal.y - position.y;
        pick = __builtin_sqrtf(gx * gx + gy * gy) < at<float>(self, 28);
    }
    if (pick) {
        Point3 chosen;
        getSmartRandomDestination__8Creaturei(&chosen, self, 300);
        goal = chosen;
    } else {
        const Point3 mark = at<Point3>(self, 776);
        const float mx = position.x - mark.x;
        const float my = position.y - mark.y;
        const float mz = position.z - mark.z;
        const float limit = at<float>(self, 788) * at<float>(self, 788) - 400.0f;
        if (limit < mx * mx + my * my + mz * mz) goal = at<Point3>(self, 776);
    }
    Point3 aim;
    if (((frameNm ^ at<signed char>(self, 569)) & 7) == 0) {
        Point3 steering;
        wallAndObjectSteering__FP8CreatureffP10GameObject(&steering, self, at<float>(self, 28) + 10.0f, at<float>(self, 28) + at<float>(self, 28), nullptr);
        at<Point3>(self, 760) = steering;
    }
    if (at<float>(self, 760) != 0.0f && at<float>(self, 764) != 0.0f) {
        aim = at<Point3>(self, 760);
        aim.x = aim.x + at<float>(self, 16);
        aim.y = aim.y + at<float>(self, 20);
        aim.z = aim.z + at<float>(self, 24);
    } else {
        getCurrentRoutePoint__8Creature(&aim, self);
        for (int count = 0; count < 16; ++count) {
            if (aim.x != -1.0f) {
                const float rx = aim.x - at<float>(self, 16);
                const float ry = aim.y - at<float>(self, 20);
                if (!(__builtin_sqrtf(rx * rx + ry * ry) < at<float>(self, 28))) break;
            }
            Point3 target = at<Point3>(threat, 16);
            getNextRoutePoint__8CreatureP6Point3G6Point3ffi(self, &aim, target, 0.0f, at<float>(self, 788), 1);
            if (aim.x == -1.0f) break;
        }
        if (aim.x == -1.0f) aim = at<Point3>(self, 260);
    }
    Slot turn = slot(self, 176);
    reinterpret_cast<int (*)(void*, int, Point3*)>(turn.function)(turn.self, 0, &aim);
    float speed = at<float>(self, 176);
    const unsigned char counter = at<unsigned char>(self, 800);
    if (static_cast<unsigned char>(counter - 1) < 60) {
        speed = speed * (static_cast<float>(60 - at<signed char>(self, 800)) * __builtin_bit_cast(float, 0x3c888888));
    }
    Point3 movement;
    movement.x = icos__Fi(at<short>(self, 56)) * speed;
    movement.y = isin__Fi(at<short>(self, 56)) * speed;
    movement.z = 0.0f;
    move2__8CreatureG6Point3(self, movement);
}
