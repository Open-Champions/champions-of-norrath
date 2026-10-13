// Observed __tf7FireFly at 0x0030ee98, 136 bytes (weak definition).
// Returns the 12-byte __ti7FireFly object. When its first word is zero:
//  1. When the first word of __ti10FireBeetle is zero, calls the Creature type-info function and then
//     __rtti_si with the FireBeetle object, the bytes "10FireBeetle" and the Creature type-info object.
//  2. __rtti_si with this object, the bytes "7FireFly" and the FireBeetle type-info object.
// A nonzero first word of this object skips everything; a nonzero first word of the FireBeetle object
// skips only step 1. __ti10FireBeetle is defined by the FireBeetle type-info function's unit.

extern "C" {
unsigned char __ti7FireFly[12];
extern unsigned char __ti10FireBeetle[];
extern unsigned char __ti8Creature[];
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, void* base);
}

namespace {
unsigned first_word(const unsigned char* raw) {
    return unsigned(raw[0]) | (unsigned(raw[1]) << 8) | (unsigned(raw[2]) << 16) | (unsigned(raw[3]) << 24);
}
}

extern "C" void* __tf7FireFly() {
    if (first_word(__ti7FireFly) == 0u) {
        if (first_word(__ti10FireBeetle) == 0u) {
            __tf8Creature();
            __rtti_si(__ti10FireBeetle, "10FireBeetle", __ti8Creature);
        }
        __rtti_si(__ti7FireFly, "7FireFly", __ti10FireBeetle);
    }
    return __ti7FireFly;
}
