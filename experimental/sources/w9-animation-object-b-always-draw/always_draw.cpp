// Observed objectAlwaysDraw__FP10GameObject at 0x143498, extent 60.
// Links the object onto alwaysDrawList (0x486888) through listOnewayLink with
// the OBJECT ADDRESS as the second head (as the delete-list routine does) and
// the object as the value, then sets bit 1 (0x2) of the flag word at object+12.
// The flag is set after the link returns. Nothing is returned.
// Unknown: what the bit means to the draw loop; it is cleared again by
// objectCancelAllAlwaysDraw.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" HeadSlot alwaysDrawList;

extern "C" void objectAlwaysDraw__FP10GameObject(GameObject* object) {
    listOnewayLink__FP8ListHeadT0Pv(&alwaysDrawList.head, reinterpret_cast<ListHead*>(object), object);
    object->flags |= 2u;
}
