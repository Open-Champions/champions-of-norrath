// Host contract for objectActiveProcessing__FP10GameObjectii. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off active_processing.cpp host_check.cpp
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct Position { float x, y, z; };
struct ActiveObject { void* word0; ListNode* run_node; ListNode* grid_node; unsigned flags; Position position; };
struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" {
HeadSlot runList;
ListHead slowRunList[60];
int slowListCycle;
unsigned char objectVisUseCamCenter;
signed char objectVisCount;
Position objectVisPoints[4];
float worldViewCenter[3];
int objectActiveProcessing__FP10GameObjectii(ActiveObject*, int, int);
}
static int removes, links, link_cycle;
static ListNode* removed;
static ListHead *link_first, *link_second;
static void* link_value;
extern "C" unsigned norrath_list_remove(ListNode* n) { ++removes; removed = n; return 0; }
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    ++links; link_first = a; link_second = b; link_value = v; link_cycle = slowListCycle; return nullptr;
}
static void reset() { removes = links = 0; link_first = link_second = nullptr; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    ActiveObject object = {};
    object.position = {10.0f, 0.0f, 0.0f};

    // Camera-centre mode: reference is worldViewCenter.
    objectVisUseCamCenter = 1;
    worldViewCenter[0] = 0; worldViewCenter[1] = 0; worldViewCenter[2] = 0;
    expect(objectActiveProcessing__FP10GameObjectii(&object, 1, 11) == 1 && links == 0, "inside and unchanged: no list traffic");
    reset();
    expect(objectActiveProcessing__FP10GameObjectii(&object, 1, 10) == 0, "distance equal to radius is outside (strict compare)");
    expect(links == 1 && link_first == &slowRunList[1] && slowListCycle == 1, "leaving range links the slow list at the advanced cycle");
    expect(link_cycle == 1, "cycle is stored before the link call");
    expect(link_second == reinterpret_cast<ListHead*>(&object.run_node) && link_value == &object, "second head is object+4");
    reset();
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 10) == 0 && links == 0, "outside and unchanged: no list traffic");
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 11) == 1, "inside the radius");
    expect(links == 1 && link_first == &runList.head && link_second == reinterpret_cast<ListHead*>(&object.run_node), "entering range links runList");
    // The flag bit selects the fixed radius 0xabe0.
    reset();
    object.flags = 0x80000;
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 1) == 1, "bit 19 gives the fixed radius");
    object.position = {43999.0f, 0, 0};
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 1000000) == 1, "just inside 0xabe0");
    object.position = {44000.0f, 0, 0};
    expect(objectActiveProcessing__FP10GameObjectii(&object, 1, 1000000) == 0, "0xabe0 = 44000 is outside (strict)");
    object.flags = 0;
    // Wrap and removal.
    reset();
    ListNode node;
    object.run_node = &node;
    slowListCycle = 59;
    objectActiveProcessing__FP10GameObjectii(&object, 1, 5);
    expect(removes == 1 && removed == &node && slowListCycle == 0 && link_first == &slowRunList[0], "wraps to zero and removes first");
    reset();
    object.run_node = &node;
    object.position = {1.0f, 0.0f, 0.0f};
    objectActiveProcessing__FP10GameObjectii(&object, 0, 100);
    expect(removes == 1 && link_first == &runList.head, "entering also removes the old node");

    // Point-list mode.
    objectVisUseCamCenter = 0;
    object.run_node = nullptr;
    object.position = {100.0f, 100.0f, 100.0f};
    objectVisCount = 0;
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 1000) == 0, "no points: not active");
    objectVisCount = -3;
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 1000) == 0, "negative count: not active");
    objectVisCount = 3;
    objectVisPoints[0] = {0, 0, 0};
    objectVisPoints[1] = {100, 100, 150};
    objectVisPoints[2] = {100, 100, 100};
    objectVisPoints[3] = {100, 100, 100};
    reset();
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 51) == 1, "second point within 51");
    objectVisCount = 1;
    expect(objectActiveProcessing__FP10GameObjectii(&object, 1, 51) == 0, "only the first point is used when count is one");
    objectVisCount = 4;
    objectVisPoints[0] = {0, 0, 0}; objectVisPoints[1] = {0, 0, 0}; objectVisPoints[2] = {0, 0, 0};
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 1) == 1, "the fourth point (exactly on the object) is reached");
    // Sum order: x*x + y*y first, then + z*z, compared against a float of the integer square.
    objectVisUseCamCenter = 1;
    worldViewCenter[0] = 0; worldViewCenter[1] = 0; worldViewCenter[2] = 0;
    object.position = {3.0f, 4.0f, 12.0f};
    expect(objectActiveProcessing__FP10GameObjectii(&object, 0, 13) == 0, "169 < 169 is false");
    expect(objectActiveProcessing__FP10GameObjectii(&object, 1, 14) == 1, "169 < 196");
    if (failures) return 1;
    std::printf("objectActiveProcessing contract passed\n");
    return 0;
}
