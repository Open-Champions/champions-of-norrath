// Observed: Erase__t10VIMultiMap2ZUiZii at 0x01220fa8 is 512 bytes. With (tree, index) it first calls
// Next__Ct10VIMultiMap2ZUiZii(tree, index) and returns that result at the end. The tree keeps its root index
// at tree+12 and 24-byte elements at the base in tree+8: +0 colour word, +4 parent, +8 left,
// +12 right; index 0 is the nil element. Let y be the index itself, except that y is the Next
// result when both links of the index are nonzero. Let x be y's left link, or its right link when
// the left is zero. It stores y's parent into x's parent word (also when x is 0), points y's
// parent (or the root word when y has no parent) at x in place of y, and, when y's colour word is
// 0, calls EraseFixup__t10VIMultiMap2ZUiZii(tree, x). When y differs from the index it then
// moves y into the index's place: y takes the index's parent, left, right and colour words, the
// index's parent (or the root word) now points at y, and the parents of the index's left and
// right elements become y (stores to element 0 included). Finally it calls
// FreeNode__t10VIMultiMap2ZUiZii(tree, index).
// Inferred: standard red-black tree deletion with 0 meaning black; keys and values stay with their
// elements, as the window set's Erase does. Unknown: template parameters beyond the mangled name.
extern "C" {
int Next__Ct10VIMultiMap2ZUiZii(const void* tree, int index);
void EraseFixup__t10VIMultiMap2ZUiZii(void* tree, int index);
void FreeNode__t10VIMultiMap2ZUiZii(void* tree, int index);

static inline unsigned* field(const void* tree, int index, int offset) {
    const unsigned char* bytes = static_cast<const unsigned char*>(tree);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return reinterpret_cast<unsigned*>(elements + index * 24 + offset);
}
static inline unsigned* root_word(void* tree) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(tree) + 12);
}
// Points the parent's link that currently leads to `from` at `to`, or the root word.
static inline void relink(void* tree, int parent, int from, int to) {
    if (parent == 0) {
        *root_word(tree) = static_cast<unsigned>(to);
    } else if (static_cast<int>(*field(tree, parent, 8)) == from) {
        *field(tree, parent, 8) = static_cast<unsigned>(to);
    } else {
        *field(tree, parent, 12) = static_cast<unsigned>(to);
    }
}

int Erase__t10VIMultiMap2ZUiZii(void* tree, int index) {
    const int following = Next__Ct10VIMultiMap2ZUiZii(tree, index);
    int removed = index;
    if (*field(tree, index, 8) != 0 && *field(tree, index, 12) != 0) removed = following;
    int child = static_cast<int>(*field(tree, removed, 8));
    if (child == 0) child = static_cast<int>(*field(tree, removed, 12));
    *field(tree, child, 4) = *field(tree, removed, 4);
    relink(tree, static_cast<int>(*field(tree, removed, 4)), removed, child);
    if (*field(tree, removed, 0) == 0) EraseFixup__t10VIMultiMap2ZUiZii(tree, child);
    if (removed != index) {
        *field(tree, removed, 4) = *field(tree, index, 4);
        *field(tree, removed, 8) = *field(tree, index, 8);
        *field(tree, removed, 12) = *field(tree, index, 12);
        *field(tree, removed, 0) = *field(tree, index, 0);
        relink(tree, static_cast<int>(*field(tree, index, 4)), index, removed);
        *field(tree, static_cast<int>(*field(tree, index, 8)), 4) = static_cast<unsigned>(removed);
        *field(tree, static_cast<int>(*field(tree, index, 12)), 4) = static_cast<unsigned>(removed);
    }
    FreeNode__t10VIMultiMap2ZUiZii(tree, index);
    return following;
}
}
