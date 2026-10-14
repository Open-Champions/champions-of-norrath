// Observed: RemHead__7VILList at 0x01072838, 64 bytes. The list object's first word
// points to a node whose first word is a "next" link and whose second word is a "prev"
// link. If that node's next link is null the function returns null and changes nothing.
// If its prev link is null it returns the node unchanged. Otherwise it stores the node's
// next into the prev node's first word, stores the node's prev into the next node's second
// word, clears both links of the node and returns the node.
// Inferred: a doubly linked list whose head word names the first element; the two-way
// splice is the detach of that element.
// Unknown: the member names and who owns the nodes; no list-end sentinel is written.
struct VILLink {
    VILLink* next;
    VILLink* prev;
};

struct VILList {
    VILLink* head;
};

extern "C" {
VILLink* RemHead__7VILList(VILList* list) {
    VILLink* node = list->head;
    if (node->next == 0) return 0;
    if (node->prev == 0) return node;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = 0;
    node->prev = 0;
    return node;
}
}
