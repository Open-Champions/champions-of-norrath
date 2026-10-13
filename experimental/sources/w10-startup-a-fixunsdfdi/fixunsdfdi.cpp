// Observed __fixunsdfdi at 0x002f9bd8, 272 bytes. Double to unsigned 64-bit integer, built from
// the soft-double entry points (doubles travel as 64-bit values): dpcmp (0x002fba48), dpmul
// (0x002fb680), dptoul (0x002fbc78), __floatdidf (0x002f9a48), dpadd (0x002fb5c0) and dpsub
// (0x002fb618).
//   1. dpcmp(a, 0.0) < 0 returns 0.
//   2. hi = dptoul(dpmul(a, 2^-32 = 0x3df0000000000000)), taken as a 32-bit value and shifted
//      left 32 into a 64-bit integer (the high half of the result).
//   3. That integer is turned back into a double: when its top bit is clear, __floatdidf of it;
//      when set, __floatdidf of ((hi >> 1) | (hi & 1)) added to itself with dpadd.
//   4. rest = dpsub(a, that double). Then dpcmp(rest, 0.0): a negative rest gives
//      hi minus the zero-extended dptoul(dpsub(0.0, rest)); otherwise hi plus the
//      zero-extended dptoul(rest). That sum is returned.
// Unknown: the bodies of the six callees (only their call shapes are used here); dptoul is
// taken to return a 32-bit unsigned value, as the original zero-extends it after the second call.
// Nothing was executed on the EE.
extern "C" {
int dpcmp(unsigned long long left, unsigned long long right);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
unsigned dptoul(unsigned long long value);
unsigned long long __floatdidf(long long value);
unsigned long long dpadd(unsigned long long left, unsigned long long right);
unsigned long long dpsub(unsigned long long left, unsigned long long right);

unsigned long long __fixunsdfdi(unsigned long long a) {
    if (dpcmp(a, 0) < 0) return 0;
    unsigned long long result = static_cast<unsigned long long>(dptoul(dpmul(a, 0x3df0000000000000ull))) << 32;
    unsigned long long back;
    if (static_cast<long long>(result) >= 0) {
        back = __floatdidf(static_cast<long long>(result));
    } else {
        unsigned long long half = __floatdidf(static_cast<long long>((result >> 1) | (result & 1)));
        back = dpadd(half, half);
    }
    unsigned long long rest = dpsub(a, back);
    if (dpcmp(rest, 0) < 0) {
        unsigned low = dptoul(dpsub(0, rest));
        return result - static_cast<unsigned long long>(low);
    }
    unsigned low = dptoul(rest);
    return result + static_cast<unsigned long long>(low);
}
}
