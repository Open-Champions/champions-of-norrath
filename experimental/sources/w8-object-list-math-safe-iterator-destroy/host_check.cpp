// Host contract for _$_16ListSafeIterator. Not an EE plan input.
#include "../w8-object-list-math-common/iterator_host.hpp"
using namespace host;


int main() {
    Fixture f;
    ListSafeIterator it;
    ListNode* chain[16];
    const unsigned flags[] = {0, 0, 0};
    const bool payload[] = {true, true, true};

    f.build(3, flags, payload);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    _$_16ListSafeIterator(&it, 0);
    expect(f.walk(chain) == 3 && chain[0] == &f.nodes[0] && chain[2] == &f.nodes[2], "marker unlinked from the middle");
    expect(host::deallocations == 0, "flag bit 0 clear does not delete");

    f.build(3, flags, payload);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    _$_16ListSafeIterator(&it, 2);
    expect(host::deallocations == 0, "only bit 0 requests deletion");
    f.build(3, flags, payload);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    _$_16ListSafeIterator(&it, 3);
    expect(host::deallocations == 1 && host::deallocated == &it, "bit 0 deletes the marker's storage");
    expect(f.walk(chain) == 3, "list restored before the delete call");

    // Marker at the end: the previous node simply loses its successor.
    const unsigned flags2[] = {1, 0};
    const bool payload2[] = {true, true};
    f.build(2, flags2, payload2);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 1);
    expect(it.link.prev == &f.nodes[1] && it.link.next == nullptr, "marker at the tail");
    _$_16ListSafeIterator(&it, 0);
    expect(f.nodes[1].next == nullptr && f.walk(chain) == 2, "tail marker removed");

    // Exhausted marker (both links zero) touches nothing.
    f.build(2, flags2, payload2);
    std::memset(&it, 0, sizeof it);
    _$_16ListSafeIterator(&it, 0);
    expect(f.walk(chain) == 2, "exhausted marker leaves the list alone");

    if (failures) return 1;
    std::printf("ListSafeIterator destroy contract passed\n");
    return 0;
}
