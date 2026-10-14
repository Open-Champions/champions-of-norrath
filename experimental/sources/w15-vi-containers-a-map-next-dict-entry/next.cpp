// Observed: Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x0120a630, 220 bytes (a weak const member). Arguments: the map and a
// node index I. B is the word at map+8 and node J lives at B + 28 * J; a node's +4, +8 and +12 words are read as parent, left and
// right indices (the layout the Find and Erase routines of this map use). The result:
//  1. When I's right index R is nonzero: from R follow the left index while it is nonzero; the last node reached is the result.
//  2. Otherwise P = I's parent. When P is zero the result is zero. Else, while the right index of P equals the current child, the
//     child becomes P and P becomes the parent of that child; reaching a zero parent gives zero. The first P whose right index
//     differs from the child is the result.
// Nothing is stored and no function is called.
// Inferred: in-order successor in the index-based binary tree, with zero as the "no node" value that End returns.
// Unknown: the colour word at +0 and the key at +16 are not read here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static inline const unsigned* node(const unsigned* map, unsigned index) {
    return reinterpret_cast<const unsigned*>(at(map[2]) + 28 * index);
}

extern "C" {
int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void* map, int iterator) {
    const unsigned* m = static_cast<const unsigned*>(map);
    unsigned child = static_cast<unsigned>(iterator);
    unsigned right = node(m, child)[3];
    if (right != 0) {
        unsigned current = right;
        while (node(m, current)[2] != 0) current = node(m, current)[2];
        return static_cast<int>(current);
    }
    unsigned parent = node(m, child)[1];
    if (parent == 0) return 0;
    while (node(m, parent)[3] == child) {
        child = parent;
        parent = node(m, child)[1];
        if (parent == 0) return 0;
    }
    return static_cast<int>(parent);
}
}
