// Observed: ReadBegin__9VIObjFileUlRUsT2 at 0x010a6dc8 is 136 bytes. Arguments: the
// object file (this), a 64-bit offset and two halfword output pointers (T, V).
// Returns -1, changing nothing, when the word at +568 is zero, or the word at +572
// is nonzero, or the word at +576 is nonzero. Otherwise it calls
// Seek__6VIFilelQ26VIFile10SeekOrigin(this, offset, 1); the store of -1 in the
// signed word at +232 sits in that call's delay slot, so it happens before the
// call enters. It then calls the overload
// ReadBegin__9VIObjFileRUsT1RUlRUi(this, T, V, &position, &size) with two stack
// locals (a 64-bit and a word) as its last two outputs. A negative result from that
// returns -1; otherwise 0.
// Inferred: it opens the object that starts at an absolute file offset: it resets
// the nesting index to empty, then reads the header there. The seek origin 1 is the
// same value PostOpen__9VIObjFile uses for an absolute position.
// Unknown: the member names, and what the two discarded outputs are used for by
// other callers.
extern "C" {
int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long long offset, int origin);
int ReadBegin__9VIObjFileRUsT1RUlRUi(unsigned char* file, unsigned short* t, unsigned short* v, unsigned long long* position, unsigned* size);

int ReadBegin__9VIObjFileUlRUsT2(unsigned char* file, unsigned long long offset, unsigned short* t, unsigned short* v) {
    if (*reinterpret_cast<int*>(file + 568) == 0) return -1;
    if (*reinterpret_cast<int*>(file + 572) != 0) return -1;
    if (*reinterpret_cast<int*>(file + 576) != 0) return -1;
    *reinterpret_cast<int*>(file + 232) = -1;
    Seek__6VIFilelQ26VIFile10SeekOrigin(file, static_cast<long long>(offset), 1);
    unsigned long long position;
    unsigned size;
    return ReadBegin__9VIObjFileRUsT1RUlRUi(file, t, v, &position, &size) >= 0 ? 0 : -1;
}
}
