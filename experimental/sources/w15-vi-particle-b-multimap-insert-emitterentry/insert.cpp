// Observed: Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry at 0x011f3e48, 796 bytes
// (a weak template instantiation). Arguments: the map, a pointer to a float key and a pointer to an
// entry. Nodes are 112-byte records at the array whose address is the word at map+8; node words are
// named by byte offset: +0 a flag word, +4 parent, +8 left, +12 right (0 means none), +16 the float
// key, +32 and +36 two value words, +48 a VIMatrix44. Map word +12 is the root index.
//  1. Descent: starting at the root, the cursor goes to the left word when key < node key (side
//     1) and otherwise to the right word (side 0, so equal keys go right) until the word is 0. The
//     last node visited is the parent (0 for an empty map) and its side is kept.
//  2. node = AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii(map, parent, side); a negative result
//     returns -1 and nothing else happens.
//  3. The new node receives the key at +16, the entry words +0 and +4 at +32 and +36, and
//     __10VIMatrix44RC10VIMatrix44(node + 48, entry + 16).
//  4. Fix-up loop, while the node is not the root and the flag word of its parent is nonzero. With
//     p the parent and g the parent of p:
//       - p is g's left child: uncle = g's right child. Nonzero uncle flag: p and the uncle get
//         flag 0, g gets 1 and the node becomes g. Zero uncle flag: when the node is p's right
//         child, LeftRotate(map, p) and the node becomes p; then the parent of the node gets flag
//         0, its parent gets flag 1 and RightRotate(map, that grandparent) runs.
//       - otherwise it is the mirror image: uncle = g's left child, RightRotate when the node is
//         p's left child, final LeftRotate.
//  5. The flag word of the root becomes 0 and the new node index is returned.
// Inferred: the flag is a red/black colour (nonzero is red) and this is the standard red-black
// insertion with the null node 0 acting as a black sentinel. Unknown: which value the allocator
// stores in a new node's flag (AddNode owns it), the member names and the entry layout beyond the
// three parts used here.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* const norrath_host_arena_base;
static inline unsigned char* array_of(unsigned word) { return norrath_host_arena_base + word; }
#else
static inline unsigned char* array_of(unsigned word) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word)); }
#endif

extern "C" {
int AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii(void* map, int parent, int side);
void LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int index);
void RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map, int index);
void __10VIMatrix44RC10VIMatrix44(void* to, const void* from);

int Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(void* map, const float* key, const void* entry) {
    unsigned* words = static_cast<unsigned*>(map);
#define NORRATH_NODE_WORD(index, k) (*reinterpret_cast<unsigned*>(array_of(words[2]) + 112 * (index) + 4 * (k)))
    int parent = 0;
    int side = 0;
    unsigned cursor = words[3];
    if (cursor != 0) {
        float wanted = *key;
        for (;;) {
            parent = static_cast<int>(cursor);
            float other = *reinterpret_cast<float*>(array_of(words[2]) + 112 * parent + 16);
            if (wanted < other) {
                cursor = NORRATH_NODE_WORD(parent, 2);
                side = 1;
            } else {
                cursor = NORRATH_NODE_WORD(parent, 3);
                side = 0;
            }
            if (cursor == 0) break;
        }
    }
    int node = AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii(map, parent, side);
    if (node < 0) return -1;
    *reinterpret_cast<float*>(array_of(words[2]) + 112 * node + 16) = *key;
    const unsigned* from = static_cast<const unsigned*>(entry);
    NORRATH_NODE_WORD(node, 8) = from[0];
    NORRATH_NODE_WORD(node, 9) = from[1];
    __10VIMatrix44RC10VIMatrix44(array_of(words[2]) + 112 * node + 48, from + 4);
    unsigned s = static_cast<unsigned>(node);
    while (s != words[3] && NORRATH_NODE_WORD(NORRATH_NODE_WORD(s, 1), 0) != 0) {
        unsigned p = NORRATH_NODE_WORD(s, 1);
        unsigned g = NORRATH_NODE_WORD(p, 1);
        bool parent_is_left = p == NORRATH_NODE_WORD(g, 2);
        unsigned uncle = parent_is_left ? NORRATH_NODE_WORD(g, 3) : NORRATH_NODE_WORD(g, 2);
        if (NORRATH_NODE_WORD(uncle, 0) != 0) {
            NORRATH_NODE_WORD(p, 0) = 0;
            NORRATH_NODE_WORD(uncle, 0) = 0;
            NORRATH_NODE_WORD(NORRATH_NODE_WORD(p, 1), 0) = 1;
            s = NORRATH_NODE_WORD(p, 1);
        } else {
            if (parent_is_left) {
                if (s == NORRATH_NODE_WORD(p, 3)) {
                    LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, static_cast<int>(p));
                    s = p;
                }
            } else {
                if (s == NORRATH_NODE_WORD(p, 2)) {
                    RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, static_cast<int>(p));
                    s = p;
                }
            }
            unsigned top = NORRATH_NODE_WORD(s, 1);
            unsigned grand = NORRATH_NODE_WORD(top, 1);
            NORRATH_NODE_WORD(top, 0) = 0;
            NORRATH_NODE_WORD(grand, 0) = 1;
            if (parent_is_left) RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, static_cast<int>(NORRATH_NODE_WORD(top, 1)));
            else LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, static_cast<int>(NORRATH_NODE_WORD(top, 1)));
        }
    }
    NORRATH_NODE_WORD(words[3], 0) = 0;
    return node;
#undef NORRATH_NODE_WORD
}
}
