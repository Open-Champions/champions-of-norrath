// Observed: Next__Ct5VIMap2ZiZii at 0x01226238 is 220 bytes. With (map, index) it reads the word
// at element+12 (elements are 24 bytes from the base at map+8). If that is nonzero it moves
// there and follows the word at element+8 until it is zero, returning that index. Otherwise it
// starts from the word at element+4: zero returns zero; else it climbs, replacing the current
// index with the word at element+4 while the word at the parent's element+12 equals the index
// just left, and returns the first parent that is not reached from its right link (or zero).
// Inferred: in-order successor on an array-indexed search tree: +4 parent, +8 left, +12 right,
// 0 the nil/end index. Unknown: the template parameters beyond the mangled name.
extern "C" {
int Next__Ct5VIMap2ZiZii(const void* map, int index) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    const unsigned char* elements = reinterpret_cast<const unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    int node = *reinterpret_cast<const int*>(elements + index * 24 + 12);
    if (node != 0) {
        for (;;) {
            const int left = *reinterpret_cast<const int*>(elements + node * 24 + 8);
            if (left == 0) return node;
            node = left;
        }
    }
    int parent = *reinterpret_cast<const int*>(elements + index * 24 + 4);
    if (parent == 0) return 0;
    node = index;
    for (;;) {
        if (*reinterpret_cast<const int*>(elements + parent * 24 + 12) != node) return parent;
        node = parent;
        parent = *reinterpret_cast<const int*>(elements + node * 24 + 4);
        if (parent == 0) return 0;
    }
}
}
