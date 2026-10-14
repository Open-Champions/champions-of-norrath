// Observed: ReadBegin__9VIObjFileRUsT1RUlRUi at 0x010a58d8, 464 bytes. Arguments: the object file (this), two halfword outputs
// (T, V), a 64-bit output (position) and a word output (size). Entry k below means the 32-byte record at this + 240 + k * 32; the
// signed word at +232 is the current index I (nesting depth), the word at +568 gates reading, +572 and +576 are nesting flags and
// +584 a "leaf" flag. An entry holds: halfword at +0, halfword at +2, word at +4, word at +8, doubleword at +16, word at +24.
// Returns -1, changing nothing further, when: +568 is zero; +572 is nonzero; I is 9; Eof__6VIFile(this) is nonzero; or I is not
// negative and the word at entry(I)+8 or the word at entry(I)+24 is zero.
// Otherwise: p = Tell__6VIFile(this) (64-bit); entry(I+1)+16 = p and *position = p; then Read__6VIFilePvi(this, entry(I+1), 12);
// a negative result returns -1. Then I becomes I + 1 (this store happens whether or not the order of bytes needs fixing). If the
// global VIEndian is not 1, VIBufferSwap__FiPUc is applied to the halfword at entry+0 (2 bytes), the halfword at +2 (2), the word
// at +4 (4) and the word at +8 (4) of the new entry, re-reading I for each call. *size = entry.word4 + 12; entry.word24 = entry.word4.
// If Eof__6VIFile(this) is nonzero it returns -1. Otherwise +576 = 1, *T = entry.halfword0, +584 = (entry.word8 == 0), *V =
// entry.halfword2 and the result is 0.
// Inferred: each nesting level reads a 12-byte object header (type, version, size, sub-object count), remembers where it began,
// and sets the remaining-bytes count to the stated size; +584 marks an object with no sub-objects, whose data may be read directly
// (the Read overloads test it). The size handed back includes the 12-byte header. The error paths after I is incremented leave it
// incremented.
// Unknown: member names, why depth 9 is refused (the entry table is probably 9 deep), and +576's reader.
extern "C" {
int Eof__6VIFile(unsigned char* file);
long long Tell__6VIFile(unsigned char* file);
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
extern int VIEndian;

int ReadBegin__9VIObjFileRUsT1RUlRUi(unsigned char* file, unsigned short* type, unsigned short* version, unsigned long long* position, unsigned* size) {
    if (*reinterpret_cast<int*>(file + 568) == 0) return -1;
    if (*reinterpret_cast<int*>(file + 572) != 0) return -1;
    if (*reinterpret_cast<int*>(file + 232) == 9) return -1;
    if (Eof__6VIFile(file) != 0) return -1;
    int index = *reinterpret_cast<int*>(file + 232);
    if (index >= 0) {
        if (*reinterpret_cast<int*>(file + index * 32 + 248) == 0) return -1;
        if (*reinterpret_cast<int*>(file + index * 32 + 264) == 0) return -1;
    }
    long long where = Tell__6VIFile(file);
    *reinterpret_cast<long long*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 288) = where;
    *position = where;
    if (Read__6VIFilePvi(file, file + *reinterpret_cast<int*>(file + 232) * 32 + 272, 12) < 0) return -1;
    *reinterpret_cast<int*>(file + 232) += 1;
    if (VIEndian != 1) {
        VIBufferSwap__FiPUc(2, file + *reinterpret_cast<int*>(file + 232) * 32 + 240);
        VIBufferSwap__FiPUc(2, file + *reinterpret_cast<int*>(file + 232) * 32 + 242);
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 244);
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 248);
    }
    *size = *reinterpret_cast<unsigned*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 244) + 12;
    unsigned char* entry = file + *reinterpret_cast<int*>(file + 232) * 32;
    *reinterpret_cast<unsigned*>(entry + 264) = *reinterpret_cast<unsigned*>(entry + 244);
    if (Eof__6VIFile(file) != 0) return -1;
    *reinterpret_cast<int*>(file + 576) = 1;
    unsigned char* top = file + *reinterpret_cast<int*>(file + 232) * 32 + 240;
    unsigned short topType = *reinterpret_cast<unsigned short*>(top);
    unsigned subs = *reinterpret_cast<unsigned*>(top + 8);
    *type = topType;
    *reinterpret_cast<int*>(file + 584) = subs == 0;
    *version = *reinterpret_cast<unsigned short*>(top + 2);
    return 0;
}
}
