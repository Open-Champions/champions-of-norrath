/* startDlgRandom__FP9dialogDirPciT1i at 0x0024a430 is 412 bytes (a local
 * function of the sound unit; the argument types come from the mangled
 * name: table, file name, int, second char pointer, int).
 *
 * `table` is a dialog directory: 80-byte records, a first byte of zero ends
 * it. A record qualifies when its name starts with the whole of `name`
 * (strncmp against strlen(name) bytes) and its signed halfword at offset 74
 * equals the signed byte `language` (0x004844b4). Records with a zero-tag
 * fallback, as soundFindDialog uses, are not considered here.
 *
 * If no record qualifies nothing happens and eRandState is not touched.
 * Otherwise eRandState (0x00483e58) advances once by
 * state * 0x19660d + 0x3c6ef35f, and the qualifying record number
 * (state >> 16) % count is chosen (state taken after the update, as an
 * unsigned 32-bit value; the remainder is the signed remainder of two
 * non-negative values). A second walk finds that record. Should the walk
 * not find it, the original goes on with record -1; that is kept.
 *
 * soundStartStream__FiPciiiiii is then called unless the words at offsets 64
 * and 144 of the record are equal:
 *
 *   stream      the third argument
 *   file name   the second argument
 *   flags       the fifth argument
 *   offset      word at 64
 *   length      word at 144 minus word at 64
 *   next three  word at 68, halfword at 72 (signed), word at 76
 *
 * Nothing is returned.
 */
extern "C" {
extern signed char language;
extern unsigned eRandState;
unsigned long strlen(const char* text);
int strncmp(const char* left, const char* right, unsigned long count);
void soundStartStream__FiPciiiiii(int stream, char* file, int flags, int offset, int length,
                                  int a5, int a6, int a7);

static bool qualifies(const unsigned char* record, const char* name, unsigned long length) {
    return strncmp(name, reinterpret_cast<const char*>(record), length) == 0 &&
           *reinterpret_cast<const short*>(record + 74) == language;
}

void startDlgRandom__FP9dialogDirPciT1i(unsigned char* table, char* file, int stream, char* name, int flags) {
    const unsigned long length = strlen(name);
    int count = 0;
    if (table[0] != 0) {
        for (const unsigned char* record = table; record[0] != 0; record += 80)
            if (qualifies(record, name, length)) ++count;
    }
    if (count == 0) return;
    const unsigned state = eRandState * 0x19660du + 0x3c6ef35fu;
    eRandState = state;
    int pick = static_cast<int>((state >> 16) % static_cast<unsigned>(count));
    int chosen = -1;
    int index = 0;
    for (const unsigned char* record = table; record[0] != 0; record += 80, ++index) {
        if (!qualifies(record, name, length)) continue;
        if (pick > 0) { --pick; continue; }
        chosen = index;
        break;
    }
    unsigned char* record = table + chosen * 80;
    const int start = *reinterpret_cast<int*>(record + 64);
    const int end = *reinterpret_cast<int*>(record + 144);
    if (end == start) return;
    soundStartStream__FiPciiiiii(stream, file, flags, start, end - start,
                                 *reinterpret_cast<int*>(record + 68),
                                 *reinterpret_cast<short*>(record + 72),
                                 *reinterpret_cast<int*>(record + 76));
}
}
