/* soundStartGlobalDialogStream__FiPci at 0x0024ebc0 is 116 bytes. The name is
 * looked up with soundFindGlobalDialog__FPc. A missing record, or one whose
 * words at offsets 64 and 144 are equal, starts nothing. Otherwise stream
 * `stream` is started with soundStartStream__FiPciiiiii:
 *
 *   file name   "mouth.vat" (rodata 0x00474630, the string soundStartGlobalMusic__FPci uses)
 *   flags       the third argument, unchanged
 *   offset      word at 64
 *   length      word at 144 minus word at 64
 *   next three  word at 68, halfword at 72 (signed), word at 76
 *
 * Unlike soundStartGlobalMusic__FPci no flag bits are added and the stream
 * volume is not touched. Nothing is returned. The meaning of the last three
 * arguments is established by neither this function nor the record layout.
 */
extern "C" {
unsigned char* soundFindGlobalDialog__FPc(char* name);
void soundStartStream__FiPciiiiii(int stream, char* file, int flags, int offset, int length,
                                  int a5, int a6, int a7);

void soundStartGlobalDialogStream__FiPci(int stream, char* name, int flags) {
    unsigned char* record = soundFindGlobalDialog__FPc(name);
    if (record == nullptr) return;
    const int start = *reinterpret_cast<int*>(record + 64);
    const int end = *reinterpret_cast<int*>(record + 144);
    if (end == start) return;
    soundStartStream__FiPciiiiii(stream, const_cast<char*>("mouth.vat"), flags, start, end - start,
                                 *reinterpret_cast<int*>(record + 68),
                                 *reinterpret_cast<short*>(record + 72),
                                 *reinterpret_cast<int*>(record + 76));
}
}
