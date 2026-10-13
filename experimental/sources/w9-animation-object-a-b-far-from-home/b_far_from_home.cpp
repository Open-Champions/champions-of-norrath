#include "../w9-animation-object-a-common/mem.hpp"
// Observed bFarFromHome__8Creature at 0x00112da8, 88 bytes. The creature position is
// the float triple at +16 and the stored point is the float triple at +776; the float
// at +788 is a distance. Returns 1 when the squared distance between the two points,
// summed as (dx*dx + dy*dy) + dz*dz, is greater than the square of that distance,
// otherwise 0.
// Inferred: +776 is a home position and +788 its allowed radius (the function name and
// the comparison say so; no field names are claimed).
// Unknown: where the home point and radius are set.
extern "C" int bFarFromHome__8Creature(unsigned char* self) {
    using namespace norrath_ao;
    const float dx = load_float(self + 16) - load_float(self + 776);
    const float dy = load_float(self + 20) - load_float(self + 780);
    const float dz = load_float(self + 24) - load_float(self + 784);
    const float radius = load_float(self + 788);
    return radius * radius < dx * dx + dy * dy + dz * dz;
}
