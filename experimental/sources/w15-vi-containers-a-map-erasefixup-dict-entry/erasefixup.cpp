// Observed: EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi at 0x0120a020, 1020 bytes. Arguments: the map and a node index X.
// Nodes are 28-byte records at B + 28 * index, B being the word at map+8 and re-read after every rotation call (and not otherwise);
// the words at +0, +4, +8 and +12 are called colour, parent, left and right below, and index 0 is a real record that gets written.
// The word at map+12 is the root index. The routine is the red-black delete repair loop, with colour 0 = black and 1 = red:
//  while X is not the root and colour(X) is 0:
//   - when X equals the left index of its parent P: W = right(P). When W is red: colour(W) = 0, colour(P) = 1,
//     LeftRotate(map, P), and W = right(parent(X)). With W black: when W's left child and right child are both black, colour(W) = 1
//     and X = parent(X). Otherwise, when W's left child is red and the right child black: colour(left(W)) = 0, colour(W) = 1,
//     RightRotate(map, W), W = right(parent(X)). Then colour(W) = colour(parent(X)), colour(parent(X)) = 0, colour(right(W)) = 0,
//     LeftRotate(map, parent(X)) and X = the root, which ends the loop.
//   - otherwise (X is any other child of P) the same steps with left and right interchanged and LeftRotate and RightRotate swapped.
// Afterwards colour(X) = 0. The result is 0. The Left/RightRotate callees' results are ignored.
// Inferred: Erase calls this after unlinking a black node (the second step of the textbook deletion for the tree whose Find, Next,
// Erase and FreeNode are published). Rotation arguments are the node index to rotate about.
// Unknown: the rotation routines' bodies are not part of this unit; the colour encoding is read from this code only.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator);
int RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator);
}

namespace {
const unsigned parentSlot = 1;

inline unsigned* node(unsigned char* base, unsigned index) { return reinterpret_cast<unsigned*>(base + 28 * index); }

template <bool IsLeft>
inline int rotateNear(void* map, unsigned index) {
    if (IsLeft) return LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, static_cast<int>(index));
    return RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, static_cast<int>(index));
}

template <bool IsLeft>
inline int rotateFar(void* map, unsigned index) {
    if (IsLeft) return RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, static_cast<int>(index));
    return LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map, static_cast<int>(index));
}

// One pass of the loop for X on the near side of its parent (the left side when IsLeft). Returns the next X.
template <bool IsLeft>
inline unsigned repair(unsigned* m, unsigned char*& base, unsigned x) {
    const unsigned near = IsLeft ? 2 : 3;
    const unsigned far = IsLeft ? 3 : 2;
    unsigned w = node(base, node(base, x)[parentSlot])[far];
    if (node(base, w)[0] != 0) {
        node(base, w)[0] = 0;
        node(base, node(base, x)[parentSlot])[0] = 1;
        rotateNear<IsLeft>(m, node(base, x)[parentSlot]);
        base = at(m[2]);
        w = node(base, node(base, x)[parentSlot])[far];
    }
    const unsigned nearChild = node(base, w)[near];
    if (node(base, nearChild)[0] == 0) {
        if (node(base, node(base, w)[far])[0] == 0) {
            node(base, w)[0] = 1;
            return node(base, x)[parentSlot];
        }
    } else if (node(base, node(base, w)[far])[0] == 0) {
        node(base, nearChild)[0] = 0;
        node(base, w)[0] = 1;
        rotateFar<IsLeft>(m, w);
        base = at(m[2]);
        w = node(base, node(base, x)[parentSlot])[far];
    }
    node(base, w)[0] = node(base, node(base, x)[parentSlot])[0];
    node(base, node(base, x)[parentSlot])[0] = 0;
    node(base, node(base, w)[far])[0] = 0;
    rotateNear<IsLeft>(m, node(base, x)[parentSlot]);
    base = at(m[2]);
    return m[3];
}
}  // namespace

extern "C" {
int EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int iterator) {
    unsigned* m = static_cast<unsigned*>(map);
    unsigned x = static_cast<unsigned>(iterator);
    unsigned char* base = at(m[2]);
    if (x != m[3] && node(base, x)[0] == 0) {
        for (;;) {
            unsigned parent = node(base, x)[parentSlot];
            if (x == node(base, parent)[2]) x = repair<true>(m, base, x);
            else x = repair<false>(m, base, x);
            if (x == m[3] || node(base, x)[0] != 0) break;
        }
    }
    node(base, x)[0] = 0;
    return 0;
}
}
