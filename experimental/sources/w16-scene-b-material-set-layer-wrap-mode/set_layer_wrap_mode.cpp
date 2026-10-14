// Observed: SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode at 0x010ce920 is 252
// bytes. It acts on the word at material + 52 + 144 * layer. Mode 0 clears bits 1 and
// 2 (mask 0x6); mode 1 clears them then sets bit 1 (0x2); mode 2 clears them then
// sets bit 2 (0x4); mode 3 sets both (0x6). Any other mode returns -1 and writes
// nothing. On a valid mode it also stores 1 at material + 32 and returns 0.
// Inferred: the two bits hold a two-bit wrap selector; offset 32 is the changed flag
// shared with the other layer setters. Unknown: which wrap each value names.
extern "C" {
int SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(unsigned char* material, int layer, int mode) {
    unsigned* word = reinterpret_cast<unsigned*>(material + 52 + layer * 144);
    switch (mode) {
    case 0:
        *word &= ~0x6u;
        break;
    case 1:
        *word = (*word & ~0x6u) | 0x2u;
        break;
    case 2:
        *word = (*word & ~0x6u) | 0x4u;
        break;
    case 3:
        *word |= 0x6u;
        break;
    default:
        return -1;
    }
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
