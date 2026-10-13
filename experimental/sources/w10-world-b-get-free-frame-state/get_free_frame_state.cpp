#include "../w10-world-b-common/mem.hpp"
// Observed getFreeFrameState__Fv at 0x00161450, 212 bytes in SLUS_205.65. Works on a
// doubly linked list of frame-state nodes with two head words: fsListBack (0x00486910, the
// front node that gets recycled) and fsList (0x0048690c, the last node). A node holds its
// previous node at +12, next node at +16, an owner pointer at +20 (whose halfword at +28 is
// tested), a word at +8 and words at +32 and +36.
// Phase 1, only when the front node has an owner: while the front node's owner exists and
// has any of bits 0x4800 set in its halfword at +28 (still in use), that node is moved to the
// end of the list. The node's next becomes the front (fsListBack, when it was the front), and
// the node is appended after the last node and becomes the last. A node with no successor is
// left in place (the image then tests it again). The head words are stored once, after the loop.
// Phase 2: take the front node, make its next the front (the store of 0 to that next's
// previous word is unconditional, as in the image), clear the owner's word at +4 when there
// is an owner, append the node after the last node and make it the last, set +8 to
// 1000000 (0x000f4240) and clear +16, +32, +36 and +20, and return it.
// Inferred: the list is a ring of recycled frame states; the 0x4800 bits mean "busy".
// Unknown: the node size, what the owner is, and what 1000000 stands for. The image loops
// forever when the only node is busy; so does this source. A busy front node that has a
// non-null previous link also loops forever in the image (the front never advances), so the
// store to a middle node's next word is never reached in a finite run; it is kept as written.
extern unsigned char* fsList;
extern unsigned char* fsListBack;

extern "C" unsigned char* getFreeFrameState__Fv() {
    using namespace norrath_wb;
    unsigned char* front = fsListBack;
    unsigned char* last = fsList;
    unsigned char* head = front;
    unsigned char* owner = load_pointer(front + 20);
    if (owner != 0 && (load_half(owner + 28) & 0x4800) != 0) {
        unsigned char* node = front;
        unsigned char* next = load_pointer(node + 16);
        for (;;) {
            if (next != 0) {
                unsigned char* previous = load_pointer(node + 12);
                if (previous == 0) {
                    head = next;
                } else {
                    store_pointer(previous + 16, next);
                }
                front = head;
                store_pointer(next + 12, previous);
                store_pointer(node + 12, last);
                store_pointer(last + 16, node);
                last = node;
                store_pointer(node + 16, 0);
                owner = load_pointer(front + 20);
            } else {
                owner = load_pointer(front + 20);
            }
            if (owner == 0) break;
            node = front;
            if ((load_half(owner + 28) & 0x4800) == 0) break;
            next = load_pointer(node + 16);
        }
        fsList = last;
        fsListBack = head;
    }
    unsigned char* chosen = front;
    unsigned char* following = load_pointer(chosen + 16);
    unsigned char* chosen_owner = load_pointer(chosen + 20);
    fsListBack = following;
    store_pointer(following + 12, 0);
    if (chosen_owner != 0) store_pointer(chosen_owner + 4, 0);
    unsigned char* tail = fsList;
    store_pointer(chosen + 12, tail);
    store_pointer(tail + 16, chosen);
    fsList = chosen;
    store_word(chosen + 8, 1000000);
    store_word(chosen + 16, 0);
    store_word(chosen + 32, 0);
    store_word(chosen + 36, 0);
    store_word(chosen + 20, 0);
    return chosen;
}
