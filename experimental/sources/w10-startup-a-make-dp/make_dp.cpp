// Observed __make_dp at 0x002fbbf0, 44 bytes. Takes the four fields of an unpacked double
// (class, sign, power as ints and the 64-bit fraction) and stores them into a 32-byte stack
// record at +0, +4, +8 and +16 (+12 and +24 are left unwritten), then returns the result of
// __pack_d (0x002ff538) on that record unchanged. The 64-bit result is passed through.
// Unknown: nothing beyond what the record names in __unpack_d / __pack_d already state.
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
unsigned long long __pack_d(const W10UnpackedDouble* value);

unsigned long long __make_dp(int cls, int sign, int power, unsigned long long fraction) {
    W10UnpackedDouble record;
    record.cls = cls;
    record.sign = sign;
    record.power = power;
    record.fraction = fraction;
    return __pack_d(&record);
}
}
