// Host contract for objectAddToSlowRunList__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
extern "C" ListHead slowRunList[60];
extern "C" int slowListCycle = 0;
ListHead slowRunList[60];
static int removes, links;
static ListNode* removed;
static ListHead *link_first, *link_second;
static void* link_value;
static int cycle_seen_by_link;
extern "C" unsigned norrath_list_remove(ListNode* node) { ++removes; removed = node; return 0; }
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    ++links; link_first = a; link_second = b; link_value = v; cycle_seen_by_link = slowListCycle; return nullptr;
}
extern "C" void objectAddToSlowRunList__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObject object = {};
    slowListCycle = 0;
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(removes == 0 && links == 1, "no removal without a run node");
    expect(slowListCycle == 1 && link_first == &slowRunList[1], "cycle 0 advances to head 1");
    expect(cycle_seen_by_link == 1, "cycle is stored before the link call");
    expect(link_second == reinterpret_cast<ListHead*>(&object.run_node) && link_value == &object, "second head is object+4, value is the object");
    slowListCycle = 58;
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(slowListCycle == 59 && link_first == &slowRunList[59], "58 advances to the last head");
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(slowListCycle == 0 && link_first == &slowRunList[0], "59 wraps to zero");
    slowListCycle = 200;
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(slowListCycle == 0, "any value at or above the limit wraps");
    slowListCycle = -5;
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(slowListCycle == -4 && link_first == &slowRunList[-4], "the compare is signed with no lower clamp");
    ListNode node;
    object.run_node = &node;
    objectAddToSlowRunList__FP10GameObject(&object);
    expect(removes == 1 && removed == &node, "the run node is removed first");
    if (failures) return 1;
    std::printf("objectAddToSlowRunList contract passed\n");
    return 0;
}
