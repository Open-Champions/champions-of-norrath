// Observed: Write__6VIFilePCvi at 0x010cb028, 124 bytes. Arguments: the file (this), a buffer and a byte count N.
//  - If the signed word at +0 is not negative: BufferedWrite__6VIFilePCvi(this, buffer, N); the result is 0 when it equals N and -1
//    otherwise.
//  - Else if the word at +48 is nonzero: the result is MemoryWrite__6VIFilePCvi(this, buffer, N), returned as is.
//  - Else if the word at +56 is nonzero: the result is MemorySegmWrite__6VIFilePCvi(this, buffer, N), returned as is.
//  - Else -1.
// Inferred: +0 is a disc descriptor and +48 / +56 select a memory block or a segment list as the backing store, the same split that
// Read__6VIFilePvi (0x010cafa8) makes. 0 / -1 means success / failure for the buffered path.
// Unknown: the member names, and the return convention of the two memory writers.
extern "C" {
int BufferedWrite__6VIFilePCvi(unsigned char* file, const void* buffer, int count);
int MemoryWrite__6VIFilePCvi(unsigned char* file, const void* buffer, int count);
int MemorySegmWrite__6VIFilePCvi(unsigned char* file, const void* buffer, int count);

int Write__6VIFilePCvi(unsigned char* file, const void* buffer, int count) {
    if (*reinterpret_cast<int*>(file) >= 0) return BufferedWrite__6VIFilePCvi(file, buffer, count) == count ? 0 : -1;
    if (*reinterpret_cast<int*>(file + 48) != 0) return MemoryWrite__6VIFilePCvi(file, buffer, count);
    if (*reinterpret_cast<int*>(file + 56) != 0) return MemorySegmWrite__6VIFilePCvi(file, buffer, count);
    return -1;
}
}
