// Host contract for objectRemoveFromGrid__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
extern "C" ListNode* norrath_constructor_default_head_view = nullptr;
extern "C" void objectRemoveFromGrid__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    // A grid cell chain head -> a -> twin -> c, with the twin held by the object.
    ListNode grid_node = {}, a = {}, twin = {}, c = {};
    ListNode* cell = &a;
    a.prev = reinterpret_cast<ListNode*>(&cell); a.next = &twin;
    twin.prev = &a; twin.next = &c; c.prev = &twin;
    grid_node.partner = &twin;
    GameObject object = {};
    object.grid_node = &grid_node;
    ListNode old_first = {};
    old_first.prev = reinterpret_cast<ListNode*>(&norrath_constructor_default_head_view);
    norrath_constructor_default_head_view = &old_first;

    objectRemoveFromGrid__FP10GameObject(&object);
    expect(a.next == &c && c.prev == &a, "the twin is unlinked from the middle of its chain");
    expect(norrath_constructor_default_head_view == &twin, "the twin is the new first off-grid node");
    expect(twin.next == &old_first && old_first.prev == &twin, "the old first off-grid node follows it");
    expect(twin.prev == reinterpret_cast<ListNode*>(&norrath_constructor_default_head_view), "previous word is the head address");
    expect(object.grid_node == &grid_node && grid_node.partner == &twin, "the object's own grid node is untouched");

    // Last in its chain, with an empty off-grid list.
    ListNode only = {}, other = {}, twin2 = {};
    ListNode* cell2 = &other;
    other.prev = reinterpret_cast<ListNode*>(&cell2); other.next = &twin2; twin2.prev = &other;
    only.partner = &twin2;
    object.grid_node = &only;
    norrath_constructor_default_head_view = nullptr;
    objectRemoveFromGrid__FP10GameObject(&object);
    expect(other.next == nullptr, "the previous node loses its successor");
    expect(norrath_constructor_default_head_view == &twin2 && twin2.next == nullptr, "a lone node becomes the head");
    if (failures) return 1;
    std::printf("objectRemoveFromGrid contract passed\n");
    return 0;
}
