// Observed: Seek__6VIFilelQ26VIFile10SeekOrigin at 0x010caee8 is 92 bytes. Arguments: this,
// offset (32-bit long), origin (enum, passed through unchanged). Dispatch uses the same words as
// Read__6VIFilePvi: word at offset 0 non-negative -> tail call BufferedSeek; else word at offset
// 48 nonzero -> tail call MemorySeek; else word at offset 56 nonzero -> tail call MemorySegmSeek;
// else returns -1. The callee results are returned unchanged.
// Unknown: the return type of the callees (only the low word is used here) and the enum values.
extern "C" {
int BufferedSeek__6VIFilelQ26VIFile10SeekOrigin(void* self, long offset, int origin);
int MemorySeek__6VIFilelQ26VIFile10SeekOrigin(void* self, long offset, int origin);
int MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(void* self, long offset, int origin);

int Seek__6VIFilelQ26VIFile10SeekOrigin(void* self, long offset, int origin) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<int*>(bytes) >= 0) return BufferedSeek__6VIFilelQ26VIFile10SeekOrigin(self, offset, origin);
    if (*reinterpret_cast<int*>(bytes + 48) != 0) return MemorySeek__6VIFilelQ26VIFile10SeekOrigin(self, offset, origin);
    if (*reinterpret_cast<int*>(bytes + 56) != 0) return MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(self, offset, origin);
    return -1;
}
}
