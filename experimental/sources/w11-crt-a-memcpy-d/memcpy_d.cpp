// Observed MemcpyD at 0x002e4f00, 84 bytes, pinned SLUS_205.65 (a local symbol).
// Arguments: an object, a destination base, a word index, a source and a byte
// count (the fifth argument arrives in $t0). The object's word at offset 48 is a
// capacity in bytes; the room left is that capacity minus four times the index.
// A null source returns 0 before anything else is looked at. Otherwise a negative
// count, or a count larger than the room left, returns -543; else the count bytes
// are copied to base + 4 * index with memcpy and 0 is returned. The index itself is
// never range-checked and the memcpy result is not used.
// Inferred: the object is the RPC client of the sceInet* wrappers (their callers pass
// a request buffer and word offsets 1, 3, 5 and 9). Unknown: what the capacity word
// is called, and the meaning of -543 beyond "does not fit".
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ size);

int MemcpyD(void* object, void* buffer, int word_index, const void* source, int size) {
    const int room = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 48) - word_index * 4;
    if (source == 0) return 0;
    if (size < 0 || room < size) return -543;
    memcpy(static_cast<unsigned char*>(buffer) + word_index * 4, source, static_cast<__SIZE_TYPE__>(size));
    return 0;
}
}
