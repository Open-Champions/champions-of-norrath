/* soundGetStreamPreloadDone__Fi at 0x0024aeb0 is 208 bytes. Stream record
 * `index` is 452 bytes from `stream`. It has eight 48-byte blocks starting
 * at offset 64; the flag word of block n is at 108 + 48 * n.
 *
 * The first block (from 0 to 7) whose flag word has bit 3 (0x8) set is
 * found. With none, the result is 1. Otherwise the result is 1 when the
 * word at offset 36 of the record is greater than 0xc800 (signed), or when
 * the word at offset 84 + 48 * n (field 20 of that block) is zero; it is 0
 * only when that word is nonzero and the offset-36 word is at most 0xc800.
 * Names for these words are not established here.
 */
extern "C" {
extern unsigned char stream[];

int soundGetStreamPreloadDone__Fi(int index) {
    unsigned char* record = stream + index * 452;
    int block = 0;
    if ((*reinterpret_cast<unsigned*>(record + 108) & 0x8u) == 0) {
        for (block = 1; block < 8; ++block) {
            if (*reinterpret_cast<unsigned*>(record + 108 + block * 48) & 0x8u) break;
        }
    }
    if (block == 8) return 1;
    if (*reinterpret_cast<int*>(record + 36) > 0xc800) return 1;
    return *reinterpret_cast<int*>(record + 84 + block * 48) == 0;
}
}
