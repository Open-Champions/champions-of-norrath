// Observed: WriteEnd__9VIObjFileRUi at 0x010a5710, 452 bytes. Arguments: the object file (this) and a word output (size). Entry k
// means the 32-byte record at this + 240 + k * 32 (see ReadBegin__9VIObjFileRUsT1RUlRUi); I is the signed word at +232.
// If the word at +572 is zero the result is -1 with no other effect. Otherwise:
//  1. here = Tell__6VIFile(this) (64-bit).
//  2. Seek__6VIFilelQ26VIFile10SeekOrigin(this, entry(I).doubleword16 + 4, 1).
//  3. *size = entry(I).word4 + 12. E = the global VIEndian.
//  4. If E is not 1, VIBufferSwap__FiPUc(4, ...) on entry(I)+4 and entry(I)+8, then Write__6VIFilePCvi(this, entry(I)+4, 8) (size
//     and sub-object count), then if E is not 1 the same two swaps again to put them back. (With E equal to 1 only the write.)
//  5. Seek__6VIFilelQ26VIFile10SeekOrigin(this, here, 1). I becomes I - 1.
//  6. If the new I is -1: the word at +204 is incremented and +572 set to zero. Otherwise the parent entry(I) gets
//     word4 += entry(I+1).word4, then word4 += 12, then word8 += 1.
//  7. +580 is set to zero and the result is 0.
// Inferred: the counterpart of ReadBegin: it closes the innermost object by going back to its header, writing the final size and
// sub-object count there, returning to the end, and folding the child's size (plus its 12-byte header) into the parent. +204 counts
// completed top-level objects. Write's result is not tested.
// Unknown: member names, the meaning of +580, and why the swap-back is done on the write path.
extern "C" {
long long Tell__6VIFile(unsigned char* file);
int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long long offset, int origin);
int Write__6VIFilePCvi(unsigned char* file, const void* buffer, int count);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
extern int VIEndian;

int WriteEnd__9VIObjFileRUi(unsigned char* file, unsigned* size) {
    if (*reinterpret_cast<int*>(file + 572) == 0) return -1;
    long long here = Tell__6VIFile(file);
    Seek__6VIFilelQ26VIFile10SeekOrigin(file, *reinterpret_cast<long long*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 256) + 4, 1);
    int endian = VIEndian;
    *size = *reinterpret_cast<unsigned*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 244) + 12;
    if (endian != 1) {
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 244);
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 248);
    }
    Write__6VIFilePCvi(file, file + *reinterpret_cast<int*>(file + 232) * 32 + 244, 8);
    if (endian != 1) {
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 244);
        VIBufferSwap__FiPUc(4, file + *reinterpret_cast<int*>(file + 232) * 32 + 248);
    }
    Seek__6VIFilelQ26VIFile10SeekOrigin(file, here, 1);
    int index = *reinterpret_cast<int*>(file + 232) - 1;
    *reinterpret_cast<int*>(file + 232) = index;
    if (index == -1) {
        *reinterpret_cast<int*>(file + 572) = 0;
        *reinterpret_cast<int*>(file + 204) += 1;
    } else {
        unsigned char* parent = file + index * 32 + 240;
        *reinterpret_cast<unsigned*>(parent + 4) += *reinterpret_cast<unsigned*>(file + index * 32 + 276);
        *reinterpret_cast<unsigned*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 244) += 12;
        *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 248) += 1;
    }
    *reinterpret_cast<int*>(file + 580) = 0;
    return 0;
}
}
