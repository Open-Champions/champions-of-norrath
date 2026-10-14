// Observed: DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry at 0x011f4b80. The routine runs to 0x011f4d10 (the image's symbol size
// of 132 bytes is too small; about 0x170 bytes of code follow the entry). Offsets are bytes from the map; nodes are 112-byte records
// at B + 112 * index where B is the word at map+8; node words +4 parent, +8 left, +12 right; index 0 is "none"; the word at map+12
// is the root index.
//  1. C is the root; when C is nonzero it is replaced by the leftmost node under it (follow +8 while nonzero).
//  2. Loop: while C differs from End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map): compute the in-order successor Nx of C (when C.right is
//     nonzero, the leftmost node under it; otherwise climb through parents while C is a right child, giving the parent of the first
//     left-child step, or 0 when the parent link reaches zero), then call _$_10VIMatrix44(B + 112 * C + 48, 2) and set C = Nx.
//  3. Return 0.
// Inferred: an in-order walk that runs the destructor of a VIMatrix44 member stored 48 bytes into every node (flag 2: destroy without
// freeing); the successor is computed before the destruction, as the image does. End is the zero cursor, so the walk ends after
// the last node or immediately for an empty tree.
// Unknown: the other node fields (the key and the rest of the entry), and who frees the node array.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static inline unsigned* node(const unsigned* map, unsigned index) {
    return reinterpret_cast<unsigned*>(at(map[2]) + 112 * index);
}

extern "C" {
int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map);
void _$_10VIMatrix44(void* matrix, int flags);

int DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map) {
    unsigned* m = static_cast<unsigned*>(map);
    unsigned current = m[3];
    if (current != 0) {
        while (node(m, current)[2] != 0) current = node(m, current)[2];
    }
    while (current != static_cast<unsigned>(End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map))) {
        unsigned next;
        unsigned right = node(m, current)[3];
        if (right != 0) {
            next = right;
            while (node(m, next)[2] != 0) next = node(m, next)[2];
        } else {
            unsigned parent = node(m, current)[1];
            if (parent == 0) {
                next = 0;
            } else if (node(m, parent)[3] != current) {
                next = parent;
            } else {
                unsigned child = parent;
                for (;;) {
                    unsigned above = node(m, child)[1];
                    if (above == 0) {
                        next = 0;
                        break;
                    }
                    unsigned previous = child;
                    child = above;
                    if (node(m, above)[3] != previous) {
                        next = above;
                        break;
                    }
                }
            }
        }
        unsigned char* matrix = at(m[2]) + 112 * current + 48;
        _$_10VIMatrix44(matrix, 2);
        current = next;
    }
    return 0;
}
}
