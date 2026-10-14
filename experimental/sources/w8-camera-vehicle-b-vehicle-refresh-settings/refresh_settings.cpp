// Observed refreshSettings__7Vehicle at 0x2782b8, extent 236, in order:
//  1. Virtual call: the vtable word at object+72, entry at vtable+224 (a
//     halfword adjustment, then the function word). The function runs with
//     object + adjustment. That is slot 28 of _vt$7Vehicle, which the vehicle
//     view documents as the per-model set() routine.
//  2. With m = float at +884 and s = floats at +888, +892, +896 each times
//     0x3cd20d20 (the .sdata word at 0x482288, about 1/39), written a, b, c:
//       +668 = m,  +672 = 1.0 / m
//       +1216 = (m * (a*a + b*b)) * K     K = 0x3daaaaaa (word at 0x48228c, about 1/12)
//       +1220 = (m * (a*a + c*c)) * K
//       +1212 = (m * (c*c + b*b)) * K
//       +1228 = 1.0 / (+1216), +1232 = 1.0 / (+1220), +1224 = 1.0 / (+1212)
//     The sums and products are in that order; zero divisors are not tested.
//  3. buildCollisionInfo__7Vehicle runs on the object. The +1224 store is
//     issued before that call.
// Inferred, not observed: +1212..+1220 look like a box inertia tensor diagonal.
// Unknown: which of x, y, z each of +888, +892, +896 is.
// When NORRATH_W8_HOST is defined a host hook resolves the 32-bit vtable word,
// since host pointers are wider than the EE words stored in the object.
extern "C" void buildCollisionInfo__7Vehicle(void* object);
#if defined(NORRATH_W8_HOST)
extern "C" const unsigned char* norrath_w8_host_resolve(unsigned bits);
#endif

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef short __attribute__((may_alias)) AliasedHalf;
union FloatBits { unsigned bits; float value; };
const FloatBits scale = {0x3cd20d20u};
const FloatBits third = {0x3daaaaaau};
inline float& at(unsigned char* o, unsigned off) { return *reinterpret_cast<AliasedFloat*>(o + off); }
}

extern "C" void refreshSettings__7Vehicle(void* object) {
    unsigned char* o = static_cast<unsigned char*>(object);
    const unsigned vtable_bits = *reinterpret_cast<AliasedWord*>(o + 72);
#if defined(NORRATH_W8_HOST)
    const unsigned char* entry = norrath_w8_host_resolve(vtable_bits) + 224;
#else
    const unsigned char* entry = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(vtable_bits)) + 224;
#endif
    const int adjust = *reinterpret_cast<const AliasedHalf*>(entry);
    typedef void (*Slot)(void*);
#if defined(NORRATH_W8_HOST)
    const Slot slot = reinterpret_cast<Slot>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_w8_host_resolve(*reinterpret_cast<const AliasedWord*>(entry + 4))));
#else
    const Slot slot = reinterpret_cast<Slot>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const AliasedWord*>(entry + 4)));
#endif
    slot(o + adjust);

    float a = at(o, 888) * scale.value;
    float b = at(o, 892) * scale.value;
    float c = at(o, 896) * scale.value;
    const float mass = at(o, 884);
    at(o, 668) = mass;
    a = a * a;
    b = b * b;
    c = c * c;
    const float inverse_mass = 1.0f / mass;
    const float ab = a + b;
    const float ac = a + c;
    const float cb = c + b;
    const float ixy = (mass * ab) * third.value;
    const float ixz = (mass * ac) * third.value;
    const float izy = (mass * cb) * third.value;
    at(o, 672) = inverse_mass;
    at(o, 1216) = ixy;
    at(o, 1220) = ixz;
    at(o, 1212) = izy;
    at(o, 1228) = 1.0f / ixy;
    at(o, 1232) = 1.0f / ixz;
    at(o, 1224) = 1.0f / izy;
    buildCollisionInfo__7Vehicle(object);
}
