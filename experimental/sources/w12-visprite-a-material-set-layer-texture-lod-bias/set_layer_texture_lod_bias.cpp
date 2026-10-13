// Observed: SetLayerTextureLODBias__10VIMaterialif at 0x010cfb00 is 36 bytes.
// It stores 1 in the word at offset 32 of the object, stores the float
// argument at offset 100 + 144 * layer and returns 0. The layer index is not
// range checked.
// Inferred: the 144-byte stride is the per-layer record also used by
// SetLayerBlendMode (word at 60 + 144 * layer); offset 32 is a changed flag.
// Unknown: the layer count and the reader of the bias.
extern "C" {
int SetLayerTextureLODBias__10VIMaterialif(void* object, int layer, float bias) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 32) = 1;
    *reinterpret_cast<float*>(bytes + 100 + layer * 144) = bias;
    return 0;
}
}
