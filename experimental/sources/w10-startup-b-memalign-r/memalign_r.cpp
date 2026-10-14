/* Observed _memalign_r at 0x00301380, 456 bytes. Arguments: a reentrancy record, an
 * alignment and a size. Chunks have a size word at offset 4 whose low bit means the
 * previous chunk is in use; a chunk's user memory starts 8 bytes after the chunk.
 *   - An alignment of 16 or less returns _malloc_r(record, size) unchanged.
 *   - Otherwise the request is rounded up to a chunk size nb: 16 when size + 19 is
 *     below 31, else (size + 19) with the low four bits cleared. _malloc_r is called
 *     with nb + alignment + 16; a null result returns null.
 *   - __malloc_lock(record) is called and the chunk is the memory address minus 8.
 *   - When the memory address modulo alignment (a 64-bit unsigned remainder, through
 *     __umoddi3) is not zero: the first aligned address at or above the memory address
 *     is found, minus 8 gives a candidate chunk, and when it is less than 16 bytes
 *     above the original chunk the alignment is added to it. Its size word becomes the
 *     original chunk size minus the leading gap, with the previous-in-use bit set; the
 *     in-use bit of the chunk after it is set; the original chunk keeps its own
 *     previous-in-use bit and takes the gap as its size; _free_r(record, memory) frees
 *     the gap, and the new chunk replaces the original.
 *   - When the chunk is at least 16 bytes larger than nb, the tail becomes a chunk with
 *     size (excess | 1), the chunk keeps its previous-in-use bit and size nb, and the
 *     tail is released with _free_r(record, tail memory).
 *   - __malloc_unlock(record) is called and the chunk address plus 8 is returned.
 * memalign (w9-startup-crt-d) calls it. This matches the usual dlmalloc-style aligned
 * allocation; the sizes 16, 19 and 31 are the observed constants.
 * The alignment is applied by masking, so only powers of two give aligned results; other
 * values behave the same way in the original. The modulo is compiled here as a 32-bit
 * divide, where the original calls __umoddi3.
 * Unknown: the record's layout (only passed through). Compiled output is not claimed
 * to match the original.
 */
extern "C" {
void* _malloc_r(void* reent, unsigned size);
void _free_r(void* reent, void* memory);
void __malloc_lock(void* reent);
void __malloc_unlock(void* reent);

static inline unsigned& size_word(unsigned char* chunk) { return *reinterpret_cast<unsigned*>(chunk + 4); }

void* _memalign_r(void* reent, unsigned alignment, unsigned size) {
    if (alignment <= 16u) return _malloc_r(reent, size);
    const unsigned request = size + 19u;
    const unsigned nb = request < 31u ? 16u : (request & ~15u);
    unsigned char* const memory = static_cast<unsigned char*>(_malloc_r(reent, nb + alignment + 16u));
    if (memory == 0) return 0;
    __malloc_lock(reent);
    unsigned char* chunk = memory - 8;
    if (static_cast<unsigned long long>(reinterpret_cast<__UINTPTR_TYPE__>(memory)) % alignment != 0) {
        const __UINTPTR_TYPE__ aligned = (reinterpret_cast<__UINTPTR_TYPE__>(memory) + alignment - 1) &
                                         (0 - static_cast<__UINTPTR_TYPE__>(alignment));
        unsigned char* candidate = reinterpret_cast<unsigned char*>(aligned) - 8;
        if (candidate - chunk < 16) candidate += alignment;
        const unsigned gap = static_cast<unsigned>(candidate - chunk);
        const unsigned rest = (size_word(chunk) & ~3u) - gap;
        size_word(candidate) = rest | 1u;
        size_word(candidate + rest) |= 1u;
        size_word(chunk) = (size_word(chunk) & 1u) | gap;
        _free_r(reent, memory);
        chunk = candidate;
    }
    const unsigned chunk_size = size_word(chunk) & ~3u;
    const long long excess = static_cast<long long>(chunk_size) - static_cast<long long>(nb);
    if (excess >= 16) {
        unsigned char* const tail = chunk + nb;
        size_word(tail) = static_cast<unsigned>(excess) | 1u;
        size_word(chunk) = (size_word(chunk) & 1u) | nb;
        _free_r(reent, tail + 8);
    }
    __malloc_unlock(reent);
    return chunk + 8;
}
}
