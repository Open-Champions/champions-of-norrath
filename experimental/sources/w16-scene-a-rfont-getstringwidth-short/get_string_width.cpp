// Observed: GetStringWidth__7VIRFontPCUsi at 0x01135640 is 396 bytes in the pinned SLUS-20565 image. The font's table is the word at +64:
// the number of 12-byte records is the word at +36 (read once), and each record's halfword at +6 is its key, in ascending order
// (the code runs a binary search on it). For each 16-bit unit c of the zero-terminated unsigned-short string it looks for a record whose key equals c;
// if there is none it looks for key 63 ('?'); if that is also missing it returns -1 at once, discarding the sum. For a record found it
// adds the record's byte at +10 plus a2 to the running total. The total is returned (0 for an empty string).
// Inferred: this is the 16-bit-text twin of GetStringWidth__7VIRFontPCci (the code is identical except the loads and the two-byte
// step); the result is a string width, the record byte at +10 being the glyph advance (InitChars writes it from the data bytes) and a2
// an extra per-glyph spacing.
// Unknown: the meaning of the rest of each record, and the unit. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
namespace {
int find_record16(unsigned char* font, int count, unsigned key) {
    int lo = 0, hi = count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64));
        unsigned k = *reinterpret_cast<unsigned short*>(table + mid * 12 + 6);
        if (k == key) return mid;
        if (key < k) hi = mid - 1; else lo = mid + 1;
    }
    return -1;
}
}
extern "C" {
int GetStringWidth__7VIRFontPCUsi(unsigned char* self, const unsigned short* text, int extra) {
    int total = 0;
    int count = *reinterpret_cast<int*>(self + 36);
    while (*text != 0) {
        int idx = find_record16(self, count, *text);
        if (idx < 0) {
            idx = find_record16(self, count, 63);
            if (idx < 0) return -1;
        }
        unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
        total += table[idx * 12 + 10] + extra;
        ++text;
    }
    return total;
}
}
