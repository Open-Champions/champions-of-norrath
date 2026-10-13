// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "trailfx-clear-segments-forward.cpp"

struct Node { Node* next; Node* prev; };
static void* removed[8];
static int rn;
static void* added[8];
static void* added_list[8];
static int an;
extern "C" void Remove__7VILNode(void* node) {
    removed[rn++] = node;
    static_cast<Node*>(node)->next = 0;  // like the image's Remove: the node's own link is cleared
}
extern "C" void AddTail__7VILListP7VILNode(void* list, void* node) {
    added_list[an] = list;
    added[an++] = node;
    static_cast<Node*>(node)->next = reinterpret_cast<Node*>(list);  // linked behind the tail: non-zero
}

int main() {
    alignas(16) unsigned char fx[96] = {};
    Node a, b, c, tail;
    tail.next = 0; tail.prev = &c;
    a.next = &b; b.next = &c; c.next = &tail;
    ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(fx, &a);
    // Round 1 reads a.next = b, moves a, current = b; b.next != 0 so round 2 reads a.next again
    // (now the list pointer written by AddTail), moves b, current = that pointer.
    if (rn < 2 || removed[0] != &a || removed[1] != &b) return 1;
    if (added[0] != &a || added[1] != &b) return 2;
    if (added_list[0] != fx + 40 || added_list[1] != fx + 40) return 3;
    return 0;
}
