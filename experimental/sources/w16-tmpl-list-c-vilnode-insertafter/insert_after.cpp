// Observed: InsertAfter__7VILNodeP7VILNode at 0x010726b8, 44 bytes. Arguments: a node and a reference node. A node is two
// words: +0 and +4. Let ref = the second argument.
//  1. If ref+0 is zero, ref becomes the word at ref+4 and the value V is read from the new ref+0; otherwise V = ref+0.
//  2. node+4 = ref; node+0 = V; V+4 = node; then the word now at node+4 (ref) receives node at its +0.
// Net effect: node is linked between ref and V, with +0 acting as the forward link and +4 as the backward link.
// Inferred: a doubly linked insert after the reference; a reference whose forward link is zero is treated
// as an end marker and the node is inserted before it (after its predecessor). Nothing is returned.
// Unknown: the list head layout; no name for the two words.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void InsertAfter__7VILNodeP7VILNode(unsigned self, unsigned reference) {
    unsigned* node = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(self));
    unsigned* ref = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(reference));
    unsigned succ = ref[0];
    if (succ == 0) {
        reference = ref[1];
        ref = reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(reference));
        succ = ref[0];
    }
    node[1] = reference;
    node[0] = succ;
    reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(succ))[1] = self;
    reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(node[1]))[0] = self;
}
}
