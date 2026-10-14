// Observed partlist.1103 at 0x007d8010, a file-local .bss object with no recorded size.
// The next symbol, vatname.844, starts at 0x007d80c0, so 176 bytes is the most the
// object can hold; it begins on a 16-byte boundary. The only code found that reaches it
// is the Skelight drop-item routine, which stores a zero doubleword at its start.
// Naming: the image symbol carries the compiler's local suffix .1103, so it is defined
// here under that exact name.
// Unknown: the real size of the object (176 is the distance to the next symbol, not a
// measured extent), its element layout, and what the doubleword zero resets.
extern "C" {
alignas(16) unsigned char norrath_skelight_partlist_1103[176] __asm__("partlist.1103");
}
