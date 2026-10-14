// Observed: Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4e40, 220 bytes (a weak
// template instantiation). With the map as this and an entry index: the entry table is
// the pointer at this + 8 and entries are 112 bytes. Each entry has three index words at
// +4, +8 and +12, and index zero means none. Let P, L and R be those words at +4, +8
// and +12. If R of the entry is non-zero the result is the end of the L chain starting at
// R (R itself when its L is zero). Otherwise, with p = P of the entry: if p is zero the
// result is zero; if R of p is the entry, walk up (entry = p, p = P of p) until R of p is
// not the entry or p is zero; the result is that p (zero if the walk ran out). Nothing is
// written.
// Inferred: this is the in-order successor step on an index-linked binary search tree
// (P parent, L left child, R right child), with entry zero as the null node.
// Unknown: the member names and the entry fields beyond the three index words.
extern "C" {
int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void* map, int index) {
    const unsigned char* table =
        *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(map) + 8);
    #define NORRATH_TREE_WORD(node, off) (*reinterpret_cast<const int*>(table + (node) * 112 + (off)))
    int right = NORRATH_TREE_WORD(index, 12);
    if (right != 0) {
        int node = right;
        while (NORRATH_TREE_WORD(node, 8) != 0) node = NORRATH_TREE_WORD(node, 8);
        return node;
    }
    int parent = NORRATH_TREE_WORD(index, 4);
    int child = index;
    while (parent != 0 && NORRATH_TREE_WORD(parent, 12) == child) {
        child = parent;
        parent = NORRATH_TREE_WORD(child, 4);
    }
    return parent;
    #undef NORRATH_TREE_WORD
}
}
