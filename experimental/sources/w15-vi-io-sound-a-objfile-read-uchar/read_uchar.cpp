// Observed: Read__9VIObjFileRUc at 0x010a6fe0, 108 bytes. Arguments: the object file (this) and a pointer to the value to fill. The word at +584 is a
// flag and the signed word at +232 selects one of the 32-byte entries that start at +256; the entry's signed word at +8 (that is,
// this + index * 32 + 264) is a count of bytes still to be read.
//  - If the flag is zero, or the count is not above zero, the result is -1 and nothing is read or changed.
//  - Otherwise it calls Read__6VIFilePvi(this, value, 1); that call's result is ignored.
//    The count is reduced by 1 and the result is 0.
// Inferred: the flag marks a file open for reading and the count is what is left of the current object's data; VIEndian equal to 1
// means the file already has the host's byte order (the image tests only for exactly 1). This one-byte read needs no swap.
// Unknown: the member names, and the meaning of VIEndian values other than 1.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);

int Read__9VIObjFileRUc(unsigned char* file, unsigned char* value) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    int* remaining = reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264);
    if (*remaining <= 0) return -1;
    Read__6VIFilePvi(file, value, 1);

    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= 1;
    return 0;
}
}
