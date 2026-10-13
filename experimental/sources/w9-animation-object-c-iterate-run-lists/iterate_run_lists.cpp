#include "../w9-animation-object-c-common/mem.hpp"
// Observed objectIterateRunLists__FPFP10GameObject_v at 0x0013ede8, 324 bytes. Calls the
// given function once for every object on three groups of run lists, in this order:
// runList (0x486878), then the fifteen one-word heads of mediumRunList (0x4fbf90, index 0
// to 14), then the sixty one-word heads of slowRunList (0x4fbfd0, index 0 to 59). Each
// head is walked with its own list safe iterator built with the mask word
// objectIgnoreFlags (0x483ac0): construct, then while the iterator's current node
// (iterator+4) exists call the function with the object word at node+12 and advance,
// then destroy with delete flag 2. The function's result is not used. Each head's
// iterator is destroyed before the next head is constructed; the mask word is read again
// for every head.
// Inferred: objects are called with the node's stored object pointer unchanged (no vtable
// adjustment); the function type is void (the mangled name ends in _v).
// Unknown: which objects the mask excludes (see the iterator constructor).
extern "C" {
extern int objectIgnoreFlags;
extern unsigned char runList[];
extern unsigned char mediumRunList[];
extern unsigned char slowRunList[];
void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char* head, int mask);
void next__16ListSafeIterator(unsigned* iterator);
void _$_16ListSafeIterator(unsigned* iterator, int flags);
}

namespace {
inline void walk(unsigned* iterator, unsigned char* head, void (*callback)(unsigned char*)) {
    using namespace norrath_aoc;
    __16ListSafeIteratorP8ListHeadi(iterator, head, objectIgnoreFlags);
    for (unsigned char* node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4); node != nullptr;
         node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4)) {
        callback(load_pointer(node + 12));
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
}
}

extern "C" void objectIterateRunLists__FPFP10GameObject_v(void (*callback)(unsigned char*)) {
    unsigned run[8];
    unsigned medium[8];
    unsigned slow[8];
    walk(run, runList, callback);
    for (int i = 0; i <= 14; ++i) walk(medium, mediumRunList + i * 4, callback);
    for (int i = 0; i <= 59; ++i) walk(slow, slowRunList + i * 4, callback);
}
