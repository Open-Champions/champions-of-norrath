// Observed: Read__6VIFilePvi at 0x010cafa8 is 124 bytes. Arguments: this, buffer, count.
// The word at offset 0 of this selects the path:
//   - non-negative: BufferedRead__6VIFilePvi(this, buffer, count) is called and the routine
//     returns 0 if its result equals count, otherwise -1.
//   - negative and the word at offset 48 is nonzero: tail call of MemoryRead__6VIFilePvi.
//   - negative, word at 48 zero, word at offset 56 nonzero: tail call of MemorySegmRead__6VIFilePvi.
//   - otherwise -1.
// Inferred: offset 0 is a descriptor-like value whose sign picks buffered or in-memory access;
// the two nonzero words look like memory-source pointers. Nothing proves those meanings.
// Unknown: the object size and the return types of the three callees (only their low words
// are used here).
extern "C" {
int BufferedRead__6VIFilePvi(void* self, void* buffer, int count);
int MemoryRead__6VIFilePvi(void* self, void* buffer, int count);
int MemorySegmRead__6VIFilePvi(void* self, void* buffer, int count);

int Read__6VIFilePvi(void* self, void* buffer, int count) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<int*>(bytes) >= 0)
        return BufferedRead__6VIFilePvi(self, buffer, count) == count ? 0 : -1;
    if (*reinterpret_cast<int*>(bytes + 48) != 0) return MemoryRead__6VIFilePvi(self, buffer, count);
    if (*reinterpret_cast<int*>(bytes + 56) != 0) return MemorySegmRead__6VIFilePvi(self, buffer, count);
    return -1;
}
}
