// Observed: Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x011f4168, 512 bytes (a weak template
// instantiation). Arguments: the map and a node index z. Nodes are 112-byte records at the array
// whose address is the word at map+8 (re-read wherever the image re-reads it); node words are named
// by byte offset: +0 a flag word, +4 parent, +8 left, +12 right; index 0 means none and is a real
// node that receives some stores. Map word +12 is the root index.
//  1. next = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, z) is kept as the result.
//  2. y is next when z has both a left and a right child, otherwise z.
//  3. x is the left child of y when that is nonzero, otherwise the right child of y.
//  4. x's parent word receives y's parent (even when x is 0). When y's parent is 0 the root word
//     receives x; otherwise the parent's left word receives x if it was y, else its right word does.
//  5. When the flag word of y is zero, EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, x) runs.
//  6. When y differs from z, y takes over z's place: its words +4, +8, +12 and +0 receive z's, the
//     parent of z (or the root word when that is 0) is pointed at y on the side where z was, and the
//     parent words of z's left and right children (index 0 included) receive y.
//  7. FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, z) runs and next is returned.
// Inferred: standard unlink of a node from an index-linked binary tree with a red-black style
// fixup after removing a node whose flag is zero (probably black). Unknown: which flag value is
// which colour, the key and the other node fields, and the member names.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* const norrath_host_arena_base;
static inline unsigned char* array_of(unsigned word) { return norrath_host_arena_base + word; }
#else
static inline unsigned char* array_of(unsigned word) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word)); }
#endif

extern "C" {
int Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(const void* map, int index);
void EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int index);
void FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int index);

int Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int z) {
    unsigned* words = static_cast<unsigned*>(map);
#define NORRATH_NODE_WORD(index, k) (*reinterpret_cast<unsigned*>(array_of(words[2]) + 112 * (index) + 4 * (k)))
    int next = Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi(map, z);
    int y = z;
    if (NORRATH_NODE_WORD(z, 2) != 0 && NORRATH_NODE_WORD(z, 3) != 0) y = next;
    unsigned x = NORRATH_NODE_WORD(y, 2) != 0 ? NORRATH_NODE_WORD(y, 2) : NORRATH_NODE_WORD(y, 3);
    NORRATH_NODE_WORD(x, 1) = NORRATH_NODE_WORD(y, 1);
    unsigned parent = NORRATH_NODE_WORD(y, 1);
    if (parent == 0) words[3] = x;
    else if (NORRATH_NODE_WORD(parent, 2) == static_cast<unsigned>(y)) NORRATH_NODE_WORD(parent, 2) = x;
    else NORRATH_NODE_WORD(parent, 3) = x;
    if (NORRATH_NODE_WORD(y, 0) == 0) EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, x);
    if (y != z) {
        NORRATH_NODE_WORD(y, 1) = NORRATH_NODE_WORD(z, 1);
        NORRATH_NODE_WORD(y, 2) = NORRATH_NODE_WORD(z, 2);
        NORRATH_NODE_WORD(y, 3) = NORRATH_NODE_WORD(z, 3);
        NORRATH_NODE_WORD(y, 0) = NORRATH_NODE_WORD(z, 0);
        unsigned above = NORRATH_NODE_WORD(z, 1);
        if (above == 0) words[3] = y;
        else if (NORRATH_NODE_WORD(above, 2) == static_cast<unsigned>(z)) NORRATH_NODE_WORD(above, 2) = y;
        else NORRATH_NODE_WORD(above, 3) = y;
        NORRATH_NODE_WORD(NORRATH_NODE_WORD(z, 2), 1) = y;
        NORRATH_NODE_WORD(NORRATH_NODE_WORD(z, 3), 1) = y;
    }
    FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, z);
    return next;
#undef NORRATH_NODE_WORD
}
}
