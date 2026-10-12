// Observed listOnewayLink__FP8ListHeadT0Pv at 0x194390, extent 140.
// Takes two nodes from the free-node stack (freeNodes at 0x486a6c, reached
// through gp), clearing each popped node's halfword at +16. The first node is
// pushed on the front of the first head and the second on the front of the
// second head. The nodes point at each other through +8; the first carries the
// caller's value at +12 and the second carries zero. The first node is returned.
// Each push stores the head's old first node at +0, repairs that node's previous
// word when it exists, stores the head address at +4 and then the new node in
// the head. There is no empty-stack check: an empty stack faults on the first
// store. Unknown: who fills the stack.
#include "../w8-object-list-math-common/list_types.hpp"

// The reviewed view of freeNodes published by object-infra-state.
extern "C" ListNode* norrath_constructor_list_pool_view;

namespace {
ListNode* take_free_node() {
    ListNode* node = norrath_constructor_list_pool_view;
    if (node) {
        norrath_constructor_list_pool_view = node->next;
        node->flags = 0;
    }
    return node;
}

void push_front(ListHead* head, ListNode* node) {
    node->next = head->first;
    if (head->first) head->first->prev = node;
    node->prev = reinterpret_cast<ListNode*>(head);
    head->first = node;
}
}  // namespace

extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value) {
    ListNode* first = take_free_node();
    ListNode* second = take_free_node();
    push_front(first_head, first);
    push_front(second_head, second);
    first->value = value;
    first->partner = second;
    second->value = nullptr;
    second->partner = first;
    return first;
}
