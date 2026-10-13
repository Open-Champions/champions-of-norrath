// Observed listInsert__FP8ListHeadP10__listNodePv at 0x001945f0, 108 bytes. A node is taken from the
// free list freeNodes (0x00486a6c): freeNodes advances to the node's first word, and the node's
// halfword at +16 is cleared. The node's word at +12 becomes the third argument and its word at +8 is
// cleared. With a second argument (before) the node is linked directly after it: node.previous =
// before, node.next = before.next, a non-null old next gets previous = node, before.next = node.
// Without one (null) the node becomes the first element of the list head: node.next = head.first,
// a non-null old first gets previous = node, node.previous = the head itself, head.first = node.
// The node is returned. The free list is not checked: with freeNodes null the original stores to
// address 12 (a fault on the console); this version leaves that case as it is.
// Node layout on the console (observed offsets): next +0, previous +4, a word at +8, payload +12,
// a halfword at +16. The head is also stored as a node's previous, so its first word sits at the
// same offset as a node's next. Unknown: what the words at +8 and +16 mean, and the size of a node.
namespace {
struct ListNode {
    ListNode* next;
    ListNode* previous;
    int field8;
    void* data;
    short field16;
};
}
struct ListHead {
    ListNode* first;
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(ListNode, previous) == 4 && __builtin_offsetof(ListNode, field8) == 8 &&
                  __builtin_offsetof(ListNode, data) == 12 && __builtin_offsetof(ListNode, field16) == 16,
              "observed node layout");
#endif

extern "C" ListNode* freeNodes;

extern "C" ListNode* listInsert__FP8ListHeadP10__listNodePv(ListHead* head, ListNode* before, void* data) {
    ListNode* node = freeNodes;
    if (node != nullptr) {
        freeNodes = node->next;
        node->field16 = 0;
    }
    node->data = data;
    node->field8 = 0;
    if (before != nullptr) {
        node->previous = before;
        node->next = before->next;
        if (before->next != nullptr) before->next->previous = node;
        before->next = node;
    } else {
        node->next = head->first;
        if (head->first != nullptr) head->first->previous = node;
        node->previous = reinterpret_cast<ListNode*>(head);
        head->first = node;
    }
    return node;
}
