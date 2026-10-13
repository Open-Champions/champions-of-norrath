// Observed __make_fp at 0x002ff508, 44 bytes. Takes the four fields of an unpacked single
// (class, sign, power and the 32-bit fraction, all as 32-bit words) and stores them at +0, +4,
// +8 and +12 of a 16-byte stack record, then returns the result of __pack_f (0x002ff808) on
// that record unchanged (a float; the original does not touch the result register after the
// call). Unknown: the body of __pack_f. Nothing was executed on the EE.
extern "C" {
struct W10UnpackedSingle {
    int cls;
    int sign;
    int power;
    unsigned fraction;
};
float __pack_f(const W10UnpackedSingle* value);

float __make_fp(int cls, int sign, int power, unsigned fraction) {
    W10UnpackedSingle record;
    record.cls = cls;
    record.sign = sign;
    record.power = power;
    record.fraction = fraction;
    return __pack_f(&record);
}
}
