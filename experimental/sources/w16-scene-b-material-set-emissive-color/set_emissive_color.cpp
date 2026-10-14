// Observed: SetEmissiveColor__10VIMaterialRC9VIColor32 at 0x010cf7d0 is 32 bytes.
// It copies four bytes (unaligned) from the colour pointer in a1 to offset 16 of
// the material, stores 1 in the word at offset 32, and returns 0.
// Inferred: offset 16 is a 32-bit emissive colour; offset 32 is the same changed
// flag the layer setters write. Unknown: the colour's channel order and the reader.
struct VIColor32Bytes {
    unsigned char b[4];
};

extern "C" {
int SetEmissiveColor__10VIMaterialRC9VIColor32(unsigned char* material, const VIColor32Bytes* color) {
    *reinterpret_cast<VIColor32Bytes*>(material + 16) = *color;
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
