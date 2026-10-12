// Host contract for _GLOBAL_$I$objectIgnoreFlags and _GLOBAL_$D$objectIgnoreFlags.
// Not an EE plan input.
// Build: c++ -std=c++17 global_init_object_lists.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w8-object-list-math-common/list_types.hpp"

struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" {
HeadSlot runList, deleteList, alwaysDrawList;
ListHead mediumRunList[16], slowRunList[60], objectGrid[120][120], ghostObjectGrid[120][120], offTheGrid;
void _GLOBAL_$I$objectIgnoreFlags();
void _GLOBAL_$D$objectIgnoreFlags();
}
static std::vector<ListHead*> destroyed;
extern "C" void listDestroy__FP8ListHead(ListHead* head) { destroyed.push_back(head); }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    ListNode marker;
    auto poison = [&]() {
        runList.head.first = deleteList.head.first = alwaysDrawList.head.first = offTheGrid.first = &marker;
        runList.unobserved_word = deleteList.unobserved_word = alwaysDrawList.unobserved_word = 0x5555;
        for (auto& h : mediumRunList) h.first = &marker;
        for (auto& h : slowRunList) h.first = &marker;
        for (auto& row : objectGrid) for (auto& h : row) h.first = &marker;
        for (auto& row : ghostObjectGrid) for (auto& h : row) h.first = &marker;
    };
    poison();
    _GLOBAL_$I$objectIgnoreFlags();
    bool all_clear = runList.head.first == nullptr && deleteList.head.first == nullptr && alwaysDrawList.head.first == nullptr && offTheGrid.first == nullptr;
    for (int i = 0; i < 15; ++i) all_clear &= mediumRunList[i].first == nullptr;
    for (int i = 0; i < 60; ++i) all_clear &= slowRunList[i].first == nullptr;
    for (int r = 0; r < 120; ++r) for (int c = 0; c < 120; ++c) all_clear &= objectGrid[r][c].first == nullptr && ghostObjectGrid[r][c].first == nullptr;
    expect(all_clear, "construction clears every head");
    expect(mediumRunList[15].first == &marker, "the sixteenth medium word is not a head");
    expect(runList.unobserved_word == 0x5555 && deleteList.unobserved_word == 0x5555 && alwaysDrawList.unobserved_word == 0x5555, "second words of the eight-byte slots are untouched");
    expect(destroyed.empty(), "construction destroys nothing");

    destroyed.clear();
    _GLOBAL_$D$objectIgnoreFlags();
    std::vector<ListHead*> want;
    want.push_back(&offTheGrid);
    for (int i = 14399; i >= 0; --i) want.push_back(&ghostObjectGrid[0][0] + i);
    for (int i = 14399; i >= 0; --i) want.push_back(&objectGrid[0][0] + i);
    want.push_back(&alwaysDrawList.head);
    want.push_back(&deleteList.head);
    for (int i = 59; i >= 0; --i) want.push_back(&slowRunList[i]);
    for (int i = 14; i >= 0; --i) want.push_back(&mediumRunList[i]);
    want.push_back(&runList.head);
    expect(destroyed == want, "destruction visits every head once, in exactly the reverse of construction order");
    expect(destroyed.size() == 1 + 14400 + 14400 + 2 + 60 + 15 + 1, "destruction count");
    if (failures) return 1;
    std::printf("object list global init/destroy contract passed\n");
    return 0;
}
