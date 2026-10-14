// Observed: SetLayerModulate__10VIMaterialii at 0x010cf960 is 80 bytes. The
// layer record starts at 48 + 144 * layer. The function sets bit 0 of the word
// at record offset 4 when the third argument is non-zero and clears it
// otherwise, then stores 1 in the word at offset 32 of the object and returns
// 0. The layer index is not range checked.
// Inferred: the 144-byte stride is the per-layer record; offset 32 is a
// changed flag. Unknown: the layer count and the flag's name.
extern "C" {
int SetLayerModulate__10VIMaterialii(void* object, int layer, int enable) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned* word = reinterpret_cast<unsigned*>(bytes + 48 + layer * 144 + 4);
    if (enable)
        *word |= 0x1;
    else
        *word &= ~0x1u;
    *reinterpret_cast<int*>(bytes + 32) = 1;
    return 0;
}
}
