// Observed drawListClear__Fv at 0x0015fa40, 92 bytes.
// writeBuffer (0x00483ef4) is read once. nmDrawUsed (0x004868e4) is zeroed, then
// eight consecutive pointers are stored in drawHead (0x00614200), starting at word
// writeBuffer * 8 (32 bytes per buffer). Pointer n is drawRecords (0x00604200)
// + (writeBuffer * 2048 + n) * 16, and the word at offset 12 of that 16-byte record
// is zeroed. nmDrawUsed finishes as 8. No return value.
// Inferred: drawHead is a per-buffer table of eight record pointers and a
// record's word at +12 is a link or count field cleared on reset.
// Unknown: the record's other fields, the table's total extent, and the element
// types; the records are treated as raw bytes here.
extern "C" int writeBuffer;
extern "C" int nmDrawUsed;
extern "C" unsigned char* drawHead[];
extern "C" unsigned char drawRecords[];

extern "C" void drawListClear__Fv() {
    int buffer = writeBuffer;
    nmDrawUsed = 0;
    unsigned char** slot = drawHead + buffer * 8;
    for (int n = 0; n < 8; ++n) {
        unsigned char* record = drawRecords + ((buffer * 2048 + n) << 4);
        *slot++ = record;
        *reinterpret_cast<int*>(record + 12) = 0;
    }
    nmDrawUsed = 8;
}
