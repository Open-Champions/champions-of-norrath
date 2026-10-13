#include "../w10-actor-q-common/q.hpp"
// Observed bNextToEdge__14WoodElfSoldier at 0x002494f0, 656 bytes.
// Returns 0 at once when the signed byte at object+570 is nonzero. Otherwise it copies the Point3 at
// object+16, then probes four spots around it by calling worldFindStandHeight__FP12_worldHeaderG6Point3f
// with the global `world`, the probe point and 10.0f. A probe is a 30.0f-long step, given by
// (icos(angle) * 30, isin(angle) * 30, 0), added to x and y of the point; z is never changed. The angles
// are 0, 16384, 32767 and -16384, tried in that order. The first, third and fourth steps are added to
// the running point (so the third and fourth start from the second probe), while the second step is
// added to the original copy. The first probe whose height is below the object's z (object+24) minus
// 100.0f ends the search with 1; if none is, the result is 0.
// Inferred: this is a ledge test for the soldier. Unknown: whether the accumulating steps are intended.
// Nothing here ran on the EE.
extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius);
extern void* world;
}

namespace {
const float reach = 30.0f;
bool low_at(const void* self, const float* point) {
    return worldFindStandHeight__FP12_worldHeaderG6Point3f(world, point, 10.0f) <
           w10q::real(const_cast<void*>(self), 24) - 100.0f;
}
}  // namespace

extern "C" int bNextToEdge__14WoodElfSoldier(void* self) {
    using namespace w10q;
    if (sbyte(self, 570) != 0) return 0;
    const float origin[3] = {real(self, 16), real(self, 20), real(self, 24)};
    float point[3] = {origin[0], origin[1], origin[2]};
    float step[3];
    step[2] = 0.0f;
    step[0] = icos__Fi(0) * reach;
    step[1] = isin__Fi(0) * reach;
    point[0] += step[0];
    point[1] += step[1];
    if (low_at(self, point)) return 1;
    step[0] = icos__Fi(16384) * reach;
    step[1] = isin__Fi(16384) * reach;
    point[0] = origin[0] + step[0];
    point[1] = origin[1] + step[1];
    point[2] = origin[2];
    if (low_at(self, point)) return 1;
    step[0] = icos__Fi(32767) * reach;
    step[1] = isin__Fi(32767) * reach;
    point[0] += step[0];
    point[1] += step[1];
    if (low_at(self, point)) return 1;
    step[0] = icos__Fi(-16384) * reach;
    step[1] = isin__Fi(-16384) * reach;
    point[0] += step[0];
    point[1] += step[1];
    return low_at(self, point) ? 1 : 0;
}
