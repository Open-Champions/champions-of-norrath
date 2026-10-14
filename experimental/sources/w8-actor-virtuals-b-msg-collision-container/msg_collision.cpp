// Observed msg_collision__9ContainerP10GameObject at 0x00217960, 212 bytes.
// Returns at once when the byte netNetworkMode is nonzero, when bit 6 (0x40) of the
// other object's flag word at +12 is clear, when bit 3 (value 8) of this object's word
// at +104 is set, or when the byte at other+1137 is nonzero.
// Otherwise it takes the float pair (ox, oy) at other+1196 and other+1200, scales it to
// unit length with the reciprocal square root of ox*ox + oy*oy (rsqrt.s on the EE), and
// takes the dot product of the unit pair with this object's float pair (px, py) at +108
// and +112. When that dot product is below 52.0, px and py each gain the other pair's
// value times 0.52 (the image reads 0.52 from a shared float constant at 0x004815c4).
// In every non-returning case the object is added to the run list.
// A unit-vector local is written on the stack in the image and never read; it is not
// reproduced. A zero pair: the host check uses IEEE division (infinity, NaN dot product,
// no update); the EE rsqrt.s result for zero was not observed. Unknown: what the float
// pairs are.

extern "C" {
extern signed char netNetworkMode;
void objectAddToRunList__FP10GameObject(void* object);
}

namespace {
typedef unsigned UWord __attribute__((may_alias));
typedef float FloatWord __attribute__((may_alias));

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
}

extern "C" void msg_collision__9ContainerP10GameObject(void* self, void* other_storage) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* other = static_cast<const unsigned char*>(other_storage);
    if (netNetworkMode != 0) return;
    if ((*reinterpret_cast<const UWord*>(other + 12) & 0x40) == 0) return;
    if ((*reinterpret_cast<const UWord*>(object + 104) & 8) != 0) return;
    if (other[1137] != 0) return;
    const float ox = *reinterpret_cast<const FloatWord*>(other + 1196);
    const float oy = *reinterpret_cast<const FloatWord*>(other + 1200);
    const float scale = reciprocal_sqrt(ox * ox + oy * oy);
    const float unit_x = ox * scale;
    const float unit_y = oy * scale;
    FloatWord* px = reinterpret_cast<FloatWord*>(object + 108);
    FloatWord* py = reinterpret_cast<FloatWord*>(object + 112);
    const float x = *px;
    const float y = *py;
    if (x * unit_x + y * unit_y < 52.0f) {
        *px = x + ox * 0.52f;
        *py = y + *reinterpret_cast<const FloatWord*>(other + 1200) * 0.52f;
    }
    objectAddToRunList__FP10GameObject(self);
}
