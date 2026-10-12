// Host contract for listOnewayLink__FP8ListHeadT0Pv. Not an EE plan input.
// Build: c++ -std=c++17 list_oneway_link.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/list_types.hpp"

extern "C" ListNode* norrath_constructor_list_pool_view = nullptr;
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead*, ListHead*, void*);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    ListNode pool[4];
    std::memset(pool, 0xee, sizeof pool);
    for (int i = 0; i < 4; ++i) pool[i].next = i < 3 ? &pool[i + 1] : nullptr;
    norrath_constructor_list_pool_view = &pool[0];

    ListNode resident;
    std::memset(&resident, 0, sizeof resident);
    ListHead head_a = {&resident, 0};
    resident.prev = reinterpret_cast<ListNode*>(&head_a);
    ListHead head_b = {nullptr, 0};
    int payload;

    ListNode* first = listOnewayLink__FP8ListHeadT0Pv(&head_a, &head_b, &payload);
    expect(first == &pool[0], "first node comes off the front of the free stack");
    expect(norrath_constructor_list_pool_view == &pool[2], "two nodes were taken");
    expect(first->flags == 0 && pool[1].flags == 0, "popped nodes have their halfword cleared");
    expect(head_a.first == first && first->next == &resident, "first node leads the first head");
    expect(resident.prev == first, "old first node now points back at the new node");
    expect(first->prev == reinterpret_cast<ListNode*>(&head_a), "previous word holds the head address");
    expect(head_b.first == &pool[1] && pool[1].next == nullptr, "second node alone on an empty second head");
    expect(pool[1].prev == reinterpret_cast<ListNode*>(&head_b), "second node previous word is its head");
    expect(first->value == &payload && pool[1].value == nullptr, "value only on the first node");
    expect(first->partner == &pool[1] && pool[1].partner == first, "partners point at each other");
    expect(pool[2].flags == static_cast<short>(0xeeee), "untouched node unchanged");

    // A second call pushes in front of the existing nodes on both heads.
    ListNode* again = listOnewayLink__FP8ListHeadT0Pv(&head_a, &head_b, nullptr);
    expect(again == &pool[2] && head_a.first == again && again->next == first, "front insertion on head a");
    expect(first->prev == again && head_b.first == &pool[3] && pool[3].next == &pool[1], "front insertion on head b");
    expect(pool[1].prev == &pool[3] && norrath_constructor_list_pool_view == nullptr, "chain repaired, stack drained");

    // Both nodes on one head: the second push sees the first.
    ListNode extra[2] = {};
    extra[0].next = &extra[1];
    norrath_constructor_list_pool_view = &extra[0];
    ListHead shared = {nullptr, 0};
    ListNode* n = listOnewayLink__FP8ListHeadT0Pv(&shared, &shared, nullptr);
    expect(shared.first == &extra[1] && extra[1].next == n && n->next == nullptr, "same head receives both nodes in order");
    expect(n->prev == &extra[1] && extra[1].prev == reinterpret_cast<ListNode*>(&shared), "same head links");

    if (failures) return 1;
    std::printf("listOnewayLink contract passed\n");
    return 0;
}
