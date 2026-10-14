// Observed posInAura__18SpellAncestralCallP6Point3PfT2 at 0x002a81e0, 336 bytes. S is the spell object (first argument),
// P a point (second), and U and V two optional output singles (third and fourth, null allowed). The object that is the
// word at S+8 is called T and the signed byte at S+0 is N. Every table call below is the entry at offset 88 of the table
// whose address is the word at T+96, called on T (adjusted by the entry's signed halfword) with (index, N).
//  1. T's call with index 3 returns an int M. The centre is the point at (the word at S+12) + 16. With d the distance from
//     the centre to P (the sum of squares is formed as x*x + y*y, then + z*z, then rooted), if d is greater than the single
//     (float)(M * 12) the result is 0 and nothing is written.
//  2. Otherwise, when U is not null, *U = (float)(call with index 0) * 0.01 (pool constant with bits 0x3c23d70a).
//  3. When V is not null, *V = (float)(call with index 1) * 0.01 (same constant).
//  4. The result is 1.
// Inferred: a point-in-radius test for an ancestral-call aura whose radius is 12 units per level, with two per-level values
// returned through the outputs. Unknown: what table entry 88 returns for the indices 0, 1 and 3. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

namespace {
int slot88(unsigned char* t, int index, int n) {
    using namespace w12c;
    unsigned char* const entry = from_word(word(t, 96)) + 88;
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
        t + *reinterpret_cast<short*>(entry), index, n);
}
}


extern "C" int posInAura__18SpellAncestralCallP6Point3PfT2(void* self, const Point3* point, float* u, float* v) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    const int m = slot88(from_word(word(s, 8)), 3, static_cast<signed char>(s[0]));
    const Point3* const centre = reinterpret_cast<const Point3*>(from_word(word(s, 12)) + 16);
    const float dx = point->x - centre->x;
    const float dy = point->y - centre->y;
    const float dz = point->z - centre->z;
    const float d = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (!(d <= static_cast<float>(m * 12))) return 0;
    if (u != nullptr) *u = static_cast<float>(slot88(from_word(word(s, 8)), 0, static_cast<signed char>(s[0]))) * bits(0x3c23d70au);
    if (v != nullptr) *v = static_cast<float>(slot88(from_word(word(s, 8)), 1, static_cast<signed char>(s[0]))) * bits(0x3c23d70au);
    return 1;
}
