// Observed: GetLayerColor__C10VIMateriali at 0x010cf938, 40 bytes. The first
// argument is the address of a 4-byte result slot (the hidden pointer for a
// small class returned by value), the second is the material, the third a
// layer number. The four bytes at material + 56 + 144 *
// layer are copied byte-exactly, with no alignment assumption, to the slot, and
// the slot address is returned.
// Inferred: the layers are the 144-byte records from material + 48, and the
// four bytes at record +8 are a VIColor32; the name and the layer count bound
// are not known from this function.
extern "C" {
void* GetLayerColor__C10VIMateriali(void* out, const void* material, int layer) {
    const unsigned char* from = static_cast<const unsigned char*>(material) + 56 + layer * 144;
    unsigned char* to = static_cast<unsigned char*>(out);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
    return out;
}
}
