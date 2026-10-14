// Observed: Add__9VIHSpritei at 0x010617e8, 224 bytes. Let V be this+64, a vector whose first word is zero when it is unused, and whose word at
// +8 (this+72) is the element base. Nodes are 256 bytes.
//  1. A zero first word at V returns -1 without calling anything.
//  2. N = Add__t8VIVector1Z13VIHSpriteNode(V); a negative result returns -1.
//  3. Add__t8VIVector1Z10VIMatrix44(this+76); a negative result returns -1 (the node is not removed).
//  4. With the element base read after both calls, the word at node N + 0 is set to the argument p. The word at node p + 4 is then read. When it
//     is -1 it is replaced by N. Otherwise the chain of words at +8 is followed from that index until a word equal to -1 is read, and the
//     last index visited has N stored in its word at +8. N is returned.
// Inferred: p is a parent node index; +4 is a node's first-child index and +8 its next-sibling index, both -1 for none, so the new node is
// appended as the last child of p. Add of the vector is expected to give the node -1 in its links (nothing here writes N's +4 or +8).
// Unknown: whether p may be -1 (the code does not test it) and the other fields of a node.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Add__t8VIVector1Z13VIHSpriteNode(void* vector);
int Add__t8VIVector1Z10VIMatrix44(void* vector);

int Add__9VIHSpritei(unsigned char* self, int parent) {
    if (*reinterpret_cast<unsigned*>(self + 64) == 0) return -1;
    int node = Add__t8VIVector1Z13VIHSpriteNode(self + 64);
    if (node < 0) return -1;
    if (Add__t8VIVector1Z10VIMatrix44(self + 76) < 0) return -1;
    unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 72));
    *reinterpret_cast<int*>(nodes + node * 256) = parent;
    int link = *reinterpret_cast<int*>(nodes + parent * 256 + 4);
    if (link == -1) {
        *reinterpret_cast<int*>(nodes + parent * 256 + 4) = node;
    } else {
        int last;
        do {
            last = link;
            link = *reinterpret_cast<int*>(nodes + last * 256 + 8);
        } while (link != -1);
        *reinterpret_cast<int*>(nodes + last * 256 + 8) = node;
    }
    return node;
}
}
