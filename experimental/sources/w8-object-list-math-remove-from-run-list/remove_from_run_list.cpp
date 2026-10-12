// Observed objectRemoveFromRunList__FP10GameObject at 0x143878, extent 40.
// If the node word at object+4 is nonzero it is passed to listRemove
// (listRemove__FP10__listNode, 0x1944a0); nothing is stored in the object, so
// the word at +4 keeps its old value afterwards. The authored provider for the
// listRemove call is norrath_list_remove, used here so that no new import is
// opened.
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};

extern "C" unsigned norrath_list_remove(ListNode* node);

extern "C" void objectRemoveFromRunList__FP10GameObject(GameObject* object) {
    if (object->run_node) norrath_list_remove(object->run_node);
}
