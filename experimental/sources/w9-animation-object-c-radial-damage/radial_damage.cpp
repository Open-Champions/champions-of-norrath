#include "../w9-animation-object-c-common/mem.hpp"
// Observed objectRadialDamage__FP10GameObjectG6Point3iffiii at 0x0013f708, 548 bytes.
// Arguments: source object, centre point (12 bytes, copied), integer damage, float inner,
// float outer, an integer passed through to the victims, an integer minimum damage and an
// integer flag mask.
//  1. Builds a box from the centre minus and plus `outer` on every axis and fills a local
//     128-entry list with objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128).
//     The list is walked until its first zero entry.
//  2. scale = 100.0, replaced by (float)damage / (outer - inner) when 1.0 < outer - inner.
//  3. For each listed object that is not the source object and whose word at +12 has no bit
//     of the flag mask: the planar distance is |dx| + |dy| - min(|dx|,|dy|) * 0.333
//     (x and y at object+16 and +20 against the centre; z is not used), minus the float at
//     object+28. Objects with outer <= distance are skipped. Otherwise
//     amount = (outer - distance) * scale, raised to (float)minimum when below it, then
//     capped at (float)damage, truncated to an integer and converted back to float.
//  4. Calls virtual slot 16 of the object's vtable word at +72 with the object adjusted by
//     the slot's signed halfword, the source object, the pass-through integer and the amount.
// Inferred: this is area damage with linear falloff between inner (full scale) and outer;
// the distance is an octagonal approximation using the constant 0x3eaa7ef9 (about 0.333).
// Unknown: the roles of the pass-through integer and the flag mask beyond their use here, the
// meaning of object+28 (used as a radius), and what slot 16 does for each class.
extern "C" int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned* out, int limit);

extern "C" void objectRadialDamage__FP10GameObjectG6Point3iffiii(unsigned char* source, const unsigned char* centre_in,
                                                                int damage, float inner, float outer, int pass,
                                                                int minimum, int mask) {
    using namespace norrath_aoc;
    float centre[3];
    centre[0] = load_float(centre_in);
    centre[1] = load_float(centre_in + 4);
    centre[2] = load_float(centre_in + 8);
    unsigned list[128];
    float low[3], high[3];
    const float damage_f = static_cast<float>(damage);
    low[0] = centre[0] - outer; low[1] = centre[1] - outer; low[2] = centre[2] - outer;
    high[0] = centre[0] + outer; high[1] = centre[1] + outer; high[2] = centre[2] + outer;
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
    float scale = 100.0f;
    const float span = outer - inner;
    if (1.0f < span) scale = damage_f / span;
    for (unsigned i = 0; load_word(reinterpret_cast<unsigned char*>(&list[i])) != 0;) {
        unsigned char* object = load_pointer(reinterpret_cast<unsigned char*>(&list[i]));
        ++i;
        if (object == source) continue;
        if ((load_word(object + 12) & static_cast<unsigned>(mask)) != 0) continue;
        float dy = load_float(object + 20) - centre[1];
        float dx = load_float(object + 16) - centre[0];
        dy = __builtin_fabsf(dy);
        dx = __builtin_fabsf(dx);
        const float smaller = dx < dy ? dx : dy;
        float distance = (dx + dy) - smaller * 0x1.54fdf2p-2f;
        distance = distance - load_float(object + 28);
        if (outer <= distance) continue;
        float amount = (outer - distance) * scale;
        const float floor_f = static_cast<float>(minimum);
        if (amount < floor_f) amount = floor_f;
        amount = amount < damage_f ? amount : damage_f;
        call_virtual_pointer_pointer_int_float(object, 16, source, pass, static_cast<float>(static_cast<int>(amount)));
    }
}
