// Observed: Read__9VIObjFileRUl at 0x010a7370 is 160 bytes. Arguments: the object
// file (this) and a pointer to the value to fill. Flag at +584 and the 32-byte
// entry selected by the signed word at +232 are as in Read__9VIObjFileRs; the
// entry's count (this + index * 32 + 264) must be at least 8.
//  - If the flag is zero, or the count is below 8, the result is -1.
//  - Otherwise Read__6VIFilePvi(this, value, 8) runs (result ignored); unless the
//    word VIEndian is 1, VIBufferSwap__FiPUc(8, value) reverses the eight bytes;
//    the count is reduced by 8 and the result is 0.
// Inferred: the value is a 64-bit unsigned long, as the image's long is 64 bits
// wide; this also means the compile used 64-bit long for mangled "Ul".
// Unknown: the member names.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);
void VIBufferSwap__FiPUc(int size, unsigned char* data);
extern int VIEndian;

int Read__9VIObjFileRUl(unsigned char* file, unsigned long long* value) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    if (*reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) < 8) return -1;
    Read__6VIFilePvi(file, value, 8);
    if (VIEndian != 1) VIBufferSwap__FiPUc(8, reinterpret_cast<unsigned char*>(value));
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= 8;
    return 0;
}
}
