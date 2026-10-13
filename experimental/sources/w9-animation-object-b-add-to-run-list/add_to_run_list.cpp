// Observed objectAddToRunList__FP10GameObject at 0x143830, extent 68.
// Detaches the object's run node (the word at object+4) through listRemove when
// it is nonzero, then links the object onto the single run list: first head is
// runList (0x486878), second head is object+4, value is the object. The link's
// result is discarded and nothing is returned. The same shape as the delete-list
// routine, except the second head here is object+4.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" unsigned norrath_list_remove(ListNode* node);
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" HeadSlot runList;

extern "C" void objectAddToRunList__FP10GameObject(GameObject* object) {
    if (object->run_node) norrath_list_remove(object->run_node);
    listOnewayLink__FP8ListHeadT0Pv(&runList.head, reinterpret_cast<ListHead*>(&object->run_node), object);
}
