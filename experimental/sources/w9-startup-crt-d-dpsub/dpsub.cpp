// Observed dpsub at 0x002fb618, 100 bytes. Soft-double subtraction. Both 64-bit arguments are
// stored in locals and each is unpacked with __unpack_d (0x002ff610) into its own
// 32-byte record. The sign word of the second record is flipped (exclusive-or 1) after it is unpacked, then the pair is added with _fpadd_parts (0x002fb360).
// A third 32-byte local is the scratch record passed as the third argument of
// _fpadd_parts, and the pointer that _fpadd_parts returns is passed to __pack_d (0x002ff538), whose 64-bit result
// is returned unchanged.
// Unknown: the bodies of the three callees and the record field names.
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
W9UnpackedDouble* _fpadd_parts(W9UnpackedDouble* left, W9UnpackedDouble* right, W9UnpackedDouble* scratch);
unsigned long long __pack_d(const W9UnpackedDouble* unpacked);

unsigned long long dpsub(unsigned long long left, unsigned long long right) {
    W9UnpackedDouble a;
    W9UnpackedDouble b;
    W9UnpackedDouble sum;
    __unpack_d(&left, &a);
    __unpack_d(&right, &b);
    b.sign ^= 1;
    return __pack_d(_fpadd_parts(&a, &b, &sum));
}
}
