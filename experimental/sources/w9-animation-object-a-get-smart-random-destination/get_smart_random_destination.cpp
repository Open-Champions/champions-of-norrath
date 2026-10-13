#include "../w9-animation-object-a-common/mem.hpp"
// Observed getSmartRandomDestination__8Creaturei at 0x0010d250, 464 bytes. The result is
// a Point3 returned through the first argument, which the function also returns in the
// result register. It is written as an explicit pointer argument because the reviewed
// compiler returns a 12-byte struct in registers, unlike the original convention. The
// creature is the second argument and the integer is the third.
//   Floats at creature+16 are the position P, at +776 the point H, and at +788 a radius.
//   limit = (float)range, replaced by (float)(int)radius when radius < limit.
//   reach = (int)(limit * 0.707), the float constant with bits 0x3f34fdf3.
//   d2 = (dx*dx + dy*dy) + dz*dz for P - H, summed in that order.
//   Result is H when (float)(reach*reach) <= d2, or when bit 0x400000 of the word at
//   creature+160 is set and 10000.0 < d2.
//   Otherwise up to ten attempts: candidate = (H.x + (float)((eRand & 2*reach) - reach),
//   H.y + (float)((eRand & 2*reach) - reach), H.z), the two eRand calls made in that order
//   on the creature; floor = worldFindStandHeight(world, candidate, float at creature+28).
//   An attempt is accepted when creature[+24] - 12.0 < floor < creature[+24] + 12.0 (both
//   strict). The result is the last candidate tried, accepted or not.
// Inferred: a wander target around H within about 0.707*range, kept within 12 units of
// the creature's own height so it does not walk up or down cliffs; creature+160 bit
// 0x400000 looks like a "stay near home" flag. No names are claimed.
// Unknown: meaning of the constants 0.707 and 10000.0 beyond their values, and the world
// pointer's pointee type.
extern "C" {
extern void* world;
unsigned eRand__10GameObject(void* self);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const void* point, float extent);
}

extern "C" unsigned char* getSmartRandomDestination__8Creaturei(unsigned char* result, unsigned char* self, int range) {
    using namespace norrath_ao;
    float limit = static_cast<float>(range);
    const float radius = load_float(self + 788);
    if (radius < limit) limit = static_cast<float>(static_cast<int>(radius));
    const float dx = load_float(self + 16) - load_float(self + 776);
    const float dy = load_float(self + 20) - load_float(self + 780);
    const float dz = load_float(self + 24) - load_float(self + 784);
    const int reach = static_cast<int>(limit * __builtin_bit_cast(float, 0x3f34fdf3u));
    const float distance2 = dx * dx + dy * dy + dz * dz;
    float candidate[3];
    if (static_cast<float>(reach * reach) <= distance2 ||
        ((load_word(self + 160) & 0x400000u) != 0u && 10000.0f < distance2)) {
        candidate[0] = load_float(self + 776);
        candidate[1] = load_float(self + 780);
        candidate[2] = load_float(self + 784);
    } else {
        int attempts = 0;
        do {
            const unsigned first = eRand__10GameObject(self);
            candidate[0] = load_float(self + 776) + static_cast<float>(static_cast<int>(first & static_cast<unsigned>(reach * 2)) - reach);
            const unsigned second = eRand__10GameObject(self);
            candidate[1] = load_float(self + 780) + static_cast<float>(static_cast<int>(second & static_cast<unsigned>(reach * 2)) - reach);
            candidate[2] = load_float(self + 784);
            const float floor = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, candidate, load_float(self + 28));
            const float height = load_float(self + 24);
            ++attempts;
            if (height - 12.0f < floor && floor < height + 12.0f) break;
        } while (attempts < 10);
    }
    store_float(result + 0, candidate[0]);
    store_float(result + 4, candidate[1]);
    store_float(result + 8, candidate[2]);
    return result;
}
