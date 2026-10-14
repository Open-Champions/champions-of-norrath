// Observed old_find_exception_handler at 0x002fa920, 208 bytes (a local routine of the C++ exception
// runtime), pinned SLUS_205.65. Arguments: a code address and an old-style table of 12-byte entries
// {start, end, handler} ended by a start word of 0xffffffff (layout in ../w15-libs-eh-a-common/eh.hpp). A null
// table returns 0. The table is scanned from the front in order (it need not be sorted). An entry applies when
// start <= address < end (unsigned compares). The first applying entry is kept as the best; a later applying entry
// replaces it only when its end is not above the best's end and its start is not below the best's start (it is
// nested inside or equal to it). An entry whose range does not apply stops the scan when a best exists and the
// address is below that entry's start. The result is the best entry's handler word, or 0 when none applied.
// Inferred: the routine picks the innermost enclosing region. The first word of an entry is read as unsigned.
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
unsigned old_find_exception_handler(unsigned pc, w15eh::OldHandlerEntry* table) {
    if (table == nullptr) return 0;
    int best = -1;
    for (int pos = 0; table[pos].start != 0xffffffffu; ++pos) {
        const unsigned start = table[pos].start;
        if (pc >= start && pc < table[pos].end) {
            if (best == -1 || (table[best].end >= table[pos].end && table[best].start <= start)) best = pos;
        } else if (best >= 0 && pc < start) {
            break;
        }
    }
    return best == -1 ? 0 : table[best].handler;
}
}
