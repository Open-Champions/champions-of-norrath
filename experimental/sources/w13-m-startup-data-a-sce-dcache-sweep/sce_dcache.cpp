/* Observed _sceSDC at 0x002d5930, 164 bytes, pinned SLUS_205.65. It is the local
 * routine SyncDCache calls with begin and end already rounded to 64-byte lines.
 *
 * Observed order of effects, for each index from 0 to 4032 in steps of 64:
 *   1. sync, then cache operation 0x10 at offset 0 on that index, then sync.
 *   2. TagLo is read. Its bits 12 and up, plus the index, give an address.
 *   3. When that address lies in the inclusive range [begin, end], sync,
 *      cache operation 0x14 at offset 0, then sync.
 *   4. sync, then the same sequence for offset 1: cache 0x10, TagLo read,
 *      and cache 0x14 at offset 1 when the address is in range.
 *   5. sync at the end of the iteration.
 * The 64-bit comparisons of the original use sign-extended values, so the
 * address computation below keeps that width.
 *
 * Inferred: this writes back and invalidates the two data-cache ways for the
 * lines whose tags fall in the range. Unknown: the exact cache-op names and
 * whether the 0x10 index operation is the one the hardware documents for TagLo.
 */
extern "C" void _sceSDC(unsigned begin, unsigned end);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_sync();
extern "C" void norrath_host_cache_op(unsigned op, unsigned offset, unsigned index);
extern "C" unsigned norrath_host_taglo();
#define NORRATH_SYNC() norrath_host_sync()
#define NORRATH_CACHE_OP(op, offset, index) norrath_host_cache_op((op), (offset), (index))
#define NORRATH_TAGLO() norrath_host_taglo()
#else
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#define NORRATH_CACHE_OP(op, offset, index) \
    asm volatile("cache %0, %1(%2)" ::"i"(op), "i"(offset), "r"(index) : "memory")
inline unsigned norrath_read_taglo() {
    unsigned value;
    asm volatile("mfc0 %0, $28" : "=r"(value));
    return value;
}
#define NORRATH_TAGLO() norrath_read_taglo()
#endif

namespace {

// Sign-extended 32-bit value, as the 64-bit register holds it after a MIPS
// 32-bit operation.
inline long long sign_extend(unsigned value) {
    return static_cast<long long>(static_cast<int>(value));
}

// The tag test on the value that TagLo held after the index operation. Mask
// 0xfffff000 is applied to the sign-extended TagLo, the index is added with a
// 32-bit add, and the result is compared as an unsigned 64-bit value.
inline bool line_in_range(unsigned taglo, unsigned index, unsigned begin, unsigned end) {
    const unsigned long long masked = static_cast<unsigned long long>(sign_extend(taglo) & sign_extend(0xfffff000u));
    const unsigned long long address = static_cast<unsigned long long>(sign_extend(static_cast<unsigned>(masked) + index));
    return !(address < static_cast<unsigned long long>(sign_extend(begin)) ||
             address > static_cast<unsigned long long>(sign_extend(end)));
}

}  // namespace

extern "C" void _sceSDC(unsigned begin, unsigned end) {
    for (unsigned index = 0; index < 4096; index += 64) {
        NORRATH_SYNC();
        NORRATH_CACHE_OP(0x10, 0, index);
        NORRATH_SYNC();
        if (line_in_range(NORRATH_TAGLO(), index, begin, end)) {
            NORRATH_SYNC();
            NORRATH_CACHE_OP(0x14, 0, index);
            NORRATH_SYNC();
        }
        NORRATH_SYNC();
        NORRATH_CACHE_OP(0x10, 1, index);
        NORRATH_SYNC();
        if (line_in_range(NORRATH_TAGLO(), index, begin, end)) {
            NORRATH_SYNC();
            NORRATH_CACHE_OP(0x14, 1, index);
            NORRATH_SYNC();
        }
        NORRATH_SYNC();
    }
}
