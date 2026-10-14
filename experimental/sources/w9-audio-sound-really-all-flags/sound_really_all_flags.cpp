/* soundReallyAllFlags__Fi at 0x0024f300 is 136 bytes. Same walk as
 * soundAllFlags__Fi over the 452-byte records of `stream` (0x007dd290), with
 * one difference: the word at offset 32 is the starting value whether or not
 * the word at offset 20 is nonzero. The words at offset 108 + 48 * n are
 * OR-ed in for n below the count at offset 448, and the count is re-read
 * from the record on every pass. Nothing is written.
 */
extern "C" {
extern unsigned char stream[];

int soundReallyAllFlags__Fi(int index) {
    unsigned char* record = stream + index * 452;
    int flags = *reinterpret_cast<int*>(record + 32);
    volatile int* count = reinterpret_cast<volatile int*>(record + 448);
    for (int n = 0; n < *count; ++n)
        flags |= *reinterpret_cast<int*>(record + 108 + 48 * n);
    return flags;
}
}
