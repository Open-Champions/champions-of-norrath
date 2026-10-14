// Observed alwaysLoadedPage at 0x006e9390 (.bss), 64 bytes. The span to the next symbol (p) is 64 bytes. The authored texture-clear routine treats it as 8-byte records and clears the word at record offset 4.
// Inferred: Eight 8-byte records, matching the clear loop.
// Unknown: What the first word of a record is, and the table capacity limit.
extern "C" {
unsigned alwaysLoadedPage[16];
}
