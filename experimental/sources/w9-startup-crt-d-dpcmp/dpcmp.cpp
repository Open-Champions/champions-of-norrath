// Observed dpcmp at 0x002fba48, 76 bytes. Soft-double comparison. Both 64-bit
// arguments are stored in locals and each is unpacked with __unpack_d
// (0x002ff610) into its own 32-byte record. __fpcmp_parts_d (0x002ff6d0) is then
// called with the first record and the second, and its integer result is
// returned unchanged.
// Unknown: the bodies of both callees and the meaning of the result values.
extern "C" {
struct W9UnpackedDouble {
    int cls;
    int sign;
    int exponent;
    int reserved;
    unsigned long long fraction;
    unsigned long long reserved2;
};
void __unpack_d(const unsigned long long* value, W9UnpackedDouble* unpacked);
int __fpcmp_parts_d(const W9UnpackedDouble* left, const W9UnpackedDouble* right);

int dpcmp(unsigned long long left, unsigned long long right) {
    W9UnpackedDouble a;
    W9UnpackedDouble b;
    __unpack_d(&left, &a);
    __unpack_d(&right, &b);
    return __fpcmp_parts_d(&a, &b);
}
}
