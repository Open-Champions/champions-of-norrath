// Observed find_exception_handler at 0x002fa9f0, 316 bytes (a local routine of the C++ exception runtime),
// pinned SLUS_205.65. Arguments: a code address (or, on a rethrow, a pointer to the entry that was used), a table,
// an info record, a rethrow flag and a pointer to an int result flag. Table layout: an 8-byte header, then 16-byte
// entries {start, end, handler, match info} ended by a start word of 0xffffffff (../w15-libs-eh-a-common/eh.hpp).
//   * The flag is set to 1 first; a null table returns 0.
//   * Without rethrow the address is lowered by one and the scan starts at entry 0. With rethrow, the first
//     argument is an entry pointer: its index is (pointer - first entry) >> 4 (arithmetic), the address becomes
//     that entry's end minus one, a terminator entry at that index returns 0, and the scan resumes at the next entry.
//   * An entry applies when start <= address < end (unsigned). With match info zero, the first applying entry's
//     handler becomes the result (later ones do not replace it). With match info set, the info record's first
//     word is a matcher function: when it is zero the entry is skipped, otherwise it is called as
//     matcher(info, match info, table); a nonzero answer makes the entry's handler the result unless one is
//     already held, sets the flag to 0 and ends the scan.
//   * The result is the held handler word, or 0.
// Inferred: the flag says "only a cleanup was found" (1) or "a matching catch was found" (0); the first word of the
// info record is a type-match callback. Names are mine.
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
unsigned find_exception_handler(unsigned pc, w15eh::HandlerTable* table, unsigned char* info, int rethrow, int* only_cleanup) {
    unsigned result = 0;
    *only_cleanup = 1;
    if (table == nullptr) return 0;
    w15eh::HandlerEntry* const entries = table->entries;
    int pos = 0;
    if (rethrow != 0) {
        const unsigned entry_pointer = pc;
        pos = static_cast<int>(entry_pointer - w15eh::to_word(entries)) >> 4;
        pc = *reinterpret_cast<unsigned*>(w15eh::from_word(entry_pointer) + 4) - 1u;
        if (entries[pos].start == 0xffffffffu) return result;
        pos += 1;
    } else {
        pc -= 1u;
    }
    for (; entries[pos].start != 0xffffffffu; ++pos) {
        w15eh::HandlerEntry* const entry = &entries[pos];
        if (pc >= entry->start && pc < entry->end) {
            const unsigned match = entry->match_info;
            if (match == 0) {
                if (result == 0) result = entry->handler;
            } else {
                const unsigned matcher = *reinterpret_cast<unsigned*>(info);
                if (matcher != 0 && w15eh::call_matcher(matcher, info, match, table) != 0) {
                    if (result == 0) result = entry->handler;
                    *only_cleanup = 0;
                    break;
                }
            }
        }
    }
    return result;
}
}
