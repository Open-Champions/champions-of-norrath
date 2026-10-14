// Observed inventoryInit__Fv at 0x001b0508, 108 bytes.
// It calls memset on the 464-byte table named inventory (0x00427288) with
// zero, then visits the first two 116-byte records and stores in each: the
// words at offsets 0 and 4 as zero, offset 8 as 320, offset 12 as 448, offset 52
// as -1 and offset 76 as 2. The remaining two records are left zero. What the
// fields mean is not established. memset and the table are not defined here.
extern "C" unsigned char inventory[];
extern "C" void* memset(void* dest, int value, unsigned size);

extern "C" void inventoryInit__Fv() {
    memset(inventory, 0, 464);
    unsigned char* record = inventory;
    for (int i = 0; i < 2; ++i) {
        *reinterpret_cast<int*>(record + 76) = 2;
        *reinterpret_cast<int*>(record + 8) = 320;
        *reinterpret_cast<int*>(record + 12) = 448;
        *reinterpret_cast<int*>(record + 0) = 0;
        *reinterpret_cast<int*>(record + 4) = 0;
        *reinterpret_cast<int*>(record + 52) = -1;
        record += 116;
    }
}
