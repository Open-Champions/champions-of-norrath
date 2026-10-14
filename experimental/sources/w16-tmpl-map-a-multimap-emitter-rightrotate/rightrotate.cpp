// Observed: RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4538 is 208 bytes. With (tree, x) it rotates the node at index x
// right in the array-indexed tree whose element array base is the word at tree+8 and whose
// root index is the word at tree+12; elements are 112 bytes with +4 the parent index,
// +8 the left link and +12 the right link, and 0 the nil index. Let y be the left link of x
// (word at +8). It stores y's right link (word at +12) into x's left link and, when that
// is nonzero, makes x its parent. It copies x's parent into y's parent; when that is zero the
// root word becomes y, otherwise the parent's link that equals x (tested on its left link,
// the right link taking the other case) is set to y. It then sets y's right link to x and
// x's parent to y, and returns 0. The colour word at +0 is not touched.
// Inferred: the rotation step of a red-black tree; the zero return is the compiler's result
// for a void-like routine and carries no meaning. Unknown: the template parameters beyond the
// mangled name, and the layout of the element past the link words.
extern "C" {
static inline unsigned* field(const void* tree, unsigned index, unsigned offset) {
    const unsigned char* bytes = static_cast<const unsigned char*>(tree);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return reinterpret_cast<unsigned*>(elements + index * 112 + offset);
}

int RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* tree, int x) {
    const unsigned node = static_cast<unsigned>(x);
    const unsigned y = *field(tree, node, 8);
    const unsigned moved = *field(tree, y, 12);
    *field(tree, node, 8) = moved;
    if (moved != 0) *field(tree, moved, 4) = node;
    const unsigned parent = *field(tree, node, 4);
    *field(tree, y, 4) = parent;
    if (parent == 0) {
        *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(tree) + 12) = y;
    } else if (*field(tree, parent, 8) == node) {
        *field(tree, parent, 8) = y;
    } else {
        *field(tree, parent, 12) = y;
    }
    *field(tree, y, 12) = node;
    *field(tree, node, 4) = y;
    return 0;
}
}
