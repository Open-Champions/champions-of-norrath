// Host contract for objectAlwaysDraw__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" HeadSlot alwaysDrawList = {{nullptr}, 0};
static ListHead *link_first, *link_second;
static void* link_value;
static unsigned flags_at_link;
static GameObject* current;
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    link_first = a; link_second = b; link_value = v; flags_at_link = current->flags; return nullptr;
}
extern "C" void objectAlwaysDraw__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObject object = {};
    object.flags = 0x10000401u;
    current = &object;
    objectAlwaysDraw__FP10GameObject(&object);
    expect(link_first == &alwaysDrawList.head, "first head is alwaysDrawList");
    expect(link_second == reinterpret_cast<ListHead*>(&object), "second head is the object address");
    expect(link_value == &object, "value is the object");
    expect(flags_at_link == 0x10000401u, "the flag is set after the link");
    expect(object.flags == 0x10000403u, "bit 1 is set and other bits are kept");
    if (failures) return 1;
    std::printf("objectAlwaysDraw contract passed\n");
    return 0;
}
