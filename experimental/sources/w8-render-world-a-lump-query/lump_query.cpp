// Observed lumpQuery__FPc at 0x00166e30, 220 bytes. The records of `lumps`
// (0x006cb280) are 40 bytes and nmLumps (0x00483f8c) of them are live. Records are
// visited in order; a record matches when strcmp of its name, which starts at
// offset 0, with the argument gives zero and bit 0 of the word at offset 20 is
// clear. After the scan, an index below nmLumps (read again, after the last
// strcmp) returns the word at offset 16 of that record; otherwise the result is
// zero. Nothing is written. lumpUnload__FPv compares the offset 16 word with its
// argument, so that word is the lump data pointer.
// Inferred: bit 0 of the offset 20 word marks a record that must not be found;
// the image only shows that such a record is skipped.
// Unknown: the name length bound and the rest of the record.
#ifdef NORRATH_W8_HOST
extern void* norrath_w8_word_to_pointer(unsigned word);
#define NORRATH_W8_POINTER(word) norrath_w8_word_to_pointer(word)
#else
#define NORRATH_W8_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
extern unsigned lumps[];
extern int nmLumps;
int strcmp(const char* left, const char* right);

void* lumpQuery__FPc(char* name) {
    int index = 0;
    for (; index < nmLumps; ++index) {
        const unsigned* const record = lumps + index * 10;
        if (strcmp(reinterpret_cast<const char*>(record), name) == 0 && (record[5] & 1u) == 0u) break;
    }
    if (index < nmLumps) return NORRATH_W8_POINTER(lumps[index * 10 + 4]);
    return nullptr;
}
}
