#include "../w9-animation-object-c-common/mem.hpp"
// Observed objectProcessDeleteList__Fv at 0x00143970, 120 bytes, 48-byte frame. It builds a list safe iterator over
// the list head deleteList (0x00486880) with the mask word objectIgnoreFlags (0x00483ac0), read once. While the
// iterator's current node (iterator + 4) exists it takes the object word at node + 12; when that is nonzero it calls
// the virtual slot at byte 8 of the object's vtable word (object + 72) with the adjusted this and the argument 3.
// The adjustment is a signed halfword at the start of the eight-byte slot and the function word is at slot + 4.
// The iterator is then advanced with next__16ListSafeIterator and the node read again; at the end it is destroyed
// with delete flag 2. Nothing is returned.
// Inferred: the argument 3 asks each queued object to run its removal step; the slot is not named in the image.
// Unknown: what slot 8 of the vtable is, and which objects objectIgnoreFlags excludes from the walk.
extern "C" {
extern int objectIgnoreFlags;
extern unsigned char deleteList[];
void __16ListSafeIteratorP8ListHeadi(unsigned* iterator, unsigned char* head, int mask);
void next__16ListSafeIterator(unsigned* iterator);
void _$_16ListSafeIterator(unsigned* iterator, int flags);
}

extern "C" void objectProcessDeleteList__Fv() {
    using namespace norrath_aoc;
    unsigned iterator[8];
    __16ListSafeIteratorP8ListHeadi(iterator, deleteList, objectIgnoreFlags);
    for (unsigned char* node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4); node != nullptr;
         node = load_pointer(reinterpret_cast<unsigned char*>(iterator) + 4)) {
        unsigned char* object = load_pointer(node + 12);
        if (object != nullptr) call_virtual_pointer_int(object, 8, 3);
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
}
