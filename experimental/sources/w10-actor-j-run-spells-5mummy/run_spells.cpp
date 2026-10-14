// Observed runSpells__5MummyG6Point3 at 0x00239988, 256 bytes. The Point3 argument is copied to the
// stack and never read again. Order of events: modelGetCharAttachmentPos(model = word +12 of the
// block at object+36, copy of the point at +16, halfword +56, animation state at +288, 0, an
// output point, null, 1.0) fills a local point; then a ListSafeIterator over the list head at
// object+888 (mask 0) visits each node: when the object word at node+12 is non-zero, the entry at
// (that object's word +20) + 16 is called on the adjusted object with (the local point, the
// signed halfword +56). The iterator is destroyed with flag 2. The meaning of the local point
// and of the list is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void __16ListSafeIteratorP8ListHeadi(void* iterator, void* head, int mask);
void next__16ListSafeIterator(void* iterator);
void _$_16ListSafeIterator(void* iterator, int flags);

void runSpells__5MummyG6Point3(void* self, Point3) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 attach;
    unsigned char* const block = from_word(word(o, 36));
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        from_word(word(block, 12)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &attach, nullptr, 1.0f);
    unsigned char iterator[32];
    __16ListSafeIteratorP8ListHeadi(iterator, o + 888, 0);
    for (unsigned node = word(iterator, 4); node != 0u; node = word(iterator, 4)) {
        const unsigned spell = word(from_word(node), 12);
        if (spell != 0u) call_entry_from(from_word(spell), 20, 16, to_word(&attach), static_cast<unsigned long>(static_cast<int>(half(o, 56))));
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
}
}
