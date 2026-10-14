// Observed: Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo at 0x012205d0 (weak symbol, a template instantiation) is 1160
// bytes in the pinned SLUS-20565 image. Arguments: the map (this), a pointer to an int key and a pointer to an 84-byte cache-info value.
// Returns the entry index of the key: an existing entry with an equal key is returned untouched (the value is not replaced); otherwise a
// new entry is made and the value copied in; -1 is returned when AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii returns a negative index.
// Layout used (observed from the accesses): the map has its entry count at +0, capacity at +4, the entry array pointer at +8, the root
// entry index at +12 (0 = empty) and a free-entry list head at +16 (read by AddNode). Entries are 104 bytes: colour word at +0 (0 is
// the colour of entry 0, which is used as the nil entry, and of the root after an insert; the new entry made by AddNode is given 1), parent
// index at +4, left index at +8, right index at +12, the int key at +16 and the 84-byte value at +20.
// Flow: starting at the root, entries are compared by the signed int key: a smaller key goes left (word +8), a larger key right (word
// +12); equal returns that index. At an empty child, AddNode(map, parent index, 1 if it is the left child else 0) makes the entry and the
// key and value are stored in it (the key store and the value store are each guarded by a non-null test of the destination address,
// as an inlined placement construction does). Then the usual red-black re-balancing: while the entry is not the root and its parent's
// colour word is non-zero: if the parent is the grandparent's left child and the uncle (right child) has a non-zero colour, parent and
// uncle are set to 0 and the grandparent to 1 and the walk continues at the grandparent; otherwise, if the entry is the parent's right
// child it becomes its parent and LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi is called on it; then the (new) parent's colour is set to
// 0, the grandparent's to 1 and RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(grandparent) is called. The right-child side is the mirror
// image with the rotations swapped. Finally the root's colour word is cleared and the new index returned.
// Inferred: this is a red-black tree map keyed by int, 1 = red and 0 = black; the map is used as the surface cache's key to slot table.
// Unknown: the rest of the map and entry layouts (AddNode, Allocate, the rotations and Erase are other unauthored template members) and
// the exact meaning of the value words. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
struct EESurfCacheInfo {
    int words[21];
};
extern "C" {
int AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(unsigned char* map, int parent, int left);
void LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int index);
void RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int index);
static inline unsigned char* entry(unsigned char* map, int index) {
    return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(map + 8)) + index * 104;
}
static inline int& field(unsigned char* map, int index, int offset) {
    return *reinterpret_cast<int*>(entry(map, index) + offset);
}
int Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(unsigned char* map, const int* key, const EESurfCacheInfo* value) {
    int parent = 0;
    int left = 0;
    int current = *reinterpret_cast<int*>(map + 12);
    if (current != 0) {
        int wanted = *key;
        for (;;) {
            int found = field(map, current, 16);
            int next;
            if (wanted < found) {
                left = 1;
                next = field(map, current, 8);
            } else if (found < wanted) {
                left = 0;
                next = field(map, current, 12);
            } else {
                return current;
            }
            parent = current;
            if (next == 0) break;
            current = next;
        }
    }
    int created = AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii(map, parent, left);
    if (created < 0) return -1;
    int node = created;
    int* keySlot = &field(map, created, 16);
    if (keySlot) *keySlot = *key;
    EESurfCacheInfo* valueSlot = reinterpret_cast<EESurfCacheInfo*>(entry(map, created) + 20);
    if (valueSlot) *valueSlot = *value;
    while (node != *reinterpret_cast<int*>(map + 12) && field(map, field(map, node, 4), 0) != 0) {
        int parentIndex = field(map, node, 4);
        int grand = field(map, parentIndex, 4);
        if (parentIndex == field(map, grand, 8)) {
            int uncle = field(map, grand, 12);
            if (field(map, uncle, 0) != 0) {
                field(map, parentIndex, 0) = 0;
                field(map, uncle, 0) = 0;
                field(map, grand, 0) = 1;
                node = grand;
            } else {
                if (node == field(map, parentIndex, 12)) {
                    node = parentIndex;
                    LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, node);
                }
                int up = field(map, node, 4);
                int up2 = field(map, up, 4);
                field(map, up, 0) = 0;
                field(map, up2, 0) = 1;
                RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, up2);
            }
        } else {
            int uncle = field(map, grand, 8);
            if (field(map, uncle, 0) != 0) {
                field(map, parentIndex, 0) = 0;
                field(map, uncle, 0) = 0;
                field(map, grand, 0) = 1;
                node = grand;
            } else {
                if (node == field(map, parentIndex, 8)) {
                    node = parentIndex;
                    RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, node);
                }
                int up = field(map, node, 4);
                int up2 = field(map, up, 4);
                field(map, up, 0) = 0;
                field(map, up2, 0) = 1;
                LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi(map, up2);
            }
        }
    }
    field(map, *reinterpret_cast<int*>(map + 12), 0) = 0;
    return created;
}
}
