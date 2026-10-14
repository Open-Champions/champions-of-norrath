// Observed: MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin at 0x01002488, 128 bytes. Arguments: the file, a 32-bit offset
// (sign-extended to 64 bits) and the origin. The position update is the one MemorySeek__6VIFilelQ26VIFile10SeekOrigin makes: with the
// 64-bit position at +16 and size at +40, origin 1 stores the offset, origin 0 adds it to the position, origin 2 adds it to the size,
// and any other origin returns -1 without storing. When 0 <= position < size afterwards it calls MemorySegmCalcSegm__6VIFile(file)
// to locate the segment, clears the word at +8 and returns 0; otherwise it returns -1 (the stored position stays).
// The call to MemorySegmCalcSegm__6VIFile leaves the offset and origin in the second and third argument registers, so the same two
// values reach that routine's otherwise unused extra parameters.
// Inferred: 0 / 1 / 2 are current / start / end relative modes.
// Unknown: the member names and the enum spelling.
extern "C" {
int MemorySegmCalcSegm__6VIFile(unsigned char* file, int offset, int origin);

int MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long offset, int origin) {
    long long* position = reinterpret_cast<long long*>(file + 16);
    long long base;
    if (origin == 1) {
        *position = offset;
    } else {
        if (origin == 0) {
            base = *position;
        } else if (origin == 2) {
            base = *reinterpret_cast<long long*>(file + 40);
        } else {
            return -1;
        }
        *position = base + offset;
    }
    long long now = *position;
    if (now >= 0 && now < *reinterpret_cast<long long*>(file + 40)) {
        MemorySegmCalcSegm__6VIFile(file, offset, origin);
        *reinterpret_cast<int*>(file + 8) = 0;
        return 0;
    }
    return -1;
}
}
