// Observed: Insert__t5VISet1ZP5VIWndRCP5VIWnd at 0x0120ad38 is 748 bytes. The set keeps a root
// index at set+12 and a 20-byte element array at the base in set+8: +0 colour word, +4 parent,
// +8 left, +12 right, +16 key; index 0 is the nil element. Insert reads the key word *key and
// walks from the root (a left link when key is below the element key, a right link when above,
// unsigned compares); an equal key returns that element's index and changes nothing. Otherwise
// it calls AddNode__t5VISet1ZP5VIWndii(set, parent, side) with side 1 after a left step and 0
// after a right step (parent 0 and side 0 when the set is empty); a negative result is returned
// as is. It stores *key at element+16 of the new index, then, while the new index is not the
// root and its parent's colour word is nonzero, it repairs the tree: parent a left child of its
// parent g: if the uncle (right of g) has colour nonzero, parent and uncle get 0, g gets 1 and
// the walk moves to g; otherwise, when the current index is the parent's right child, it moves
// to the parent and calls LeftRotate__t5VISet1ZP5VIWndi(set, parent); then parent gets 0, g gets
// 1 and RightRotate__t5VISet1ZP5VIWndi(set, g) is called. The mirror image handles a parent that
// is a right child. At the end the root's colour word is set to 0 and the new index is returned.
// Inferred: a red-black tree insertion with 1 meaning red and 0 black; AddNode makes the new
// element red (this routine never colours it).
// Unknown: AddNode's behaviour (not read here beyond its call), and the template parameters
// beyond the mangled name.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
int AddNode__t5VISet1ZP5VIWndii(void* set, int parent, int side);
void LeftRotate__t5VISet1ZP5VIWndi(void* set, int index);
void RightRotate__t5VISet1ZP5VIWndi(void* set, int index);

static inline unsigned* field(const void* set, int index, int offset) {
    const unsigned char* bytes = static_cast<const unsigned char*>(set);
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        NORRATH_WORD_ADDRESS(*reinterpret_cast<const unsigned*>(bytes + 8)));
    return reinterpret_cast<unsigned*>(elements + index * 20 + offset);
}
static inline int root_of(const void* set) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(set) + 12);
}

int Insert__t5VISet1ZP5VIWndRCP5VIWnd(void* set, const unsigned* key) {
    int parent = 0;
    int side = 0;
    int node = root_of(set);
    if (node != 0) {
        const unsigned wanted = *key;
        for (;;) {
            parent = node;
            const unsigned found = *field(set, node, 16);
            side = 1;
            if (wanted < found) {
                node = static_cast<int>(*field(set, node, 8));
            } else if (found < wanted) {
                side = 0;
                node = static_cast<int>(*field(set, node, 12));
            } else {
                return parent;
            }
            if (node == 0) break;
        }
    }
    const int created = AddNode__t5VISet1ZP5VIWndii(set, parent, side);
    if (created < 0) return -1;
    *field(set, created, 16) = *key;
    int current = created;
    while (current != root_of(set) && *field(set, static_cast<int>(*field(set, current, 4)), 0) != 0) {
        int up = static_cast<int>(*field(set, current, 4));
        const int grand = static_cast<int>(*field(set, up, 4));
        if (up == static_cast<int>(*field(set, grand, 8))) {
            const int uncle = static_cast<int>(*field(set, grand, 12));
            if (*field(set, uncle, 0) != 0) {
                *field(set, up, 0) = 0;
                *field(set, uncle, 0) = 0;
                *field(set, grand, 0) = 1;
                current = grand;
            } else {
                if (current == static_cast<int>(*field(set, up, 12))) {
                    current = up;
                    LeftRotate__t5VISet1ZP5VIWndi(set, up);
                }
                up = static_cast<int>(*field(set, current, 4));
                const int top = static_cast<int>(*field(set, up, 4));
                *field(set, up, 0) = 0;
                *field(set, top, 0) = 1;
                RightRotate__t5VISet1ZP5VIWndi(set, top);
            }
        } else {
            const int uncle = static_cast<int>(*field(set, grand, 8));
            if (*field(set, uncle, 0) != 0) {
                *field(set, up, 0) = 0;
                *field(set, uncle, 0) = 0;
                *field(set, grand, 0) = 1;
                current = grand;
            } else {
                if (current == static_cast<int>(*field(set, up, 8))) {
                    current = up;
                    RightRotate__t5VISet1ZP5VIWndi(set, up);
                }
                up = static_cast<int>(*field(set, current, 4));
                const int top = static_cast<int>(*field(set, up, 4));
                *field(set, up, 0) = 0;
                *field(set, top, 0) = 1;
                LeftRotate__t5VISet1ZP5VIWndi(set, top);
            }
        }
    }
    *field(set, root_of(set), 0) = 0;
    return created;
}
}
