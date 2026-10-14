// Observed: Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x01209b80, 512 bytes. Arguments: the map and a node index Z. Nodes
// are 28 bytes at B + 28 * index where B is the word at map+8; node words are named here by byte offset (+0 colour-like word,
// +4 parent, +8 left, +12 right); index 0 is "none" but node 0 is a real record that gets written (x's parent below). The word at
// map+12 is the root index.
//  1. S = Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, Z) (the result of the whole call).
//  2. Y = Z when Z.left or Z.right is zero, else S. X = Y.left, or Y.right when Y.left is zero.
//  3. X.parent = Y.parent (even when X is 0). If Y.parent is zero the root word becomes X; otherwise the parent's left link becomes
//     X when it equals Y, else its right link becomes X.
//  4. When Y's +0 word is zero, EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, X) is called.
//  5. When Y != Z, Y takes over Z's place: Y's +4, +8, +12 and +0 words are copied from Z; Z's parent (or the root word when Z's
//     parent is zero) links to Y in the same left-if-equal-else-right way; the parents of Z's left and right children are set to Y
//     (even for index 0).
//  6. FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, Z) is called and S is returned.
// Inferred: the textbook deletion for a red-black tree stored in an index array (the +0 word read as the colour, with fixup run when
// the removed node's word is zero); the node array base is read again after each call or store group as in the image.
// Unknown: the colour encoding, the meaning of the data words past +16, and the exact callee results (ignored here).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static inline unsigned* node(const unsigned* map, unsigned index) {
    return reinterpret_cast<unsigned*>(at(map[2]) + 28 * index);
}

extern "C" {
int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void* map, int iterator);
int EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator);
int FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator);

int Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int z) {
    unsigned* m = static_cast<unsigned*>(map);
    unsigned zi = static_cast<unsigned>(z);
    int successor = Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, z);
    unsigned y = zi;
    if (node(m, zi)[2] != 0 && node(m, zi)[3] != 0) y = static_cast<unsigned>(successor);
    unsigned x = node(m, y)[2];
    if (x == 0) x = node(m, y)[3];
    node(m, x)[1] = node(m, y)[1];
    unsigned parent = node(m, y)[1];
    if (parent == 0) {
        m[3] = x;
    } else if (y == node(m, parent)[2]) {
        node(m, parent)[2] = x;
    } else {
        node(m, parent)[3] = x;
    }
    if (node(m, y)[0] == 0) EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, static_cast<int>(x));
    if (y != zi) {
        node(m, y)[1] = node(m, zi)[1];
        node(m, y)[2] = node(m, zi)[2];
        node(m, y)[3] = node(m, zi)[3];
        node(m, y)[0] = node(m, zi)[0];
        unsigned zparent = node(m, zi)[1];
        if (zparent == 0) {
            m[3] = y;
        } else if (node(m, zparent)[2] == zi) {
            node(m, zparent)[2] = y;
        } else {
            node(m, zparent)[3] = y;
        }
        node(m, node(m, zi)[2])[1] = y;
        node(m, node(m, zi)[3])[1] = y;
    }
    FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, z);
    return successor;
}
}
