// Observed msg_alert__3OrcifG6Point3P10GameObject at 0x0022b570, 328 bytes. Arguments: the
// object, an integer kind, a float (unused), a three-float position taken by value, and another
// object (may be null). "State" is the halfword at +572. "Entry 184" is the table entry at
// (word at +72) + 184, called on the object adjusted by the entry's signed halfword with a
// second argument; the table address is the word at +72 of the object concerned.
//  kind 0: with a non-null other: its table's function word at +4 is called with this object
//    as the only argument and the returned type record is compared (__eq__ on the type_info
//    pair) with the record from __tf3Orc; when equal and the state, read unsigned, is below 2,
//    entry 184 is called with 2 and the position is copied to the floats at +272..+280.
//  kind 2: when the word at +1112 is nonzero, other is non-null and the state is 3 or 5, an
//    angle is formed by iatan2__Fff from two differences: (float +20 of other minus float +20
//    of this) as the first argument and (float +16 of other minus float +16 of this) as the
//    second. When angle minus the halfword at +56 plus 9999 is below 19999 (unsigned), entry
//    184 is called with 16.
// Any other kind does nothing. Nothing here ran on the EE; the meaning of the fields is unknown.
#include "../w10-actor-k-common/access.hpp"

extern "C" {
const void* __tf3Orc();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
int iatan2__Fff(float y, float x);

void msg_alert__3OrcifG6Point3P10GameObject(void* self, int kind, float, Point3 position, void* other) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const target = static_cast<unsigned char*>(other);
    if (kind == 0) {
        if (target) {
            unsigned char* e = w10k::from_word(w10k::word(target, 72));
            const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W10_ACTOR_K_HOST)
            const void* type = reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(w10_actor_k_host_call(fn, self, 0, 0, 0)));
#else
            const void* type = reinterpret_cast<const void* (*)(void*)>(static_cast<__UINTPTR_TYPE__>(fn))(self);
#endif
            if (__eq__C9type_infoRC9type_info(type, __tf3Orc()) && static_cast<unsigned short>(w10k::half(o, 572)) < 2) {
                w10k::call_entry(o, 184, 2);
                *reinterpret_cast<Point3*>(o + 272) = position;
            }
        }
    } else if (kind == 2) {
        if (w10k::word(o, 1112) != 0 && target) {
            const int state = w10k::half(o, 572);
            if (state == 3 || state == 5) {
                const int angle = iatan2__Fff(w10k::real(target, 20) - w10k::real(o, 20), w10k::real(target, 16) - w10k::real(o, 16));
                if (static_cast<unsigned>(angle - w10k::half(o, 56) + 9999) < 19999u) w10k::call_entry(o, 184, 16);
            }
        }
    }
}
}
