// Observed scriptAddDeferedCall__FPc at 0x001d3d40, 108 bytes. While
// nextNmDeferedCalls (int at 0x00486b9c) is below 64, the argument string is
// copied into slot nextNmDeferedCalls of one of two banks of deferedCall
// (0x007d0de8) and the count is incremented. The bank is the opposite of the
// low bit of scriptFrame (int at 0x00485088). Each bank is 5120 bytes and each
// slot 80 bytes, so deferedCall spans 10240 bytes up to the game-state table;
// the 80-byte slot size is inferred from that stride, and the copy itself is
// unbounded. A full list drops the call without a write.
extern "C" {
char* strcpy(char* destination, const char* source);
extern int nextNmDeferedCalls;
extern int scriptFrame;
extern unsigned char deferedCall[];

void scriptAddDeferedCall__FPc(char* text) {
    if (nextNmDeferedCalls < 64) {
        const int bank = (scriptFrame & 1) ^ 1;
        const int slot = nextNmDeferedCalls;
        nextNmDeferedCalls = slot + 1;
        strcpy(reinterpret_cast<char*>(deferedCall + bank * 5120 + slot * 80), text);
    }
}
}
