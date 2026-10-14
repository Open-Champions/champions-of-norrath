// Observed: Next__Ct5VISet1ZP5VIWndi at 0x0120ac58 is 220 bytes. With (set, index) it reads the
// word at element+12 (elements are 20 bytes from the base at set+8). If that is nonzero it
// moves there and then follows the word at element+8 until it is zero, returning that index.
// Otherwise it starts from the word at element+4: if zero it returns zero; else it climbs,
// replacing the current index with the word at element+4 while the word at the new index's
// element+12 equals the index just left, and returns the first index that is not reached
// from its right link.
// Inferred: a successor step on a search tree stored in an array: +4 parent, +8 left, +12
// right, 0 the nil/end index; matches Begin's use of +8.
// Unknown: whether the original loops re-read the parent after a failed test (the order of
// loads here follows the instruction sequence, not a stated source).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
int Next__Ct5VISet1ZP5VIWndi(const void* set, int index) {
    const unsigned char* bytes = static_cast<const unsigned char*>(set);
    const unsigned char* elements = reinterpret_cast<const unsigned char*>(
        NORRATH_WORD_ADDRESS(*reinterpret_cast<const unsigned*>(bytes + 8)));
    int node = *reinterpret_cast<const int*>(elements + index * 20 + 12);
    if (node != 0) {
        for (;;) {
            const int left = *reinterpret_cast<const int*>(elements + node * 20 + 8);
            if (left == 0) return node;
            node = left;
        }
    }
    int parent = *reinterpret_cast<const int*>(elements + index * 20 + 4);
    if (parent == 0) return 0;
    node = index;
    for (;;) {
        if (*reinterpret_cast<const int*>(elements + parent * 20 + 12) != node) return parent;
        node = parent;
        parent = *reinterpret_cast<const int*>(elements + node * 20 + 4);
        if (parent == 0) return 0;
    }
}
}
