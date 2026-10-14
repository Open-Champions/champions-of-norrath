// Host contract for objectClear__Fv. Not an EE plan input.
// Build: c++ -std=c++17 object_clear.cpp ../w8-object-list-math-safe-iterator-construct/safe_iterator_construct.cpp \
//        ../w8-object-list-math-safe-iterator-next/safe_iterator_next.cpp \
//        ../w8-object-list-math-safe-iterator-destroy/safe_iterator_destroy.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w8-object-list-math-common/safe_iterator.hpp"

struct VirtualEntry { short delta; short reserved; void (*function)(void*, int); };
struct ObjectForClear { void* word0; ListNode* run_node; ListNode* grid_node; unsigned flags; unsigned char gap[72 - 16]; const VirtualEntry* vtable; };
struct HeadSlot { ListHead head; unsigned unobserved_word; };

extern "C" {
int objectIgnoreFlags, lastNmBlocksFree, objectKillingEveryone = 7, slowListCycle = 9, mediumListCycle = 9, objectStrings = 5;
HeadSlot runList;
ListHead objectGrid[120][120], ghostObjectGrid[120][120], offTheGrid;
unsigned stringHash[512], objectHash[512];
void objectClear__Fv();
void norrath_animation_state_deallocate(void*) {}
}

struct Entry {
    ObjectForClear object;
    ListNode node;
    ListHead* head;
};
static Entry entries[16];
static int used;
struct Call { void* self; int flags; int killing; };
static std::vector<Call> calls;

static void unlink(ListNode* node) {
    node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
}
static void destroy_object(void* self, int flags) {
    calls.push_back({self, flags, objectKillingEveryone});
    // The delta moves the pointer by 8 bytes; undo it to find the entry.
    Entry* e = reinterpret_cast<Entry*>(static_cast<char*>(self) - 8);
    unlink(&e->node);
}
static const VirtualEntry table[2] = {{0, 0, nullptr}, {8, 0, destroy_object}};

static Entry* add(ListHead* head, unsigned flags = 0) {
    Entry* e = &entries[used++];
    std::memset(e, 0, sizeof *e);
    e->object.vtable = table;
    e->object.flags = flags;
    e->node.value = &e->object;
    e->head = head;
    ListNode** link = &head->first;
    ListNode* previous = reinterpret_cast<ListNode*>(head);
    while (*link) { previous = *link; link = &(*link)->next; }
    *link = &e->node;
    e->node.prev = previous;
    return e;
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    for (int i = 0; i < 512; ++i) { stringHash[i] = 0x11111111u + i; objectHash[i] = 0x22222222u + i; }

    Entry* r1 = add(&runList.head);
    Entry* r_ignored = add(&runList.head, 0x80);
    Entry* r2 = add(&runList.head);
    Entry* g_first = add(&objectGrid[0][0]);
    Entry* g_second_same_cell = add(&objectGrid[0][0]);
    Entry* g_row1 = add(&objectGrid[1][0]);
    Entry* g_last = add(&objectGrid[119][119]);
    Entry* gh = add(&ghostObjectGrid[5][7]);
    Entry* off = add(&offTheGrid);
    objectIgnoreFlags = 0x80;

    objectClear__Fv();
    Entry* expected[] = {r1, r2, g_first, g_second_same_cell, g_row1, g_last, gh, off};
    bool order = calls.size() == 8;
    for (size_t i = 0; order && i < 8; ++i) order = calls[i].self == reinterpret_cast<char*>(&expected[i]->object) + 8 && calls[i].flags == 3;
    expect(order, "objects deleted with (object + delta, 3): run list, grid rows in order, ghost grid, off-grid");
    expect(r_ignored->node.prev == reinterpret_cast<ListNode*>(&runList.head) && runList.head.first == &r_ignored->node, "the run-list object with an ignore bit survives; the grids ignore the mask");
    bool killing = true;
    for (const Call& c : calls) killing &= c.killing == 1;
    expect(killing, "objectKillingEveryone is 1 while objects are deleted");
    expect(objectKillingEveryone == 0, "and 0 afterwards");
    expect(lastNmBlocksFree == 100000 && slowListCycle == 0 && mediumListCycle == 0 && objectStrings == 0, "counters reset");
    bool zeroed = true;
    for (int i = 0; i < 512; ++i) zeroed &= stringHash[i] == 0 && objectHash[i] == 0;
    expect(zeroed, "both 512-word hash tables zeroed");
    bool empty = objectGrid[0][0].first == nullptr && objectGrid[119][119].first == nullptr && ghostObjectGrid[5][7].first == nullptr && offTheGrid.first == nullptr;
    expect(empty, "no iterator marker is left in any list");
    if (failures) return 1;
    std::printf("objectClear contract passed\n");
    return 0;
}
