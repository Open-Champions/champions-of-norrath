// Observed: ReadStringLength__9VIObjFileRUs at 0x010a7640 is 164 bytes. Arguments:
// the object file (this) and a pointer to a 16-bit value. Flag at +584 and the
// 32-byte entry selected by the signed word at +232 are as in Read__9VIObjFileRs;
// the entry's count (this + index * 32 + 264) must be at least 2.
//  - If the flag is zero, or the count is below 2, the result is -1.
//  - Otherwise Read__6VIFilePvi(this, value, 2) runs (result ignored); unless the
//    word VIEndian is 1, VIBufferSwap__FiPUc(2, value) swaps the two bytes; the
//    count is reduced by 2 and the result is 0.
// Inferred: it reads the 16-bit length prefix that ReadStringBody then uses.
// Unknown: the member names.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
extern int VIEndian;

int ReadStringLength__9VIObjFileRUs(unsigned char* file, unsigned short* length) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    if (*reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) < 2) return -1;
    Read__6VIFilePvi(file, length, 2);
    if (VIEndian != 1) VIBufferSwap__FiPUc(2, reinterpret_cast<unsigned char*>(length));
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= 2;
    return 0;
}
}
