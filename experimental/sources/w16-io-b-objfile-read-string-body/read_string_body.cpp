// Observed: ReadStringBody__9VIObjFilePcUsUs at 0x010a7780 is 224 bytes. Arguments:
// the object file (this), a character buffer, a 16-bit string length (length) and
// a 16-bit buffer capacity (capacity). Flag at +584 and the 32-byte entry selected
// by the signed word at +232 are as in Read__9VIObjFileRs; the entry's count
// (this + index * 32 + 264) must be at least length.
//  - If the flag is zero, or the count is below length, the result is -1.
//  - If capacity - 1 is below length (the string does not fit): Read__6VIFilePvi
//    (this, buffer, capacity - 1), then Seek__6VIFilelQ26VIFile10SeekOrigin(this,
//    length - capacity + 1, 0) skips the unread rest, then buffer[capacity - 1]
//    is set to zero.
//  - Otherwise Read__6VIFilePvi(this, buffer, length) and buffer[length] = 0.
//  Either way the count is then reduced by length (not by what was stored) and the
//  result is 0. Read's results are ignored. No byte swap.
// Inferred: this is the body read for a length-prefixed string, truncating to the
// caller's buffer. A capacity of zero would write at buffer[-1]; callers are
// unknown.
// Unknown: the member names, and the meaning of seek origin 0 (the image passes it
// as a plain 0).
extern "C" {
int Read__6VIFilePvi(unsigned char* file, void* buffer, int count);
int Seek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long long offset, int origin);

int ReadStringBody__9VIObjFilePcUsUs(unsigned char* file, char* buffer, unsigned short length, unsigned short capacity) {
    if (*reinterpret_cast<int*>(file + 584) == 0) return -1;
    if (*reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) < length) return -1;
    int fits = capacity - 1;
    if (fits < length) {
        Read__6VIFilePvi(file, buffer, fits);
        Seek__6VIFilelQ26VIFile10SeekOrigin(file, length - capacity + 1, 0);
        buffer[capacity - 1] = 0;
    } else {
        Read__6VIFilePvi(file, buffer, length);
        buffer[length] = 0;
    }
    *reinterpret_cast<int*>(file + *reinterpret_cast<int*>(file + 232) * 32 + 264) -= length;
    return 0;
}
}
