/* Observed drawStart__Fv at 0x0015faf0, 136 bytes. writeBuffer at
 * 0x00483ef4 is toggled with xor 1. nmDrawUsed at 0x004868e4 is cleared,
 * then eight record addresses are written. Index 0 uses drawHead
 * (0x00614200). Index 1 uses drawHead+32. Record i is
 * drawRecords + ((index * 2048 + i) << 4), and the word at record offset
 * 12 is cleared. nmDrawUsed becomes 8, then DIntr, reloadDMA__Fi(-1), and
 * EIntr. Fresh compilation is not an original-byte match.
 */
extern "C" unsigned char drawRecords[];
extern "C" unsigned char drawHead[];
extern "C" int writeBuffer;
extern "C" int nmDrawUsed;
extern "C" int DIntr();
extern "C" int EIntr();
extern "C" void reloadDMA__Fi(int cause);

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void drawStart__Fv() {
    const int index = writeBuffer ^ 1;
    nmDrawUsed = 0;
    writeBuffer = index;
    for (int slot = 0; slot < 8; ++slot) {
        unsigned char* record = drawRecords + ((static_cast<unsigned>(index) * 2048u + static_cast<unsigned>(slot)) << 4);
        store_word(drawHead + index * 32 + slot * 4,
                   static_cast<unsigned>(reinterpret_cast<unsigned long>(record)));
        store_word(record + 12, 0u);
    }
    nmDrawUsed = 8;
    DIntr();
    reloadDMA__Fi(-1);
    EIntr();
}
