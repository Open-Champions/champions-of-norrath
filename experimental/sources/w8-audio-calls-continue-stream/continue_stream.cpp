/* soundContinueStream__Fi at 0x0024f218 is 108 bytes. In stream record
 * `index` (452 bytes each from `stream`), the queue holds the count at offset
 * 448 of 48-byte blocks from offset 64. The flag word of block n is at
 * 108 + 48 * n. The first block, in order, whose flag word has bit 3 (0x8)
 * set gets that bit cleared, and the function returns. A count of zero or
 * less, or no flagged block among the counted ones, changes nothing.
 */
extern "C" {
extern unsigned char stream[];

void soundContinueStream__Fi(int index) {
    unsigned char* record = stream + index * 452;
    const int count = *reinterpret_cast<int*>(record + 448);
    if (count <= 0) return;
    unsigned* flags = reinterpret_cast<unsigned*>(record + 108);
    for (int block = 0; block < count; ++block) {
        if (*flags & 0x8u) {
            *flags &= ~0x8u;
            return;
        }
        flags += 12;
    }
}
}
