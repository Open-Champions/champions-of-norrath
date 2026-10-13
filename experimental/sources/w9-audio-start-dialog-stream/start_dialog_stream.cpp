/* soundStartDialogStream__Fiii at 0x0024a2c8 is 200 bytes. The table comes
 * from soundGetDialogDir__Fv (80-byte records, a record whose first byte is
 * zero ends the table); a null table starts nothing.
 *
 * For a non-negative `index` the first index + 1 records must all have a
 * nonzero first byte, checked in order; a zero byte at record k (k <= index)
 * starts nothing. A negative index skips that check. Record `index` is then
 * used without any further bound (a negative index reaches before the table).
 *
 * soundStartStream__FiPciiiiii is called with:
 *
 *   stream      the first argument
 *   file name   soundGetDialogFilename__Fv()
 *   flags       the third argument, unchanged
 *   offset      word at 64 of the record
 *   length      word at 144 minus word at 64 (not tested for equality here)
 *   next three  word at 68, halfword at 72 (signed), word at 76
 *
 * Nothing is returned. What the last three arguments mean is not established.
 */
extern "C" {
unsigned char* soundGetDialogDir__Fv();
char* soundGetDialogFilename__Fv();
void soundStartStream__FiPciiiiii(int stream, char* file, int flags, int offset, int length,
                                  int a5, int a6, int a7);

void soundStartDialogStream__Fiii(int stream, int index, int flags) {
    unsigned char* table = soundGetDialogDir__Fv();
    if (table == nullptr) return;
    if (index >= 0) {
        const unsigned char* probe = table;
        int checked = 0;
        do {
            if (probe[0] == 0) return;
            probe += 80;
            ++checked;
        } while (checked <= index);
    }
    unsigned char* record = table + index * 80;
    char* file = soundGetDialogFilename__Fv();
    soundStartStream__FiPciiiiii(stream, file, flags, *reinterpret_cast<int*>(record + 64),
                                 *reinterpret_cast<int*>(record + 144) - *reinterpret_cast<int*>(record + 64),
                                 *reinterpret_cast<int*>(record + 68),
                                 *reinterpret_cast<short*>(record + 72),
                                 *reinterpret_cast<int*>(record + 76));
}
}
