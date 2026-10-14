// Observed: Erase__t5VISet1ZP5VIWndi at 0x0120b028 is 512 bytes. With (set, index) it first calls
// Next__Ct5VISet1ZP5VIWndi(set, index) and returns that result at the end. The set keeps a root
// index at set+12 and 20-byte elements at the base in set+8: +0 colour word, +4 parent, +8 left,
// +12 right; index 0 is the nil element. Let y be the index itself, except that y is the Next
// result when both links of the index are nonzero. Let x be y's left link, or its right link when
// the left is zero. It stores y's parent into x's parent word (also when x is 0), points y's
// parent (or the root word when y has no parent) at x in place of y, and, when y's colour word is
// 0, calls EraseFixup__t5VISet1ZP5VIWndi(set, x). When y differs from the index it then moves y into the
// index's place: y takes the index's parent, left, right and colour words, the index's parent (or
// the root word) now points at y, and the parents of the index's left and right elements become
// y (stores to element 0 included). Finally it calls FreeNode__t5VISet1ZP5VIWndi(set, index).
// Inferred: standard red-black tree deletion with 0 meaning black; keys stay with their elements.
// Unknown: what EraseFixup and FreeNode do beyond their calls; template parameters beyond the name.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
int Next__Ct5VISet1ZP5VIWndi(const void* set, int index);
void EraseFixup__t5VISet1ZP5VIWndi(void* set, int index);
void FreeNode__t5VISet1ZP5VIWndi(void* set, int index);

static inline unsigned* field(const void* set, int index, int offset) {
    const unsigned char* bytes = static_cast<const unsigned char*>(set);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        NORRATH_WORD_ADDRESS(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return reinterpret_cast<unsigned*>(elements + index * 20 + offset);
}
static inline unsigned* root_word(void* set) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(set) + 12);
}
// Points the parent's link that currently leads to `from` at `to`, or the root word.
static inline void relink(void* set, int parent, int from, int to) {
    if (parent == 0) {
        *root_word(set) = static_cast<unsigned>(to);
    } else if (static_cast<int>(*field(set, parent, 8)) == from) {
        *field(set, parent, 8) = static_cast<unsigned>(to);
    } else {
        *field(set, parent, 12) = static_cast<unsigned>(to);
    }
}

int Erase__t5VISet1ZP5VIWndi(void* set, int index) {
    const int following = Next__Ct5VISet1ZP5VIWndi(set, index);
    int removed = index;
    if (*field(set, index, 8) != 0 && *field(set, index, 12) != 0) removed = following;
    int child = static_cast<int>(*field(set, removed, 8));
    if (child == 0) child = static_cast<int>(*field(set, removed, 12));
    *field(set, child, 4) = *field(set, removed, 4);
    relink(set, static_cast<int>(*field(set, removed, 4)), removed, child);
    if (*field(set, removed, 0) == 0) EraseFixup__t5VISet1ZP5VIWndi(set, child);
    if (removed != index) {
        *field(set, removed, 4) = *field(set, index, 4);
        *field(set, removed, 8) = *field(set, index, 8);
        *field(set, removed, 12) = *field(set, index, 12);
        *field(set, removed, 0) = *field(set, index, 0);
        relink(set, static_cast<int>(*field(set, index, 4)), index, removed);
        *field(set, static_cast<int>(*field(set, index, 8)), 4) = static_cast<unsigned>(removed);
        *field(set, static_cast<int>(*field(set, index, 12)), 4) = static_cast<unsigned>(removed);
    }
    FreeNode__t5VISet1ZP5VIWndi(set, index);
    return following;
}
}
