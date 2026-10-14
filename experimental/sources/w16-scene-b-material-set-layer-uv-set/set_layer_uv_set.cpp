// Observed: SetLayerUVSet__10VIMaterialii at 0x010cfb78 is 64 bytes. With the layer
// record word at material + 52 + 144 * layer, it first stores that word with bit 4
// (mask 0x10) cleared; when the third argument is non-zero it then stores the word
// with the compiler-visible expression (cleared | ~0x10), which is 0xffffffef.
// It then stores 1 at offset 32 and returns 0.
// Inferred: the non-zero path looks like a mask slip in the original (all bits other
// than bit 4 become set); it is reproduced as observed, not corrected.
// Unknown: the meaning of the record word and its reader.
extern "C" {
int SetLayerUVSet__10VIMaterialii(unsigned char* material, int layer, int set) {
    unsigned* word = reinterpret_cast<unsigned*>(material + 52 + layer * 144);
    unsigned cleared = *word & ~0x10u;
    *word = cleared;
    if (set)
        *word = cleared | ~0x10u;
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
