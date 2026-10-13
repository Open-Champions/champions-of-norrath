// Host contract for objectAddToMediumRunList__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
extern "C" ListHead mediumRunList[16];
extern "C" int mediumListCycle = 0;
ListHead mediumRunList[16];
static int removes, links;
static ListNode* removed;
static ListHead *link_first, *link_second;
static void* link_value;
static int cycle_seen_by_link;
extern "C" unsigned norrath_list_remove(ListNode* node) { ++removes; removed = node; return 0; }
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    ++links; link_first = a; link_second = b; link_value = v; cycle_seen_by_link = mediumListCycle; return nullptr;
}
extern "C" void objectAddToMediumRunList__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObject object = {};
    mediumListCycle = 0;
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(removes == 0 && links == 1, "no removal without a run node");
    expect(mediumListCycle == 1 && link_first == &mediumRunList[1], "cycle 0 advances to head 1");
    expect(cycle_seen_by_link == 1, "cycle is stored before the link call");
    expect(link_second == reinterpret_cast<ListHead*>(&object.run_node) && link_value == &object, "second head is object+4, value is the object");
    mediumListCycle = 13;
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(mediumListCycle == 14 && link_first == &mediumRunList[14], "13 advances to the last head");
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(mediumListCycle == 0 && link_first == &mediumRunList[0], "14 wraps to zero");
    mediumListCycle = 200;
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(mediumListCycle == 0, "any value at or above the limit wraps");
    mediumListCycle = -5;
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(mediumListCycle == -4 && link_first == &mediumRunList[-4], "the compare is signed with no lower clamp");
    ListNode node;
    object.run_node = &node;
    objectAddToMediumRunList__FP10GameObject(&object);
    expect(removes == 1 && removed == &node, "the run node is removed first");
    if (failures) return 1;
    std::printf("objectAddToMediumRunList contract passed\n");
    return 0;
}
