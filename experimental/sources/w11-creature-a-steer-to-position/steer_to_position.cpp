// Observed steerToPosition__8CreatureG6Point3 at 0x001108e0, 740 bytes.
// Argument: the creature and a Point3 target passed by address. With r the creature's single at
// +28 and d = target minus the creature position (+16), the 3-D distance is
// dist = sqrt(dx*dx + dy*dy + dz*dz).
//   * dist < r gives result 1.
//   * When the signed byte at +569 matches frameNm in the low three bits and dist < 2r, the heading
//     iatan2__Fff(d.y, d.x) minus the creature's halfword at +56 must have an absolute value (as a
//     32-bit int, no truncation) of at most 16384, else the result is 1.
//   * When the signed byte at +570 is zero and the byte at +569 matches frameNm in the low five bits,
//     wallAndObjectSteering(creature, min(dist, r + 10.0), r + r, null) fills a Point3 that is copied
//     to the creature's words at +760..+768.
//   * When the singles at +760 and +764 are both nonzero, the aim point is that Point3 plus the
//     creature position; otherwise it is the target.
// The virtual function at byte 4 of the table entry at (table at +72) + 176, called on the creature
// adjusted by the entry's signed halfword with the arguments 0 and the aim point address, returns a
// value whose low halfword (signed) has its magnitude compared with 16000. The speed s is the
// creature's single at +176. When that magnitude is not below 16001, s and the singles at +180 and
// +184 are halved (the two fields are written back, s is local). When the unsigned byte at +800
// lies in 1..60, s is multiplied by (60 - that byte as signed) times 1/60 (0x3c888888). The
// creature then moves with move2__8CreatureG6Point3 by (icos(h) * s, isin(h) * s, 0) with h the
// halfword at +56; the result is 0.
// Inferred: the function steers a creature toward a point, with the return value 1 meaning
// arrived or blocked by a turn, and +760 caching an avoidance vector refreshed on a frame cadence.
// Unknown: the virtual at table byte 176 (a turn error: its magnitude gates the slow-down), and the
// bytes at +569, +570 and +800.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern int frameNm;
int iatan2__Fff(float x, float y);
float icos__Fi(int angle);
float isin__Fi(int angle);
void move2__8CreatureG6Point3(void* self, w11_creature_a::Point3 movement);
w11_creature_a::Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(w11_creature_a::Point3* out, void* creature, float reach, float second, void* ignore);
}

extern "C" int steerToPosition__8CreatureG6Point3(void* self, w11_creature_a::Point3 target) {
    using namespace w11_creature_a;
    Point3 aim = target;
    const Point3 position = at<Point3>(self, 16);
    Point3 d;
    d.x = target.x - position.x;
    d.y = target.y - position.y;
    d.z = target.z - position.z;
    const float radius = at<float>(self, 28);
    const float distance = __builtin_sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
    if (distance < radius) return 1;
    if (((frameNm ^ at<signed char>(self, 569)) & 7) == 0 && distance < radius + radius) {
        int difference = iatan2__Fff(d.y, d.x) - at<short>(self, 56);
        if (difference < 0) difference = -difference;
        if (!(difference < 16385)) return 1;
    }
    if (at<signed char>(self, 570) == 0 && ((frameNm ^ at<signed char>(self, 569)) & 0x1f) == 0) {
        const float limit = radius + 10.0f;
        Point3 steering;
        wallAndObjectSteering__FP8CreatureffP10GameObject(&steering, self, distance < limit ? distance : limit, radius + radius, nullptr);
        at<Point3>(self, 760) = steering;
    }
    if (at<float>(self, 760) != 0.0f && at<float>(self, 764) != 0.0f) {
        aim = at<Point3>(self, 760);
        aim.x = aim.x + at<float>(self, 16);
        aim.y = aim.y + at<float>(self, 20);
        aim.z = aim.z + at<float>(self, 24);
    }
    Slot turn = slot(self, 176);
    int error = reinterpret_cast<int (*)(void*, int, Point3*)>(turn.function)(turn.self, 0, &aim);
    error = static_cast<short>(error);
    if (error < 0) error = -error;
    float speed = at<float>(self, 176);
    if (!(error < 16001)) {
        speed = speed * 0.5f;
        at<float>(self, 180) = at<float>(self, 180) * 0.5f;
        at<float>(self, 184) = at<float>(self, 184) * 0.5f;
    }
    const unsigned char counter = at<unsigned char>(self, 800);
    if (static_cast<unsigned char>(counter - 1) < 60) {
        speed = speed * (static_cast<float>(60 - at<signed char>(self, 800)) * __builtin_bit_cast(float, 0x3c888888));
    }
    Point3 movement;
    movement.x = icos__Fi(at<short>(self, 56)) * speed;
    movement.y = isin__Fi(at<short>(self, 56)) * speed;
    movement.z = 0.0f;
    move2__8CreatureG6Point3(self, movement);
    return 0;
}
