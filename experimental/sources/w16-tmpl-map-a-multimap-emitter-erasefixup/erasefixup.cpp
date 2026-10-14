// Observed: EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4608 is 1020 bytes. With
// (tree, x) it works on the array-indexed tree whose element array base is the word at tree+8
// and whose root index is the word at tree+12; elements are 112 bytes with +0 a colour word
// (0 or 1), +4 parent, +8 left link, +12 right link, and index 0 the nil element. While x is not
// the root and element x's colour is 0, it looks at the sibling w of x under x's parent p (the
// right link when x is p's left link, otherwise the left link). When w's colour is nonzero it sets
// w to 0 and p to 1, rotates p toward x (LeftRotate when x was the left child, RightRotate
// otherwise, both with the tree and p) and takes the new sibling from the re-read parent. When
// both of w's children have colour 0 it sets w's colour to 1 and moves x up to its parent.
// Otherwise, when the child of w on the far side from x has colour 0, it sets the near child to 0
// and w to 1, rotates w away from x and re-reads the sibling; then w takes p's colour, p becomes
// 0, the far child of w becomes 0, p is rotated toward x, and x becomes the root. After the
// loop x's colour is set to 0 (this also runs when x starts as the root). It returns 0.
// Inferred: the delete-repair step of a red-black tree (0 black, 1 red); every read goes through
// the current array base because the rotations are separate calls. Element 0 can be x and can
// receive stores. Unknown: template parameters beyond the mangled name.
extern "C" {
int LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* tree, int x);
int RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* tree, int x);

static inline unsigned* word(void* tree, unsigned offset) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(tree) + offset);
}
static inline unsigned* field(void* tree, unsigned index, unsigned offset) {
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*word(tree, 8)));
    return reinterpret_cast<unsigned*>(elements + index * 112 + offset);
}

int EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* tree, int index) {
    unsigned x = static_cast<unsigned>(index);
    while (x != *word(tree, 12) && *field(tree, x, 0) == 0) {
        unsigned w;
        if (*field(tree, *field(tree, x, 4), 8) == x) {
            w = *field(tree, *field(tree, x, 4), 12);
            if (*field(tree, w, 0) != 0) {
                *field(tree, w, 0) = 0;
                *field(tree, *field(tree, x, 4), 0) = 1;
                LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(*field(tree, x, 4)));
                w = *field(tree, *field(tree, x, 4), 12);
            }
            if (*field(tree, *field(tree, w, 8), 0) == 0 && *field(tree, *field(tree, w, 12), 0) == 0) {
                *field(tree, w, 0) = 1;
                x = *field(tree, x, 4);
                continue;
            }
            if (*field(tree, *field(tree, w, 12), 0) == 0) {
                *field(tree, *field(tree, w, 8), 0) = 0;
                *field(tree, w, 0) = 1;
                RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(w));
                w = *field(tree, *field(tree, x, 4), 12);
            }
            *field(tree, w, 0) = *field(tree, *field(tree, x, 4), 0);
            *field(tree, *field(tree, x, 4), 0) = 0;
            *field(tree, *field(tree, w, 12), 0) = 0;
            LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(*field(tree, x, 4)));
        } else {
            w = *field(tree, *field(tree, x, 4), 8);
            if (*field(tree, w, 0) != 0) {
                *field(tree, w, 0) = 0;
                *field(tree, *field(tree, x, 4), 0) = 1;
                RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(*field(tree, x, 4)));
                w = *field(tree, *field(tree, x, 4), 8);
            }
            if (*field(tree, *field(tree, w, 12), 0) == 0 && *field(tree, *field(tree, w, 8), 0) == 0) {
                *field(tree, w, 0) = 1;
                x = *field(tree, x, 4);
                continue;
            }
            if (*field(tree, *field(tree, w, 8), 0) == 0) {
                *field(tree, *field(tree, w, 12), 0) = 0;
                *field(tree, w, 0) = 1;
                LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(w));
                w = *field(tree, *field(tree, x, 4), 8);
            }
            *field(tree, w, 0) = *field(tree, *field(tree, x, 4), 0);
            *field(tree, *field(tree, x, 4), 0) = 0;
            *field(tree, *field(tree, w, 8), 0) = 0;
            RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(tree, static_cast<int>(*field(tree, x, 4)));
        }
        x = *word(tree, 12);
    }
    *field(tree, x, 0) = 0;
    return 0;
}
}
