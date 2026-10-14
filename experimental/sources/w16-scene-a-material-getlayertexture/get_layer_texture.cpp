// Observed: GetLayerTexture__10VIMateriali at 0x010cf8f0 is 24 bytes in the pinned SLUS-20565 image. It returns the word at
// this + a1*144 + 48, with no range check.
// Inferred: layer records are 144 bytes starting at offset 48 (the same stride and base GetLayerColor uses in this tree), so the word is
// the first word of layer a1's record and, from the name, its texture reference.
// Unknown: the number of layers, whether the word is a pointer or an id, and any range check (none is made). Nothing here ran on the EE.
extern "C" {
unsigned GetLayerTexture__10VIMateriali(unsigned char* self, int layer) {
    return *reinterpret_cast<unsigned*>(self + layer * 144 + 48);
}
}
