// Observed: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode at 0x010cfa10 is
// 36 bytes. It stores 1 in the word at offset 32 of the object, stores the third
// argument in the word at offset 60 + 144 * layer, and returns 0. The layer
// index is not range checked.
// Inferred: the 144-byte stride is a per-layer record; offset 32 is a
// changed flag. Unknown: the layer count and the flag's reader.
extern "C" {
int SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* object, int layer, int mode) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 32) = 1;
    *reinterpret_cast<int*>(bytes + 60 + layer * 144) = mode;
    return 0;
}
}
