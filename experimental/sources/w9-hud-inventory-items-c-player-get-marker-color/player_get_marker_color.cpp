// Observed playerGetMarkerColor__Fii at 0x001ac1d0, 136 bytes.
// Arguments: a color index and an alpha value. Returns (alpha << 24) | rgb where
// rgb is chosen by the index: 0 -> 0x3a2fb6, 1 -> 0x1ed918, 2 -> 0x21dbd8,
// 3 -> 0xd13d47, anything else -> 0x808080. The index is compared as a full
// int; the alpha is shifted as an int, so bits above 7 fall off the top.
// Inferred: the byte order is a packed color word; which channel is which is not
// established here.
extern "C" unsigned playerGetMarkerColor__Fii(int index, int alpha) {
    unsigned color;
    switch (index) {
        case 0: color = 0x3a2fb6; break;
        case 1: color = 0x1ed918; break;
        case 2: color = 0x21dbd8; break;
        case 3: color = 0xd13d47; break;
        default: color = 0x808080; break;
    }
    return (static_cast<unsigned>(alpha) << 24) | color;
}
