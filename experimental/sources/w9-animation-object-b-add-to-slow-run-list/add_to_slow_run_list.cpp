// Observed objectAddToSlowRunList__FP10GameObject at 0x1438a0, extent 100.
// Detaches the run node (object+4) with listRemove when it is nonzero. Then
// advances the shared cycle word slowListCycle (0x483ad0) by one, wrapping to
// zero when the signed result reaches 60, stores it, and links the object onto
// the head slowRunList[cycle] (0x4fbfd0, sixty one-word heads) with object+4 as
// the second head. The link result is not used.
// Distinct from the earlier authored norrath_object_add_to_slow_run_list, which
// keeps its own head table; this definition uses the slowRunList that the
// run-list-heads unit defines at the observed address.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};

extern "C" unsigned norrath_list_remove(ListNode* node);
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" ListHead slowRunList[60];
extern "C" int slowListCycle;

extern "C" void objectAddToSlowRunList__FP10GameObject(GameObject* object) {
    if (object->run_node) norrath_list_remove(object->run_node);
    int cycle = slowListCycle + 1;
    if (cycle >= 60) cycle = 0;
    slowListCycle = cycle;
    listOnewayLink__FP8ListHeadT0Pv(&slowRunList[cycle], reinterpret_cast<ListHead*>(&object->run_node), object);
}
