// Host contract for objectCancelAllAlwaysDraw__Fv. Not an EE plan input.
// Build: c++ -std=c++17 cancel_all_always_draw.cpp ../w8-object-list-math-safe-iterator-construct/safe_iterator_construct.cpp \
//        ../w8-object-list-math-safe-iterator-next/safe_iterator_next.cpp \
//        ../w8-object-list-math-safe-iterator-destroy/safe_iterator_destroy.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/safe_iterator.hpp"

struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" {
int objectIgnoreFlags;
HeadSlot alwaysDrawList;
void norrath_animation_state_deallocate(void*) {}
}
static int destroyed;
static ListHead* destroyed_head;
extern "C" void listDestroy__FP8ListHead(void* head) { ++destroyed; destroyed_head = static_cast<ListHead*>(head); }
extern "C" void objectCancelAllAlwaysDraw__Fv();

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObjectLinks objects[4];
    ListNode nodes[4];
    std::memset(objects, 0, sizeof objects);
    std::memset(nodes, 0, sizeof nodes);
    objects[0].flags = 0x3u;          // drawn, no ignore bit
    objects[1].flags = 0x2u | 0x80u;  // ignored by the mask
    objects[2].flags = 0x7u;
    objects[3].flags = 0x2u;
    objectIgnoreFlags = 0x80;
    ListNode* previous = reinterpret_cast<ListNode*>(&alwaysDrawList.head);
    for (int i = 0; i < 4; ++i) {
        nodes[i].value = &objects[i];
        nodes[i].prev = previous;
        previous->next = &nodes[i];
        previous = &nodes[i];
    }
    nodes[3].value = &objects[3];
    objectCancelAllAlwaysDraw__Fv();
    expect(objects[0].flags == 0x1u, "bit 1 cleared, other bits kept");
    expect(objects[1].flags == 0x82u, "masked object is skipped");
    expect(objects[2].flags == 0x5u, "third object cleared");
    expect(objects[3].flags == 0x0u, "last object cleared");
    expect(destroyed == 1 && destroyed_head == &alwaysDrawList.head, "list destroyed once after the walk");
    // An empty list is only destroyed.
    alwaysDrawList.head.first = nullptr;
    destroyed = 0;
    objectCancelAllAlwaysDraw__Fv();
    expect(destroyed == 1, "empty list still destroyed");
    if (failures) return 1;
    std::printf("objectCancelAllAlwaysDraw contract passed\n");
    return 0;
}
