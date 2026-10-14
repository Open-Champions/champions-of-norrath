// Observed sceSifWriteBackDCache at 0x002d7858, 176 bytes.
// A nonpositive byte count returns without a cache operation. Otherwise the
// first and last covered bytes are aligned down to 64, and every 64-byte line
// in that closed span is written back. The line count is split into a
// remainder of count & 7 and groups of eight. Each line is preceded by sync.
// The remainder loop also executes a nop after the second sync. Operation
// 0x18 is the writeback. The trailing unreachable return in the original has
// no predecessor and is not reproduced.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_sony_cd_record_cache_line(unsigned address);
static void write_back_line(unsigned address) {
    norrath_sony_cd_record_cache_line(address);
}
#else
static void sync_line() {
    asm volatile("sync" ::: "memory");
}
template <int Offset>
static void write_back_one(unsigned address) {
    sync_line();
    asm volatile("cache 0x18,%0(%1)" :: "i"(Offset), "r"(address) : "memory");
}
#endif
extern "C" void sceSifWriteBackDCache(void* address, int byte_count) {
    const unsigned first = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(address));
    const unsigned end = first + static_cast<unsigned>(byte_count);
    if (byte_count <= 0) return;
    unsigned cursor = first & 0xffffffc0u;
    const unsigned aligned_last = (end - 1u) & 0xffffffc0u;
    const unsigned line_count = ((aligned_last - cursor) >> 6) + 1u;
    unsigned remainder = line_count & 7u;
    unsigned blocks = line_count >> 3;
    if (remainder != 0u) {
        do {
#if defined(NORRATH_HOST_CONTRACT)
            write_back_line(cursor);
#else
            write_back_one<0>(cursor);
            sync_line();
            asm volatile("nop" ::: "memory");
#endif
            remainder -= 1u;
            cursor += 64u;
        } while (remainder != 0u);
    }
    if (blocks != 0u) {
        do {
            blocks -= 1u;
#if defined(NORRATH_HOST_CONTRACT)
            write_back_line(cursor);
            write_back_line(cursor + 64u);
            write_back_line(cursor + 128u);
            write_back_line(cursor + 192u);
            write_back_line(cursor + 256u);
            write_back_line(cursor + 320u);
            write_back_line(cursor + 384u);
            write_back_line(cursor + 448u);
#else
            write_back_one<0>(cursor);
            write_back_one<64>(cursor);
            write_back_one<128>(cursor);
            write_back_one<192>(cursor);
            write_back_one<256>(cursor);
            write_back_one<320>(cursor);
            write_back_one<384>(cursor);
            write_back_one<448>(cursor);
            sync_line();
#endif
            cursor += 512u;
        } while (blocks != 0u);
    }
}
