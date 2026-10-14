// Observed: Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo
// at 0x012205d0 is 1160 bytes. With (map, key, value) it searches the array-indexed tree
// whose element array base is the word at map+8 and whose root index is the word at map+12;
// elements are 104 bytes with +0 colour word, +4 parent, +8 left, +12 right, +16 the key word and
// +20 the value (84 bytes, copied as 21 words). It reads the key word once and walks from the root (a left link when the
// key is below the element key, a right link when above, signed compares); an equal key returns that
// element's index and changes nothing. Otherwise it calls AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(map, parent, side)
// with side 1 after a left step and 0 after a right step (parent 0 and side 0 when the map is
// empty); a negative result is returned as is. It stores the key at element+16 and copies the value
// to element+20, then, while the new index is not the root and its parent's colour word is nonzero,
// repairs the tree: when the parent is a left child of its parent g: if the uncle (right of g) has
// colour nonzero, parent and uncle get 0, g gets 1 and the walk moves to g; otherwise, when the
// current index is the parent's right child it moves to the parent and calls LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, parent);
// then the (new) parent gets 0, g gets 1 and RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, g) is called. The mirror image handles a
// parent that is a right child. At the end the root's colour word is set to 0 and the new index is
// returned.
// Inferred: red-black insertion (1 red, 0 black; AddNode makes the new element red); keys compare as signed integers, matching the signed compares in the image.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
int AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(void* map, int parent, int side);
int LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(void* map, int index);
int RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(void* map, int index);

static inline unsigned* field(const void* map, int index, int offset) {
    const unsigned char* bytes = static_cast<const unsigned char*>(map);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return reinterpret_cast<unsigned*>(elements + index * 104 + offset);
}
static inline int root_of(const void* map) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(map) + 12);
}

int Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(void* map, const unsigned* key, const unsigned* value) {
    int parent = 0;
    int side = 0;
    int node = root_of(map);
    if (node != 0) {
        const int wanted = static_cast<int>(*key);
        for (;;) {
            parent = node;
            const int found = static_cast<int>(*field(map, node, 16));
            side = 1;
            if (wanted < found) {
                node = static_cast<int>(*field(map, node, 8));
            } else if (found < wanted) {
                side = 0;
                node = static_cast<int>(*field(map, node, 12));
            } else {
                return parent;
            }
            if (node == 0) break;
        }
    }
    const int created = AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(map, parent, side);
    if (created < 0) return -1;
    *field(map, created, 16) = *key;
    for (unsigned i = 0; i < 21; ++i) *field(map, created, 20 + 4 * i) = value[i];
    int current = created;
    while (current != root_of(map) && *field(map, static_cast<int>(*field(map, current, 4)), 0) != 0) {
        int up = static_cast<int>(*field(map, current, 4));
        const int grand = static_cast<int>(*field(map, up, 4));
        if (up == static_cast<int>(*field(map, grand, 8))) {
            const int uncle = static_cast<int>(*field(map, grand, 12));
            if (*field(map, uncle, 0) != 0) {
                *field(map, up, 0) = 0;
                *field(map, uncle, 0) = 0;
                *field(map, grand, 0) = 1;
                current = grand;
            } else {
                if (current == static_cast<int>(*field(map, up, 12))) {
                    current = up;
                    LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, up);
                }
                up = static_cast<int>(*field(map, current, 4));
                const int top = static_cast<int>(*field(map, up, 4));
                *field(map, up, 0) = 0;
                *field(map, top, 0) = 1;
                RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, top);
            }
        } else {
            const int uncle = static_cast<int>(*field(map, grand, 8));
            if (*field(map, uncle, 0) != 0) {
                *field(map, up, 0) = 0;
                *field(map, uncle, 0) = 0;
                *field(map, grand, 0) = 1;
                current = grand;
            } else {
                if (current == static_cast<int>(*field(map, up, 8))) {
                    current = up;
                    RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, up);
                }
                up = static_cast<int>(*field(map, current, 4));
                const int top = static_cast<int>(*field(map, up, 4));
                *field(map, up, 0) = 0;
                *field(map, top, 0) = 1;
                LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, top);
            }
        }
    }
    *field(map, root_of(map), 0) = 0;
    return created;
}
}
