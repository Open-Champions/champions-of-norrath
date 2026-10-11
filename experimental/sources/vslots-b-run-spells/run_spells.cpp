#include "../vslots-b-bytes/bytes.hpp"
// Observed runSpells__8CreatureG6Point3 at 0x113978, 180 bytes.
// Twelve bytes from the point are copied, then the safe iterator is built on
// the list at object+888 with mask 0. Each selected node's word at +12 is an
// element. That element's word at +20 is a table; slot byte 16 is called with
// the copied point and the signed halfword at object+56. The iterator is
// destroyed with flag 2 even when the first selection is empty.
// The three iterator names are the existing safe-iterator bodies.
extern "C" void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
extern "C" void norrath_list_safe_iterator_next(unsigned char* iterator);
extern "C" void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
extern "C" void runSpells__8CreatureG6Point3(void* self, const void* point) {
    using namespace vslots_b;
    unsigned char point_copy[12];
    const unsigned char* incoming = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) point_copy[i] = incoming[i];
    alignas(8) unsigned char iterator[32];
    norrath_list_safe_iterator_construct(iterator, static_cast<unsigned char*>(self) + 888u, 0);
    unsigned current_bits = read_word(address(iterator) + 4u);
    while (current_bits != 0u) {
        const Address element = address(pointer(read_word(address(pointer(current_bits)) + 12u)));
        const Address slot = address(pointer(read_word(element + 20u))) + 16u;
        const short adjustment = read_i16(slot);
        using SpellCall = void (*)(void*, void*, int);
        SpellCall call = reinterpret_cast<SpellCall>(pointer(read_word(slot + 4u)));
        call(static_cast<unsigned char*>(pointer(element)) + adjustment, point_copy, read_i16(address(self) + 56u));
        norrath_list_safe_iterator_next(iterator);
        current_bits = read_word(address(iterator) + 4u);
    }
    norrath_list_safe_iterator_destroy(iterator, 2);
}
