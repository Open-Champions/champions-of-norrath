// Observed: topArg at 0x008de0c8. The symbol size is 0; the next symbol, q.12,
// is 1080 bytes later. InitThread clears its first two words and passes its
// address to StartThread. topThread reads the word at offset 0, masks it with
// 511, and reads two request bytes at offset 8 + 2 * index. iWakeupThread
// uses the word at offset 4 as its own cursor and writes request bytes at
// the same stride. The highest byte that formula reaches is offset 1031.
// Inferred: the extent is the 1080-byte gap to the next symbol. The word
// alignment follows the 64-bit-aligned original address.
// Unknown: whether any byte past offset 1031 is used.
extern "C" {
alignas(8) unsigned char topArg[1080];
}
