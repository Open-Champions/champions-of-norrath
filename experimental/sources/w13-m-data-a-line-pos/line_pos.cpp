// Observed linePos at 0x00486a00, a four-byte zero-initialised word in .bss. The next
// symbol is lastAmountPlayed at 0x00486a04. The lightning source stores zero here before
// it draws, and the line code reads it as the index into lineBuffer.
// The reference symbol is local; this unit exports it because another unit links to it.
// Unknown: the exact visibility the integrator will choose.
extern "C" {
int linePos;
}
static_assert(sizeof linePos == 4u, "linePos width");
