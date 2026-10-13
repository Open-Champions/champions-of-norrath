// Observed plotRoute__8CreatureG6Point3ffi at 0x00113348, 288 bytes.
// Arguments: the creature, a target Point3 passed by address, singles a and b, and an int mode.
// The creature position (+16) and the target are copied to locals, then exactly one of three
// world routines is called with the global world header, the position copy, the target copy, a
// single, the creature, the buffer at creature+580 and the count 16:
//   mode == 1:        worldPlotRouteCantShoot, with b;
//   else if b != 0:   worldPlotRouteCanShoot, with b;
//   else:             worldPlotRouteCloseTo, with a.
// Afterwards the word at +708 is set to 16. The routines' results are not used.
// Inferred: +580 is a route buffer (the type named by the callees' mangled parameter) and 16 is
// its capacity or step count. Unknown: the meaning of mode other than 1 and of a and b.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
void worldPlotRouteCantShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 to, float range, void* object, void* buffer, int count);
void worldPlotRouteCanShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 to, float range, void* object, void* buffer, int count);
void worldPlotRouteCloseTo__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 to, float range, void* object, void* buffer, int count);
}

extern "C" void plotRoute__8CreatureG6Point3ffi(void* self, w11_creature_a::Point3 target, float a, float b, int mode) {
    using namespace w11_creature_a;
    void* const buffer = static_cast<unsigned char*>(self) + 580;
    if (mode == 1) {
        worldPlotRouteCantShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(world, at<Point3>(self, 16), target, b, self, buffer, 16);
    } else if (b != 0.0f) {
        worldPlotRouteCanShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(world, at<Point3>(self, 16), target, b, self, buffer, 16);
    } else {
        worldPlotRouteCloseTo__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi(world, at<Point3>(self, 16), target, a, self, buffer, 16);
    }
    at<int>(self, 708) = 16;
}
