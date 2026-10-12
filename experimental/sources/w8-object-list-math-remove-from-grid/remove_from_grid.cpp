// Observed objectRemoveFromGrid__FP10GameObject at 0x143558, extent 80.
// Takes the object's grid node (object+8) and its partner (the node at +8 of
// that), unlinks the partner from whatever list holds it, and pushes the
// partner on the front of offTheGrid (0x486890, gp-relative). The unlink writes
// the previous word's target without testing it, then repairs the following
// node when there is one; the push stores the head address as the previous
// word and repairs the old first node. The object's own grid node is untouched.
// Inferred: the partner is the off-grid twin that listOnewayLink pairs with the
// grid node. The authored view of the head is norrath_constructor_default_head_view.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};

extern "C" ListNode* norrath_constructor_default_head_view;

extern "C" void objectRemoveFromGrid__FP10GameObject(GameObject* object) {
    ListNode* node = object->grid_node->partner;
    node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = reinterpret_cast<ListNode*>(&norrath_constructor_default_head_view);
    node->next = norrath_constructor_default_head_view;
    norrath_constructor_default_head_view = node;
    if (node->next) node->next->prev = node;
}
