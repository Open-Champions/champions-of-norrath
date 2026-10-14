// Observed: LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x01209e80, 204 bytes. Arguments: the map and a node index X.
// Nodes are 28-byte records at B + 28 * index, B being the word at map+8; +4, +8 and +12 are read as parent, left and right (the
// layout Find, Next, Erase and EraseFixup use); the word at map+12 is the root index. With Y = right(X):
//  1. right(X) = left(Y). When left(Y) is nonzero its parent becomes X, and B is re-read afterwards.
//  2. parent(Y) = parent(X). B is re-read. P = parent(X); when P is zero the root word becomes Y; otherwise right(P) = Y when X
//     differs from left(P), else left(P) = Y followed by a re-read of B.
//  3. left(Y) = X. B is re-read, then parent(X) = Y. The result is 0.
// Inferred: the textbook left rotation of a red-black tree held in an index array; index 0 is "none" and is written when a link
// slot of it is assigned (as the image does, nothing guards Y being zero).
// Unknown: the colour word and key are not touched here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static inline unsigned* node(unsigned char* base, unsigned index) { return reinterpret_cast<unsigned*>(base + 28 * index); }

extern "C" {
int LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator) {
    unsigned* m = static_cast<unsigned*>(map);
    unsigned x = static_cast<unsigned>(iterator);
    unsigned char* base = at(m[2]);
    unsigned y = node(base, x)[3];
    node(base, x)[3] = node(base, y)[2];
    if (node(base, y)[2] != 0) {
        node(base, node(base, y)[2])[1] = x;
        base = at(m[2]);
    }
    node(base, y)[1] = node(base, x)[1];
    base = at(m[2]);
    unsigned parent = node(base, x)[1];
    if (parent == 0) {
        m[3] = y;
    } else if (x != node(base, parent)[2]) {
        node(base, parent)[3] = y;
    } else {
        node(base, parent)[2] = y;
        base = at(m[2]);
    }
    node(base, y)[2] = x;
    node(at(m[2]), x)[1] = y;
    return 0;
}
}
