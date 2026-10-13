// Observed soundGetDialogSize__Fi at 0x0024ed48, 156 bytes, called from
// executeAction. soundGetDialogDir__Fv returns a table of 80-byte records.
// A null table gives 0. For an index of zero or more, records 0 .. index
// are tested in order and a zero first byte in any of them gives 0 (the
// test is on the signed byte). A negative index is not tested and selects
// the record at index * 80 before the table. For the selected record the
// result is the signed difference word[+144] - word[+64]; the second word
// lies in the following record.
// Inferred: the difference is a size taken from two offsets. Unknown: what
// the records hold and why the end word is read from the next record.
extern "C" const signed char* soundGetDialogDir__Fv();

extern "C" int soundGetDialogSize__Fi(int index) {
    const signed char* dir = soundGetDialogDir__Fv();
    if (dir == 0) return 0;
    if (index >= 0) {
        const signed char* p = dir;
        int n = 0;
        do {
            if (*p == 0) return 0;
            p += 80;
            ++n;
        } while (index >= n);
    }
    const signed char* record = dir + index * 80;
    return *reinterpret_cast<const int*>(record + 144) - *reinterpret_cast<const int*>(record + 64);
}
