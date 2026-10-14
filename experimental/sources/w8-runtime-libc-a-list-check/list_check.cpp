// Observed _listCheckAllIntegrity__FPCci at 0x00194158, 424 bytes. It walks the
// 6096 twenty-byte records of `nodes` (0x007ad000, 121920 bytes) in address
// order and checks four words of each. Words 0 and 8 (list links) must be zero
// or lie inside the table, from its first byte through the address just past its
// last record (inclusive). Words 4 and 12 (pointers) must be zero, or in
// 0x00100000..0x08000000, or in 0x70000000..0x70004000 (a 16 KiB window; inferred
// to be scratchpad, unconfirmed here). The check order per record is word 0, 4, 8, 12.
// Each failure calls scePrintf at 0x002d70d0 with the text "list integrity breach!
// %s:%d" and a newline, the file argument and the line argument, then stores zero
// to address 0, which faults on the target; if the store does not fault the walk
// continues. Word 16 of a record is not checked. Records are not otherwise
// modified.
// Unknown: what the fault handler does with the store; the record meaning.
extern "C" {
extern unsigned char nodes[6096 * 20];
int scePrintf(const char* format, ...);
}

#if !defined(__mips__)
extern "C" void norrath_list_check_host_fault_probe();
#endif

namespace {
const char kBreachText[] = "list integrity breach! %s:%d\n";

inline void fail(const char* file, int line) {
    scePrintf(kBreachText, file, line);
#if defined(__mips__)
    asm volatile("sw $zero,0($zero)" ::: "memory");
#else
    norrath_list_check_host_fault_probe();
#endif
}

inline bool link_bad(unsigned word, unsigned first, unsigned limit) {
    return word != 0u && (word < first || word > limit);
}

inline bool pointer_bad(unsigned word) {
    if (word == 0u) return false;
    if (word - 0x00100000u <= 0x07f00000u) return false;
    return !(word - 0x70000000u < 16385u);
}
}  // namespace

extern "C" void _listCheckAllIntegrity__FPCci(const char* file, int line) {
    const unsigned first = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(nodes));
    const unsigned limit = first + 6096u * 20u;
    const unsigned* record = reinterpret_cast<const unsigned*>(nodes);
    for (int index = 0; index < 6096; ++index, record += 5) {
        if (link_bad(record[0], first, limit)) fail(file, line);
        if (pointer_bad(record[1])) fail(file, line);
        if (link_bad(record[2], first, limit)) fail(file, line);
        if (pointer_bad(record[3])) fail(file, line);
    }
}
