// Observed objectAddToDeleteList__FP10GameObject at 0x1437e8, extent 68.
// Detaches the node at object+4 through listRemove when it is nonzero, then
// calls listOnewayLink with the delete list head (deleteList, 0x486880) as the
// first head and the OBJECT ADDRESS ITSELF (not object+4) as the second head,
// and the object as the value. The link's result is returned. The run-list
// callers pass object+4 as the second head; the difference is observed in the
// instruction stream and is kept. Unknown: why this routine differs, and what the
// word at object+0 holds, which the link rewrites.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};

extern "C" unsigned norrath_list_remove(ListNode* node);
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" ListHead deleteList;

extern "C" ListNode* objectAddToDeleteList__FP10GameObject(GameObject* object) {
    if (object->run_node) norrath_list_remove(object->run_node);
    return listOnewayLink__FP8ListHeadT0Pv(&deleteList, reinterpret_cast<ListHead*>(object), object);
}
