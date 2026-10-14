// Observed: Begin__Ct5VIMap2ZiZi at 0x012261f0 is 72 bytes. The map holds a root index at
// this+12 and an element array base at this+8; elements are 24 bytes and the word at element+8
// is a link index. Starting from the root, while the root is nonzero it follows the link at
// element+8 until that link is zero, and returns the last nonzero index (0 for an empty map).
// Inferred: element+8 is the left child of a search tree whose nodes are array indices with 0
// as nil, so this returns the lowest-key element (same shape as the window set's Begin, whose
// elements are 20 bytes). Unknown: the template parameters beyond the mangled name.
extern "C" {
int Begin__Ct5VIMap2ZiZi(const void* map) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    int index = *reinterpret_cast<const int*>(bytes + 12);
    if (index != 0) {
        const unsigned char* elements = reinterpret_cast<const unsigned char*>(
            static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
        for (;;) {
            const int left = *reinterpret_cast<const int*>(elements + index * 24 + 8);
            if (left == 0) break;
            index = left;
        }
    }
    return index;
}
}
