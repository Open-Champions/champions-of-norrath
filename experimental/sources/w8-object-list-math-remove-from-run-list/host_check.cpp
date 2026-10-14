// Host contract for objectRemoveFromRunList__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/object_types.hpp"

struct GameObject : GameObjectLinks {};
static int calls;
static ListNode* seen;
extern "C" unsigned norrath_list_remove(ListNode* node) { ++calls; seen = node; return 0; }
extern "C" void objectRemoveFromRunList__FP10GameObject(GameObject*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObject object = {};
    objectRemoveFromRunList__FP10GameObject(&object);
    expect(calls == 0, "no call when the node word is zero");
    ListNode node;
    object.run_node = &node;
    objectRemoveFromRunList__FP10GameObject(&object);
    expect(calls == 1 && seen == &node, "the node at +4 is removed");
    expect(object.run_node == &node, "the object's node word is left alone");
    if (failures) return 1;
    std::printf("objectRemoveFromRunList contract passed\n");
    return 0;
}
