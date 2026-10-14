/* Observed MemcpyS at 0x002e4f58, 84 bytes. The first argument points at an object
 * whose signed word at byte offset 48 is a size limit (the same word CallRpc
 * compares against). The fourth argument, shifted left two, is a byte offset into
 * the source; the limit minus that offset is the room left. A null destination
 * returns 0 without copying. Otherwise a negative size, or a size larger than the
 * room left (signed comparison), returns -543 (0xfffffde1); else memcpy copies
 * `size` bytes from source + offset to the destination and 0 is returned
 * regardless of the copy's own result. What the object is, and why the offset is
 * counted in words, are unknown. Compiled output is not claimed to match the
 * original.
 */
extern "C" void* memcpy(void* destination, const void* source, unsigned size);

struct MemcpySObject {
    unsigned char before_limit[48];
    int limit;
};

extern "C" int MemcpyS(const void* object, void* destination, const void* source, int word_offset, int size) {
    const int room = static_cast<const MemcpySObject*>(object)->limit - word_offset * 4;
    if (destination == nullptr) return 0;
    if (size < 0 || room < size) return -543;
    memcpy(destination, static_cast<const unsigned char*>(source) + word_offset * 4, static_cast<unsigned>(size));
    return 0;
}
