// Host contract for __16ListSafeIteratorP8ListHeadi. Not an EE plan input.
// Build with the next/destroy sources; see the other host checks.
#include "../w8-object-list-math-common/iterator_host.hpp"
using namespace host;

int main() {
    Fixture f;
    ListSafeIterator it;
    ListNode* chain[16];

    // Empty list: mask stored, links and payload zeroed.
    f.build(0, nullptr, nullptr);
    std::memset(&it, 0xaa, sizeof it);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x30);
    expect(it.mask == 0x30 && it.link.next == nullptr && it.link.prev == nullptr && it.link.value == nullptr, "empty list yields an exhausted marker");

    // First node accepted: marker goes right after it.
    const unsigned flags_a[] = {0, 0, 0};
    const bool payload_a[] = {true, true, true};
    f.build(3, flags_a, payload_a);
    std::memset(&it, 0xaa, sizeof it);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x30);
    expect(it.link.prev == &f.nodes[0] && it.link.next == &f.nodes[1], "marker stands on the first node");
    expect(f.nodes[0].next == &it.link && f.nodes[1].prev == &it.link, "marker is linked between first and second");
    expect(it.link.value == nullptr && it.mask == 0x30, "payload zero and mask kept");
    expect(f.walk(chain) == 4 && chain[0] == &f.nodes[0] && chain[1] == &it.link && chain[2] == &f.nodes[1], "chain with marker is consistent");

    // First two skipped: one has a mask bit, one has no payload.
    const unsigned flags_b[] = {0x10, 0, 0, 0x01};
    const bool payload_b[] = {true, false, true, true};
    f.build(4, flags_b, payload_b);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x10);
    expect(it.link.prev == &f.nodes[2], "skips a masked object and a payload-less node");
    expect(f.nodes[2].next == &it.link && it.link.next == &f.nodes[3], "inserted before the last node");

    // Marker at the end of the list: no successor.
    const unsigned flags_c[] = {0x2, 0x2, 0};
    const bool payload_c[] = {true, true, true};
    f.build(3, flags_c, payload_c);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x2);
    expect(it.link.prev == &f.nodes[2] && it.link.next == nullptr && f.nodes[2].next == &it.link, "marker appended after the last node");

    // Nothing acceptable: exhausted, list untouched.
    const unsigned flags_d[] = {0x2, 0x3};
    const bool payload_d[] = {true, true};
    f.build(2, flags_d, payload_d);
    std::memset(&it, 0xaa, sizeof it);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0x2);
    expect(it.link.prev == nullptr && it.link.next == nullptr && f.walk(chain) == 2, "no match leaves the list alone");

    // Mask zero accepts everything with a payload, including flagged objects.
    f.build(2, flags_d, payload_d);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, 0);
    expect(it.link.prev == &f.nodes[0], "mask zero accepts the first node");

    // The mask is the third argument as a full word.
    const unsigned flags_e[] = {0x80000000u, 0};
    const bool payload_e[] = {true, true};
    f.build(2, flags_e, payload_e);
    __16ListSafeIteratorP8ListHeadi(&it, &f.head, static_cast<int>(0x80000000u));
    expect(it.link.prev == &f.nodes[1], "high mask bit excludes");

    if (failures) return 1;
    std::printf("ListSafeIterator constructor contract passed\n");
    return 0;
}
