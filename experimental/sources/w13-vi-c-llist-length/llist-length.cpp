// Observed: Length__C7VILList at 0x010728b8, 52 bytes. With the list as this:
// the first word of the list is a pointer to a header, the first word of the
// header is the first node, and each node's first word links to the next. The
// result is the number of nodes counted from that first node until a node whose
// first word is zero has been counted; zero if there is no first node. Nothing
// is written.
// Inferred: the header-based list shape matches the other VIList walkers, and the
// zero-terminated chain means the sentinel is not counted separately.
// Unknown: whether a sentinel node is ever present, and the member names.
extern "C" {
int Length__C7VILList(const void* list) {
    const void* const* header = *reinterpret_cast<const void* const* const*>(list);
    const void* const* node = static_cast<const void* const*>(*header);
    int count = 0;
    if (node == 0) return 0;
    do {
        ++count;
        node = static_cast<const void* const*>(*node);
    } while (node != 0);
    return count;
}
}
