// Observed: resetQuickLoad__Fv at 0x0025bd20 is 40 bytes. It calls memset on
// the 64 bytes at 0x007f7fc0 with value 0 and returns. That address is the
// symbol quickLoadStamp in the private image's symbol table; the next symbol,
// sortedTimes, starts 64 bytes later.
// Inferred: the extent of quickLoadStamp is 64 bytes, from the call length and
// the gap to the next symbol.
// Unknown: what the stamp holds and who writes it.

extern "C" {
extern unsigned char quickLoadStamp[64];
void* memset(void* destination, int value, unsigned long size);

void resetQuickLoad__Fv() {
    memset(quickLoadStamp, 0, 64);
}
}
