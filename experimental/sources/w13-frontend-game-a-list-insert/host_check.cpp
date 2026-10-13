// Host check for listInsert__FP8ListHeadP10__listNodePv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The node layout is typed, so the check runs at any pointer width. The empty-free-list case (a
// fault in the original) is not exercised.
#include "list_insert.cpp"

#include <cstdio>

extern "C" ListNode* freeNodes = nullptr;

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static ListNode pool[6];

static void resetPool() {
    for (int i = 0; i < 6; ++i) {
        pool[i].next = i < 5 ? &pool[i + 1] : nullptr;
        pool[i].previous = reinterpret_cast<ListNode*>(0x77);
        pool[i].field8 = 0x55;
        pool[i].data = reinterpret_cast<void*>(0x66);
        pool[i].field16 = 0x1234;
    }
    freeNodes = &pool[0];
}

int main() {
    resetPool();
    ListHead head = {nullptr};
    int a, b, c, d;
    // empty list, no anchor: the node becomes the first one, with the head as its previous
    ListNode* n0 = listInsert__FP8ListHeadP10__listNodePv(&head, nullptr, &a);
    CHECK(n0 == &pool[0] && freeNodes == &pool[1]);
    CHECK(head.first == n0 && n0->next == nullptr && n0->previous == reinterpret_cast<ListNode*>(&head));
    CHECK(n0->data == &a && n0->field8 == 0 && n0->field16 == 0);
    // another without an anchor goes in front of the first
    ListNode* n1 = listInsert__FP8ListHeadP10__listNodePv(&head, nullptr, &b);
    CHECK(n1 == &pool[1] && freeNodes == &pool[2]);
    CHECK(head.first == n1 && n1->next == n0 && n0->previous == n1 && n1->previous == reinterpret_cast<ListNode*>(&head));
    CHECK(n1->data == &b && n0->data == &a);
    // an anchor puts the node right after it
    ListNode* n2 = listInsert__FP8ListHeadP10__listNodePv(&head, n1, &c);
    CHECK(n2 == &pool[2] && n1->next == n2 && n2->previous == n1 && n2->next == n0 && n0->previous == n2);
    CHECK(head.first == n1 && n2->data == &c);
    // after the last element: nothing follows, nothing else is changed
    ListNode* n3 = listInsert__FP8ListHeadP10__listNodePv(&head, n0, &d);
    CHECK(n3 == &pool[3] && n0->next == n3 && n3->previous == n0 && n3->next == nullptr);
    CHECK(head.first == n1 && n1->next == n2 && n2->next == n0 && n0->previous == n2 && n2->previous == n1);
    // the walk forwards and backwards sees the same four nodes
    ListNode* order[4] = {n1, n2, n0, n3};
    ListNode* at = head.first;
    for (int i = 0; i < 4; ++i, at = at->next) CHECK(at == order[i]);
    CHECK(at == nullptr);
    for (int i = 3; i > 0; --i) CHECK(order[i]->previous == order[i - 1]);
    // untouched nodes keep their fields, and only the taken ones lost their halfword
    CHECK(pool[4].field16 == 0x1234 && pool[4].field8 == 0x55 && freeNodes == &pool[4]);
    std::puts("list insert passed");
    return 0;
}
