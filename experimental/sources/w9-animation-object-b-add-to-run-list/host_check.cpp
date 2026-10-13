// Host contract for objectAddToRunList__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" HeadSlot runList = {{nullptr}, 0};
static int order, remove_order, link_order, removes;
static ListNode* removed;
static ListHead *link_first, *link_second;
static void* link_value;
extern "C" unsigned norrath_list_remove(ListNode* node) { ++removes; removed = node; remove_order = ++order; return 0; }
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    link_first = a; link_second = b; link_value = v; link_order = ++order; return nullptr;
}
extern "C" void objectAddToRunList__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObject object = {};
    objectAddToRunList__FP10GameObject(&object);
    expect(removes == 0 && link_order == 1, "no removal without a run node");
    expect(link_first == &runList.head, "first head is runList");
    expect(link_second == reinterpret_cast<ListHead*>(&object.run_node), "second head is object+4");
    expect(link_value == &object, "value is the object");
    ListNode node;
    object.run_node = &node;
    order = 0;
    objectAddToRunList__FP10GameObject(&object);
    expect(removes == 1 && removed == &node && remove_order < link_order, "the run node is removed before linking");
    if (failures) return 1;
    std::printf("objectAddToRunList contract passed\n");
    return 0;
}
