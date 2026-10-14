// Observed: MemorySeek__6VIFilelQ26VIFile10SeekOrigin at 0x010021b0, 104 bytes. Arguments: the file, a 32-bit offset (sign-extended
// to 64 bits) and the origin value. The 64-bit word at +16 is the current position and the 64-bit word at +40 is the size.
//   origin 1: position = offset.
//   origin 0: position = position + offset.
//   origin 2: position = size + offset.
//   any other origin: nothing is stored and the result is -1.
// After the store the result is 0, and the word at +8 is cleared, when 0 <= position < size; otherwise it is -1 and the stored
// position stays (position == size is therefore also -1).
// Inferred: 0 / 1 / 2 are current / start / end relative modes; +8 is an end-of-data flag that a successful seek clears.
// Unknown: the member names and the enum spelling.
extern "C" {
int MemorySeek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long offset, int origin) {
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
        *reinterpret_cast<int*>(file + 8) = 0;
        return 0;
    }
    return -1;
}
}
