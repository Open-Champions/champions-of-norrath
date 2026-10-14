// Observed __eq__19ParticleEmitterDataRC19ParticleEmitterData at 0x00186a70,
// 232 bytes. Both arguments are addresses of 17-byte records. The fields are
// compared in this order, stopping at the first difference: bytes at 0 to 7, the
// halfwords at 8 and 10, the halfword at 12 (loaded signed; equality does not
// depend on that), then the bytes at 14, 15 and 16. Bytes 0 to 16 are therefore
// all compared. The result is 1 when every field is equal and 0 otherwise.
// Nothing is written and bytes past 16 are not read. Fresh compilation is not an
// original-byte match.
extern "C" int __eq__19ParticleEmitterDataRC19ParticleEmitterData(const void* left, const void* right) {
    const unsigned char* a = static_cast<const unsigned char*>(left);
    const unsigned char* b = static_cast<const unsigned char*>(right);
    for (int index = 0; index < 8; ++index) {
        if (a[index] != b[index]) return 0;
    }
    for (int offset = 8; offset <= 12; offset += 2) {
        const unsigned lo = a[offset] | (static_cast<unsigned>(a[offset + 1]) << 8);
        const unsigned ro = b[offset] | (static_cast<unsigned>(b[offset + 1]) << 8);
        if (lo != ro) return 0;
    }
    if (a[14] != b[14]) return 0;
    if (a[15] != b[15]) return 0;
    return a[16] == b[16] ? 1 : 0;
}
