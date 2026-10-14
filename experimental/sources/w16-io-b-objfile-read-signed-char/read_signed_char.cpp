// Observed: Read__9VIObjFileRSc at 0x010a6f70 is 108 bytes. Arguments: the object
// file (this) and a pointer to the byte to fill. The word at +584 is a flag and
// the signed word at +232 selects one of the 32-byte entries that start at +256;
// the entry's signed word at +8 (this + index * 32 + 264) is the count of bytes
// still to be read.
//  - If the flag is zero, or the count is zero or negative, the result is -1 and
//    nothing is read or changed.
//  - Otherwise it calls Read__6VIFilePvi(this, value, 1), ignoring its result,
//    reduces the count by 1 and returns 0. There is no byte swap.
// Inferred: same protocol as the 16-bit reader Read__9VIObjFileRs.
// Unknown: the member names.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);

int Read__9VIObjFileRSc(unsigned char* file, signed char* value) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    if (*reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) <= 0) return -1;
    Read__6VIFilePvi(file, value, 1);
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= 1;
    return 0;
}
}
