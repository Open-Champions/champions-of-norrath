// Observed msg_collision__13AutoContainerP10GameObject at 0x00221940, 220 bytes.
// Does nothing when any of these holds, checked in this order: the signed byte
// netNetworkMode is nonzero; the other object is null; bit 6 (0x40) of the other
// object's word at +12 is clear; bit 3 (0x08) of this object's word at +292 is set;
// the other object's byte at +1137 is nonzero.
// Otherwise the float pair (ox, oy) at other+1196 and other+1200 is scaled to unit
// length with the reciprocal square root of ox*ox + oy*oy (rsqrt.s on the EE, numerator
// 1.0). When the dot product of the unit pair with this object's float pair
// (px, py) at +296 and +300 is below 52.0, that pair gains the other pair times the
// single 0.52 (word at 0x481710): px += ox * 0.52, py += oy * 0.52, where oy is read
// again from +1200. In every case objectAddToRunList then runs on this object. A
// unit-vector stack local that is written and never read in the image is not
// reproduced.
// Unknown: what the float pairs mean and why the threshold is 52.0.
extern "C" {
extern signed char netNetworkMode;
extern const unsigned norrath_rtti_entities_a_view_481710;
void objectAddToRunList__FP10GameObject(void* object);
}

namespace {
typedef float FloatWord __attribute__((may_alias));
typedef unsigned UnsignedWord __attribute__((may_alias));

inline float reciprocal_sqrt(float squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(squared);
#endif
}
float push_factor() {
    union { unsigned bits; float value; } u = {norrath_rtti_entities_a_view_481710};
    return u.value;
}
}

extern "C" void msg_collision__13AutoContainerP10GameObject(void* self, void* other_storage) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* other = static_cast<unsigned char*>(other_storage);
    if (netNetworkMode != 0) return;
    if (other == nullptr) return;
    if ((*reinterpret_cast<const UnsignedWord*>(other + 12) & 0x40u) == 0u) return;
    if ((*reinterpret_cast<const UnsignedWord*>(object + 292) & 0x8u) != 0u) return;
    if (other[1137] != 0) return;
    const float ox = *reinterpret_cast<const FloatWord*>(other + 1196);
    const float oy = *reinterpret_cast<const FloatWord*>(other + 1200);
    const float scale = reciprocal_sqrt(ox * ox + oy * oy);
    const float unit_x = ox * scale;
    const float unit_y = oy * scale;
    FloatWord* px = reinterpret_cast<FloatWord*>(object + 296);
    FloatWord* py = reinterpret_cast<FloatWord*>(object + 300);
    const float x = *px;
    const float y = *py;
    if (x * unit_x + y * unit_y < 52.0f) {
        const float factor = push_factor();
        *px = x + ox * factor;
        *py = y + *reinterpret_cast<const FloatWord*>(other + 1200) * factor;
    }
    objectAddToRunList__FP10GameObject(self);
}
