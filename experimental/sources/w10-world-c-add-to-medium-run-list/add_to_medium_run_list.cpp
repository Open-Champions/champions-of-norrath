// Observed objectAddToMediumRunList__FP10GameObject at 0x143908, extent 100.
// Detaches the run node (object+4) with listRemove when it is nonzero. Then
// advances the shared cycle word mediumListCycle (0x483ad4) by one, setting it
// to zero when the signed result reaches 15, stores it, and links the object
// onto the head mediumRunList[cycle] (0x4fbf90, fifteen one-word heads) with
// object+4 as the second head. The store happens in the delay slot of the link
// call, so the callee sees the new cycle value. The link result is not used.
// Sibling of objectAddToSlowRunList (60 heads, cycle word 0x483ad0).
// Unknown: the object layout beyond the node word at +4.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};

extern "C" unsigned norrath_list_remove(ListNode* node);
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" ListHead mediumRunList[16];
extern "C" int mediumListCycle;

extern "C" void objectAddToMediumRunList__FP10GameObject(GameObject* object) {
    if (object->run_node) norrath_list_remove(object->run_node);
    int cycle = mediumListCycle + 1;
    if (cycle >= 15) cycle = 0;
    mediumListCycle = cycle;
    listOnewayLink__FP8ListHeadT0Pv(&mediumRunList[cycle], reinterpret_cast<ListHead*>(&object->run_node), object);
}
