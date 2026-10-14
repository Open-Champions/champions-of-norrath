// Observed: Read__9VIObjFilePUci at 0x010a7550, 128 bytes. Arguments: the object file (this), a buffer and a byte count N. The word
// at +584 is a flag and the signed word at +232 selects one of the 32-byte entries that start at +240; the entry's signed word at
// +24 (this + index * 32 + 264) is a count of bytes still to be read.
//  - If the flag is zero, or the count is below N (signed compare), the result is -1 and nothing is read or changed.
//  - Otherwise it calls Read__6VIFilePvi(this, buffer, N), ignoring its result, subtracts N from the count and returns 0.
// Inferred: same scheme as the fixed-size Read__9VIObjFile overloads, without byte swapping, so it is for raw data. A negative N
// passes the compare and is subtracted as given.
// Unknown: the member names, and whether negative counts occur.
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);

int Read__9VIObjFilePUci(unsigned char* file, unsigned char* buffer, int count) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    if (*reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) < count) return -1;
    Read__6VIFilePvi(file, buffer, count);
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= count;
    return 0;
}
}
