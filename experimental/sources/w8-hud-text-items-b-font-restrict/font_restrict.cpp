// Observed fontRestrict__Fi at 0x001743a8, 32 bytes.
// The argument -1 stores 100000 (0x186a0). Any other value is stored as given.
// The destination is the gp-relative word named restrictChars in the symbol
// table. Its readers (the text printers) are not part of this unit, and the
// storage is not defined here.
extern "C" int restrictChars;

extern "C" void fontRestrict__Fi(int count) {
    if (count == -1) {
        restrictChars = 100000;
        return;
    }
    restrictChars = count;
}
