/* soundGetDialogSize__FPc at 0x0024ede8 is 48 bytes. It looks the name up
 * with soundFindDialog__FPc. A null result returns 0. Otherwise it returns
 * the word at offset 144 minus the word at offset 64 of the found record.
 * Offset 144 is offset 64 of the next 80-byte record, so the value is the
 * distance between this record's start word and the next record's.
 */
extern "C" {
unsigned char* soundFindDialog__FPc(char* name);

int soundGetDialogSize__FPc(char* name) {
    unsigned char* record = soundFindDialog__FPc(name);
    if (record == nullptr) return 0;
    const int start = *reinterpret_cast<const int*>(record + 64);
    const int next = *reinterpret_cast<const int*>(record + 144);
    return next - start;
}
}
