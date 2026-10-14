// Observed: Read__9VIObjFileRf at 0x010a7410, 160 bytes. Arguments: the object file (this) and a pointer to the value to fill. The word at +584 is a
// flag and the signed word at +232 selects one of the 32-byte entries that start at +256; the entry's signed word at +8 (that is,
// this + index * 32 + 264) is a count of bytes still to be read.
//  - If the flag is zero, or the count is below 4, the result is -1 and nothing is read or changed.
//  - Otherwise it calls Read__6VIFilePvi(this, value, 4); that call's result is ignored.
//    Then, unless the global word VIEndian is 1, it calls VIBufferSwap__FiPUc(4, value) to reverse the bytes.
//    The count is reduced by 4 and the result is 0.
// Inferred: the flag marks a file open for reading and the count is what is left of the current object's data; VIEndian equal to 1
// means the file already has the host's byte order (the image tests only for exactly 1).
// Unknown: the member names, and the meaning of VIEndian values other than 1.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
extern int VIEndian;

int Read__9VIObjFileRf(unsigned char* file, float* value) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    int* remaining = reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264);
    if (*remaining < 4) return -1;
    Read__6VIFilePvi(file, value, 4);
    if (VIEndian != 1) VIBufferSwap__FiPUc(4, reinterpret_cast<unsigned char*>(value));
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= 4;
    return 0;
}
}
