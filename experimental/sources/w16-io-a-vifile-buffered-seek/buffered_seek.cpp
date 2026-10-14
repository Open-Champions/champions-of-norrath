// Observed: BufferedSeek__6VIFilelQ26VIFile10SeekOrigin at 0x010ca8d0, 292 bytes. Arguments: the file, a 32-bit offset (sign-extended
// to 64 bits) and the origin. File fields used (all offsets from the file): word +0 (descriptor), +8 (end flag), 64-bit +16 (position),
// 64-bit +24 (the descriptor's own position), unsigned word +76 (size), 64-bit +136 (file offset of the buffered block), 64-bit +144
// and +152 (start and end of the dirty range inside the buffer), 64-bit +160 (position inside the buffer), word +188 (buffer
// address) and 64-bit +176 (bytes held in the buffer).
//   1. Position update: origin 1 stores the offset; origin 0 adds it to the position; origin 2 adds it to the unsigned word at +76; any
//      other origin returns -1 with nothing stored.
//   2. When the buffer holds no bytes (+176 is zero): clear +8 and return 0.
//   3. When the new position lies in [+136, +136 + +176): store position - +136 at +160, clear +8 and return 0.
//   4. Otherwise the buffer is dropped: when the dirty range is non-empty (+144 differs from +152), the dirty bytes are written back:
//      T = +136 + +144 and n = (int)(+152 - +144); when T differs from +24, sceLseek(descriptor, (int)T, 0) is called and its result is
//      stored at +24, and a result other than T skips the write; then sceWrite(descriptor, buffer + (low word of +144), n) is called and
//      its result is added to +24; when the write returned n, +144 and +152 are cleared.
//   5. BufferClear__6VIFile(file) runs, +8 is cleared and the result is 0.
// Inferred: a read/write buffered file; +8 is an end-of-data flag, +136 the file offset of the cached block and the dirty range the
// part not yet written. 0 / 1 / 2 are current / start / end relative modes.
// The buffer address is passed to sceWrite as a 32-bit address word.
// Unknown: the member names, the meaning of the sceLseek whence value 0, and the size source at +76.
extern "C" {
int sceLseek(int descriptor, int offset, int whence);
int sceWrite(int descriptor, unsigned buffer, int size);
int BufferClear__6VIFile(unsigned char* file);

int BufferedSeek__6VIFilelQ26VIFile10SeekOrigin(unsigned char* file, long offset, int origin) {
    long long* position = reinterpret_cast<long long*>(file + 16);
    if (origin == 1) {
        *position = offset;
    } else {
        long long base;
        if (origin == 0) {
            base = *position;
        } else if (origin == 2) {
            base = *reinterpret_cast<unsigned*>(file + 76);
        } else {
            return -1;
        }
        *position = base + offset;
    }
    long long held = *reinterpret_cast<long long*>(file + 176);
    if (held == 0) {
        *reinterpret_cast<int*>(file + 8) = 0;
        return 0;
    }
    long long now = *position;
    long long start = *reinterpret_cast<long long*>(file + 136);
    if (!(now < start) && now < start + held) {
        *reinterpret_cast<long long*>(file + 160) = now - start;
        *reinterpret_cast<int*>(file + 8) = 0;
        return 0;
    }
    long long dirtyStart = *reinterpret_cast<long long*>(file + 144);
    long long dirtyEnd = *reinterpret_cast<long long*>(file + 152);
    if (dirtyStart != dirtyEnd) {
        long long target = start + dirtyStart;
        int size = static_cast<int>(dirtyEnd - dirtyStart);
        bool write = true;
        if (target != *reinterpret_cast<long long*>(file + 24)) {
            long long moved = sceLseek(*reinterpret_cast<int*>(file), static_cast<int>(target), 0);
            *reinterpret_cast<long long*>(file + 24) = moved;
            if (target != moved) write = false;
        }
        if (write) {
            int written = sceWrite(*reinterpret_cast<int*>(file), *reinterpret_cast<unsigned*>(file + 188) + *reinterpret_cast<unsigned*>(file + 144), size);
            *reinterpret_cast<long long*>(file + 24) = written + *reinterpret_cast<long long*>(file + 24);
            if (written == size) {
                *reinterpret_cast<long long*>(file + 144) = 0;
                *reinterpret_cast<long long*>(file + 152) = 0;
            }
        }
    }
    BufferClear__6VIFile(file);
    *reinterpret_cast<int*>(file + 8) = 0;
    return 0;
}
}
