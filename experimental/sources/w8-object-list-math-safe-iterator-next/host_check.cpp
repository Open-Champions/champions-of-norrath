// Host contract for next__16ListSafeIterator. Not an EE plan input.
#include "../w8-object-list-math-common/iterator_host.hpp"
using namespace host;

int main() {
    Fixture f;
    ListSafeIterator it;
    ListNode* chain[16];

    const unsigned flags[] = {0, 0x10, 0, 0, 0x10, 0};
    const bool payload[] = {true, true, false, true, true, true};
    f.build(6, flags, payload);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x10);
    int visited[8], n = 0;
    while (it.link.prev && n < 8) {
        for (int i = 0; i < 6; ++i) if (it.link.prev == &f.nodes[i]) visited[n++] = i;
        expect(f.walk(chain) == 7, "list stays consistent with one marker during the walk");
        next__16ListSafeIterator(&it);
    }
    expect(n == 3 && visited[0] == 0 && visited[1] == 3 && visited[2] == 5, "visits accepted nodes in order, skipping masked and payload-less ones");
    expect(it.link.prev == nullptr && it.link.next == nullptr, "exhausted marker has both links zero");
    expect(f.walk(chain) == 6, "marker has left the list");
    for (int i = 0; i < 6; ++i) expect(chain[i] == &f.nodes[i], "original order restored");

    // Removing the current node between steps is the point of the marker.
    f.build(4, (const unsigned[]){0, 0, 0, 0}, (const bool[]){true, true, true, true});
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    ListNode* current = it.link.prev;
    expect(current == &f.nodes[0], "starts on the first node");
    current->prev->next = current->next;          // what a list removal does
    if (current->next) current->next->prev = current->prev;
    next__16ListSafeIterator(&it);
    expect(it.link.prev == &f.nodes[1], "advances to the second node after the first was removed");
    expect(f.walk(chain) == 4 && chain[0] == &f.nodes[1] && chain[1] == &it.link, "head now leads to the second node, marker behind it");

    // Last element: advancing past it zeroes the marker.
    f.build(1, (const unsigned[]){0}, (const bool[]){true});
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    expect(it.link.prev == &f.nodes[0] && it.link.next == nullptr, "single node, marker at the end");
    next__16ListSafeIterator(&it);
    expect(it.link.prev == nullptr && it.link.next == nullptr && f.nodes[0].next == nullptr, "advance from the last node unlinks and exhausts");

    // Mask is read from the stored word, so changing it between steps takes effect.
    const unsigned flags2[] = {0, 0x4, 0};
    const bool payload2[] = {true, true, true};
    f.build(3, flags2, payload2);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    it.mask = 0x4;
    next__16ListSafeIterator(&it);
    expect(it.link.prev == &f.nodes[2], "stored mask is used by next");

    if (failures) return 1;
    std::printf("ListSafeIterator next contract passed\n");
    return 0;
}
