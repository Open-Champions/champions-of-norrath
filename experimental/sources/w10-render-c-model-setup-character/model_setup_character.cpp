// Observed modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc at 0x00168c08,
// 728 bytes. Arguments: a packet record, a 4x4 matrix, a position, a model header and a
// signed fade byte. No result.
// The packet record is three words: [0] the write cursor, [1] the address of the DMA tag
// at the start of this packet (set here), [2] the address of the current GIF tag (set
// here). The packet is built at the cursor:
//   +0   64-bit DMA tag, written last: quadword count in the low bits, id 1 (CNT);
//   +8   VIF word 0x11000000;
//   then zero bytes up to an address that is 12 modulo 16, and
//   VIF word 0x50000000 | (5 + 1 if bit 0x10 of the model half at +16 is set);
//   GIF tag 0x1000000000008000 with its loop count patched, register word 14 (A+D);
//   A+D entries (data, register): (0, 63), (0, 6), then register 66 with data
//   ((128 - fade) << 32) | 100 when fade is nonzero, otherwise the 64-bit word at model
//   +24; with model bit 0x10 also register 67 with data (byte at model +32 << 32) | 100;
//   then register 71 with data 0x507f5. The loop count is the number of entries.
//   After the GIF block: VIF word 0x6c04c000, the sixteen matrix floats, then VIF word
//   0x04000000 | modelClippingOn (the signed byte at 0x0048407c) and VIF
//   word 0x14000002. lightSetupForCharacter(packet, position) is called, the cursor is
//   padded with zero bytes up to a multiple of 16, and the DMA tag at the start is
//   written with quadword count ((cursor - start) / 8 - 2) / 2.
// Register numbers are as stored; their GS register names are not asserted here.
// Unknown: what the model fields at +16, +24 and +32 hold beyond the uses above.
struct Packet {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* gifTag;
};
struct Point3 {
    float x, y, z;
};
struct Matrix44 {
    float m[16];
};
struct _modelHeader;

extern "C" volatile signed char modelClippingOn;
extern "C" void lightSetupForCharacter__FP6PacketG6Point3(Packet* packet, Point3 position);

namespace {
inline void store64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}
inline void store32(unsigned char* where, unsigned value) {
    *reinterpret_cast<unsigned*>(where) = value;
}
}  // namespace

extern "C" void modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(Packet* packet, Matrix44* matrix,
                                                                                Point3* position,
                                                                                _modelHeader* header, char fade) {
    const unsigned char* model = reinterpret_cast<const unsigned char*>(header);
    unsigned char* start = packet->cursor;
    store32(start + 8, 0x11000000u);
    packet->start = start;
    unsigned char* at = start + 12;
    packet->cursor = at;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(at) & 0xfu) != 12u) *at++ = 0;
    const unsigned short flags = *reinterpret_cast<const unsigned short*>(model + 16);
    const bool extra = (flags & 0x10) != 0;
    unsigned char* vif = at;
    store32(vif, (5u + (extra ? 1u : 0u)) | 0x50000000u);
    unsigned char* tag = vif + 4;
    store64(tag, 0x1000000000008000ull);
    store64(tag + 8, 14ull);
    packet->gifTag = tag;
    store64(tag + 16, 0ull);
    store64(tag + 24, 63ull);
    packet->cursor = tag + 48;
    store64(tag + 32, 0ull);
    store64(tag + 40, 6ull);
    unsigned char* next;
    if (fade != 0) {
        store64(tag + 48, (static_cast<unsigned long long>(static_cast<unsigned>(128 - fade)) << 32) | 100ull);
        store64(tag + 56, 66ull);
        next = tag + 64;
    } else {
        store64(tag + 48, *reinterpret_cast<const unsigned long long*>(model + 24));
        store64(tag + 56, 66ull);
        next = tag + 64;
    }
    packet->cursor = next;
    if (extra) {
        unsigned char* p = packet->cursor;
        store64(p, (static_cast<unsigned long long>(model[32]) << 32) | 100ull);
        store64(p + 8, 67ull);
        packet->cursor = p + 16;
    }
    unsigned char* p = packet->cursor;
    store64(p, 0x507f5ull);
    store64(p + 8, 71ull);
    unsigned char* afterTag = p + 16;
    unsigned long long gif = *reinterpret_cast<unsigned long long*>(packet->gifTag);
    long long count = ((static_cast<int>(afterTag - packet->gifTag) >> 3) - 2) >> 1;
    *reinterpret_cast<unsigned long long*>(packet->gifTag) = static_cast<unsigned long long>(count) | gif;
    store32(afterTag, 0x6c04c000u);
    float* out = reinterpret_cast<float*>(p + 20);
    for (int i = 0; i < 16; i++) out[i] = matrix->m[i];
    unsigned char* clip = p + 84;
    packet->cursor = clip;
    unsigned char* mscal = p + 92;
    packet->cursor = mscal;
    store32(clip, static_cast<unsigned>(static_cast<int>(modelClippingOn)) | 0x04000000u);
    store32(p + 88, 0x14000002u);
    Point3 copy = *position;
    lightSetupForCharacter__FP6PacketG6Point3(packet, copy);
    unsigned char* end = packet->cursor;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(end) & 0xfu) != 0) *end++ = 0;
    unsigned char* dma = packet->start;
    packet->cursor = end;
    store64(dma, static_cast<unsigned long long>(
                     static_cast<long long>(((static_cast<int>(end - dma) >> 3) - 2) >> 1)) |
                     0x10000000ull);
}
