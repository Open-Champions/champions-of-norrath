#include "../w9-animation-object-c-common/mem.hpp"
// Observed objectIsOffGrid__FP10GameObject at 0x001435a8, 132 bytes. Walks the default
// off-grid list (the head word offTheGrid at 0x486890) with the list safe iterator
// (constructor with a zero mask word, then next, then the destructor with delete flag 2),
// reading each visited node from iterator+4 and the object from node+12. Returns 1 as
// soon as a node's object equals the argument (after destroying the iterator), and 0
// when the list runs out (also after destroying the iterator). An empty list returns 0
// after one construct and one destroy. The 24-byte iterator lives in the caller's frame.
// Inferred: "off the grid" means the object has been parked on that list by
// objectRemoveFromGrid; the list holds one node per such object.
// Unknown: whether the 0x486848 object of the same name is a second list.
extern "C" {
extern unsigned char* offTheGrid;
void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char** head, int mask);
void next__16ListSafeIterator(unsigned* iterator);
void _$_16ListSafeIterator(unsigned* iterator, int flags);
}

extern "C" int objectIsOffGrid__FP10GameObject(unsigned char* object) {
    using namespace norrath_aoc;
    unsigned iterator[8];
    __16ListSafeIteratorP8ListHeadi(iterator, &offTheGrid, 0);
    for (unsigned char* node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4); node != nullptr;
         node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4)) {
        if (load_pointer(node + 12) == object) {
            _$_16ListSafeIterator(iterator, 2);
            return 1;
        }
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
    return 0;
}
