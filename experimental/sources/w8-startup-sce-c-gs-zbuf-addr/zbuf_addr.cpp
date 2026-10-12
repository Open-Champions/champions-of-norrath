// Observed sceGszbufaddr at 0x002d1dd8, 200 bytes. The three arguments are
// truncated to 16-bit signed values. The width in 64-pixel blocks is
// (width + 63) / 64 with signed division. The height is divided by 64 (rounded
// the same way) when bit 1 of the index is set, otherwise (height + 31) / 32.
// The block product is shifted left 16, or 17 when the first 64-bit word
// returned by sceGsGetGParam, masked with 0x0000ffff0000ffff, is not exactly
// 1, and the result is sign-extended from bit 15 of the shifted value after a
// right shift of 16. What the parameter words mean is unknown.
extern "C" {
const unsigned long long* sceGsGetGParam();
int sceGszbufaddr(int index, int width, int height) {
    const short short_index = static_cast<short>(index);
    const short short_width = static_cast<short>(width);
    const short short_height = static_cast<short>(height);
    const unsigned long long* param = sceGsGetGParam();
    const int width_blocks = (short_width + 63) / 64;
    int height_blocks;
    if ((short_index & 2) != 0) {
        height_blocks = (short_height + 63) / 64;
    } else {
        height_blocks = (short_height + 31) / 32;
    }
    const unsigned product = static_cast<unsigned>(width_blocks * height_blocks);
    if ((*param & 0x0000ffff0000ffffull) == 1ull) {
        return static_cast<int>(product << 16) >> 16;
    }
    return static_cast<int>(product << 17) >> 16;
}
}
