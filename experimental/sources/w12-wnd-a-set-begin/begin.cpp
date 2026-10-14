// Observed: Begin__Ct5VISet1ZP5VIWnd at 0x0120ac08 is 72 bytes. The set holds a root index at
// this+12 and an element array base at this+8; elements are 20 bytes and the word at element+8
// is a link index. Starting from the root, while the root is nonzero it follows the link at
// element+8 until that link is zero, and returns the last nonzero index (0 for an empty set).
// Inferred: element+8 is the left child of a binary search tree whose nodes are array indices
// with 0 as the nil node, so this returns the first (lowest-key) element, consistent with the
// key word at element+16 used by the find routine.
// Unknown: the template parameters beyond the mangled name.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
int Begin__Ct5VISet1ZP5VIWnd(const void* set) {
    const unsigned char* bytes = static_cast<const unsigned char*>(set);
    int index = *reinterpret_cast<const int*>(bytes + 12);
    if (index != 0) {
        const unsigned char* elements = reinterpret_cast<const unsigned char*>(
            NORRATH_WORD_ADDRESS(*reinterpret_cast<const unsigned*>(bytes + 8)));
        for (;;) {
            const int left = *reinterpret_cast<const int*>(elements + index * 20 + 8);
            if (left == 0) break;
            index = left;
        }
    }
    return index;
}
}
