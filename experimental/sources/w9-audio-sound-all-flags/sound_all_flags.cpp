/* soundAllFlags__Fi at 0x0024f288 is 116 bytes. `stream` (0x007dd290) holds
 * 452-byte records. For record `index` the result starts as the word at
 * offset 32 when the word at offset 20 is nonzero, otherwise as zero. The
 * word at offset 448 counts queue blocks; the words at offset 108 + 48 * n
 * (the +44 word of block n when blocks start at offset 64) are OR-ed in for
 * n from 0 up to the count. A count of zero or less ORs none. No bounds
 * check on `index`. Nothing is written.
 *
 * What the flag bits mean is not established here; callers pass the result
 * on as a mask.
 */
extern "C" {
extern unsigned char stream[];

int soundAllFlags__Fi(int index) {
    const unsigned char* record = stream + index * 452;
    int flags = 0;
    if (*reinterpret_cast<const int*>(record + 20) != 0)
        flags = *reinterpret_cast<const int*>(record + 32);
    const int count = *reinterpret_cast<const int*>(record + 448);
    for (int n = 0; n < count; ++n)
        flags |= *reinterpret_cast<const int*>(record + 108 + 48 * n);
    return flags;
}
}
