// Observed: SetLayerColor__10VIMaterialiRC9VIColor32 at 0x010cf908 is 48 bytes. With the
// material in a0, a layer number in a1 and a pointer to a 4-byte colour in a2, it copies
// the four colour bytes (byte-wise, unaligned) to material + layer * 144 + 56, stores the
// word 1 at material + 32, and returns 0. Nothing else is read or written.
// Inferred: layers are 144-byte records starting at the material base, with a 32-bit
// colour at +56 inside the record; the word at +32 is a changed/dirty indication that the
// other layer setters in this class may also set. The layer number is not range checked.
// Unknown: the record's other fields, and whether +32 is really a dirty flag.
struct VIColor32Bytes {
    unsigned char b[4];
};

extern "C" {
int SetLayerColor__10VIMaterialiRC9VIColor32(unsigned char* material, int layer, const VIColor32Bytes* color) {
    *reinterpret_cast<VIColor32Bytes*>(material + layer * 144 + 56) = *color;
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
