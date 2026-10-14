// Observed: SetText__9VIWndEulaPCUsi at 0x010b57f0 is 184 bytes. With (this, text, count) it allocates
// 2 * count + 2 bytes with __builtin_vec_new and stores the pointer in the word at +1552. It then copies
// count 16-bit units from text into the new buffer, replacing every unit equal to 9 with 32, stores 1 in
// the word at +1548, and writes a 16-bit zero at unit index count. Nothing is returned. The loop does not run
// when count is not positive, but the allocation, the store of 1 and the terminator write still happen.
// Inferred: text is a UTF-16 style string of count units that the licence-text window keeps as a private
// zero-terminated copy with tabs turned into spaces; the word at +1548 is a "text is set" flag. Any earlier
// buffer at +1552 is not released here.
// Unknown: the field names, the unit encoding, whether the 1 at +1548 is a flag or a count, and the layout of
// the window class.
extern "C" {
void* __builtin_vec_new(unsigned long size);

void SetText__9VIWndEulaPCUsi(void* self, const unsigned short* text, int count) {
    int* words = static_cast<int*>(self);
    unsigned short* copy = static_cast<unsigned short*>(__builtin_vec_new(count * 2 + 2));
    words[388] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(copy));
    for (int i = 0; i < count; ++i) {
        const unsigned short unit = text[i];
        copy[i] = unit;
        if (unit == 9) copy[i] = 32;
    }
    words[387] = 1;
    copy[count] = 0;
}
}
