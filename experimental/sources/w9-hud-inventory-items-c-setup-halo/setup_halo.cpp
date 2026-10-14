// Observed setupHalo__4itemPQ24item11descriptionf at 0x00108308, 220 bytes.
// Arguments: an item record and a float scale. Reads the 64-bit word at
// record+8 and tests bit 33 (0x200000000): when set the level is 7, otherwise 0.
//  - level 0: calls modelSetHalo(&halo, 0) with the record untouched and returns
//    through that call.
//  - level > 0: s = level * 0.025 + 0.6 (the two single-precision words
//    0x3ccccccc and 0x3f19999a), limited to at most 1.0. The halo record then
//    gets byte+3 = (int)(s * 64.0), byte+11 = (int)(s * 72.0), float+4 =
//    scale * 200.0 and float+12 = scale * 100.0, and modelSetHalo(&halo, 2) runs.
// The halo record is a 16-byte file-local object (halos.1138, 0x0031f530) that
// persists between calls; its starting bytes are 0a 0a 44 40, 200.0f, 3c 3c 44 48,
// 100.0f. The bytes at +0..+2 and +8..+10 are never written here.
// Unknown: what the bit at 33 means and the HaloDef field names.
extern "C" void modelSetHalo__FP7HaloDefi(void* halo, int mode);

namespace {
struct Halo {
    unsigned char b0, b1, b2, b3;
    float first;
    unsigned char b8, b9, b10, b11;
    float second;
};
Halo halo = {0x0a, 0x0a, 0x44, 0x40, 200.0f, 0x3c, 0x3c, 0x44, 0x48, 100.0f};
}

extern "C" void setupHalo__4itemPQ24item11descriptionf(const unsigned char* record, float scale) {
    const int level = (*reinterpret_cast<const unsigned long long*>(record + 8) & 0x200000000ull) ? 7 : 0;
    if (level <= 0) {
        modelSetHalo__FP7HaloDefi(&halo, 0);
        return;
    }
    float s = static_cast<float>(level) * __builtin_bit_cast(float, 0x3ccccccc) + __builtin_bit_cast(float, 0x3f19999a);
    if (1.0f < s) s = 1.0f;
    const float high = s * 64.0f;
    const float wide = s * 72.0f;
    halo.first = scale * 200.0f;
    halo.second = scale * 100.0f;
    halo.b11 = static_cast<unsigned char>(static_cast<int>(wide));
    halo.b3 = static_cast<unsigned char>(static_cast<int>(high));
    modelSetHalo__FP7HaloDefi(&halo, 2);
}
