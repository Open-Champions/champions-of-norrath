// Observed __fpcmp_parts_d at 0x002ff6d0, 288 bytes. Orders two unpacked doubles (the 32-byte
// records of __unpack_d: class +0, sign +4, power +8, 64-bit fraction +16) and returns -1, 0
// or 1. Checks in this order:
//   either class below 2 (the NaN classes)  : 1.
//   first is class 4, second is not         : -1 if the first's sign is set, else 1.
//   both class 4                            : second sign - first sign.
//   second is class 4 (first is not)        : 1 if the second's sign is set, else -1.
//   both class 2                            : 0.
//   first class 2 (second not)              : 1 if the second's sign is set, else -1.
//   second class 2 (first not)              : -1 if the first's sign is set, else 1.
//   otherwise (finite, nonzero): different signs give -1 for a set first sign and 1 for a clear
//   one; equal signs compare power then fraction (unsigned), the larger magnitude giving 1 for
//   a clear sign and -1 for a set sign, the smaller magnitude the reverse; all equal gives 0.
// Inferred only: class 4 is infinity and class 2 is zero, from the paired constructor routines.
// Nothing was executed on the EE.
extern "C" {
struct W10UnpackedDouble {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

int __fpcmp_parts_d(const W10UnpackedDouble* left, const W10UnpackedDouble* right) {
    unsigned lc = static_cast<unsigned>(left->cls);
    unsigned rc = static_cast<unsigned>(right->cls);
    if (lc < 2 || rc < 2) return 1;
    if (lc == 4) {
        if (rc == 4) return right->sign - left->sign;
        return left->sign ? -1 : 1;
    }
    if (rc == 4) return right->sign ? 1 : -1;
    if (lc == 2) {
        if (rc == 2) return 0;
        return right->sign ? 1 : -1;
    }
    if (rc == 2) return left->sign ? -1 : 1;
    int sign = left->sign;
    if (sign == right->sign) {
        if (right->power < left->power) return sign ? -1 : 1;
        if (left->power < right->power) return sign ? 1 : -1;
        if (right->fraction < left->fraction) return sign ? -1 : 1;
        if (!(left->fraction < right->fraction)) return 0;
        return sign ? 1 : -1;
    }
    return sign ? -1 : 1;
}
}
