// Observed GetPlayerPosition__7VehicleiP6Point3Ps at 0x00271168, 336 bytes.
// Arguments: vehicle, a player value, an output point and a halfword output.
// The word at vehicle+976 is a count n and the words from vehicle+960 are a
// table. The seat index i is the first index below n whose table word equals
// the player value, or n when none matches, or 0 when n is not above zero.
// getSeatPosition__7VehicleiP6Point3 is called with the vehicle, i and the
// output point. The pointer at vehicle+2088+4*i then selects the path. Null:
// the halfword is iatan2__Fff(-float(vehicle+1132), -float(vehicle+1128)).
// Otherwise the 36 bytes at vehicle+1116 are copied to a local 3x3 matrix, which
// matRot__FP8Matrix33is rotates on axis 2 by the short of cvt.w.s of
// float(pointer+280) * A * B, where A has bits 0x3bb60b60 (about 1/180) and B
// is 32767.0, multiplied in that order; the halfword is then
// iatan2__Fff(-m[4], -m[3]) of the local matrix as floats. Nothing is
// returned. What the table and the float at +280 are is not established here.
// Nothing here was executed on the EE. The cvt.w.s rounding is whatever the
// processor mode is; the host check assumes round to nearest.
extern "C" {
void getSeatPosition__7VehicleiP6Point3(void* vehicle, int seat, void* point);
void matRot__FP8Matrix33is(float* matrix, int axis, short angle);
short iatan2__Fff(float a, float b);
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;

namespace {
inline int cvt_w_s(float value) {
#if defined(__mips__)
    int out;
    float scratch;
    __asm__("cvt.w.s %1, %2\n\tmfc1 %0, %1" : "=r"(out), "=&f"(scratch) : "f"(value));
    return out;
#else
    return static_cast<int>(__builtin_rintf(value));
#endif
}
inline float float_from_bits(unsigned bits) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = bits;
    return converted.number;
}
}

extern "C" void GetPlayerPosition__7VehicleiP6Point3Ps(unsigned char* vehicle, int player, void* point, short* yaw) {
    const int count = *reinterpret_cast<AliasedInt*>(vehicle + 976);
    int seat = 0;
    if (count > 0 && *reinterpret_cast<AliasedInt*>(vehicle + 960) != player) {
        seat = 1;
        while (seat < count && *reinterpret_cast<AliasedInt*>(vehicle + 960 + 4 * seat) != player) ++seat;
    }
    getSeatPosition__7VehicleiP6Point3(vehicle, seat, point);
    unsigned char* occupant = *reinterpret_cast<unsigned char**>(vehicle + 2088 + 4 * seat);
    if (occupant == nullptr) {
        *yaw = iatan2__Fff(-*reinterpret_cast<AliasedFloat*>(vehicle + 1132),
                           -*reinterpret_cast<AliasedFloat*>(vehicle + 1128));
        return;
    }
    float matrix[9];
    for (int i = 0; i < 9; ++i) matrix[i] = *reinterpret_cast<AliasedFloat*>(vehicle + 1116 + 4 * i);
    const float angle = *reinterpret_cast<AliasedFloat*>(occupant + 280) * float_from_bits(0x3bb60b60u) * 32767.0f;
    matRot__FP8Matrix33is(matrix, 2, static_cast<short>(cvt_w_s(angle)));
    *yaw = iatan2__Fff(-matrix[4], -matrix[3]);
}
