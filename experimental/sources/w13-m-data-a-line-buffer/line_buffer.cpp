// Observed lineBuffer at 0x006f4880, a zero-initialised .bss array of 128 records of 36
// bytes (4608 bytes). The line code writes each record at lineBuffer + nmLines * 36.
// The reference symbol is local; this unit exports it because another unit links to it.
// Unknown: the record fields and the exact alignment of the original object.
extern "C" {
unsigned char lineBuffer[128 * 36];
}
static_assert(sizeof lineBuffer == 4608u, "lineBuffer extent");
