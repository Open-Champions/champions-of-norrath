#include "../level-anim-load-common/mem.hpp"
// Observed lumpFind__FPvPc at 0x00166580, 204 bytes. The lump starts with an unsigned
// halfword record count, followed by 12-byte records whose first word points at the
// record's name. The search is a binary search over the records, comparing the key
// against the name with strcmp and returning the address of the matching record, or null
// when the bounds meet without a match. The middle index is the arithmetic shift of
// low + high; an index below zero compares as greater and one past the initial high
// bound compares as less, without reading memory. Lowering uses middle - 1 for a
// relation of zero or below, raising uses middle + 1.
// Inferred: the records are sorted by name in strcmp order, which the search relies on.
// The callee is the published strcmp stand-in norrath_string_compare, as in the other
// loader sources; only the sign of its result is used.
// Unknown: the meaning of the other two words of each record (lumpFindResource reads
// the one at +4).
extern "C" int norrath_string_compare(const char* left, const char* right);
extern "C" unsigned char* lumpFind__FPvPc(unsigned char* lump, char* key) {
    using namespace norrath_lal;
    unsigned char* const records = lump + 4;
    const int last = static_cast<int>(load_half(lump)) - 1;
    int low = 0;
    int high = last;
    for (;;) {
        const int middle = (low + high) >> 1;
        int relation;
        if (middle < 0) {
            relation = 1;
        } else if (last < middle) {
            relation = -1;
        } else {
            const char* name = reinterpret_cast<const char*>(load_pointer(records + 12 * middle));
            relation = norrath_string_compare(key, name);
        }
        if (relation == 0) return records + 12 * middle;
        if (!(low < high)) return nullptr;
        if (relation <= 0) high = middle - 1;
        else low = middle + 1;
    }
}
