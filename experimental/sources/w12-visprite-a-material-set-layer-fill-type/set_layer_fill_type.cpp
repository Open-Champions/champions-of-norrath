// Observed: SetLayerFillType__10VIMateriali18VIMaterialFillType at 0x010cf7f0
// is 92 bytes. The layer record starts at 48 + 144 * layer. When the fill type
// argument equals 1 the function sets bit 5 (mask 0x20) of the word at record
// offset 4; for every other value it clears that bit. It then stores 1 in the
// word at offset 32 of the object and returns 0. The layer index is not range
// checked.
// Inferred: the 144-byte stride is the per-layer record; offset 32 is a
// changed flag. Unknown: the other fill type values and the flag's name.
extern "C" {
int SetLayerFillType__10VIMateriali18VIMaterialFillType(void* object, int layer, int fillType) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned* word = reinterpret_cast<unsigned*>(bytes + 48 + layer * 144 + 4);
    if (fillType == 1)
        *word |= 0x20;
    else
        *word &= ~0x20u;
    *reinterpret_cast<int*>(bytes + 32) = 1;
    return 0;
}
}
