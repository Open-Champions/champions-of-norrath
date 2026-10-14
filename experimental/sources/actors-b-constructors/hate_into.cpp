#include "detail.hpp"
// __8HateIntoPcG6Point3sPPc at 0x226140, 116 bytes.
// After AutoProp: _vt$8HateInto (0x44f150) at object+72, byte 156 cleared,
// animAdd of the header word at object+296 onto the state at object+152 with
// flag 1, then 1 ORed into the word at object+208. Low V0 is animAdd's low V0.
extern "C" unsigned norrath_construct_HateInto(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44f150));
    void* header = pointer(read_word(a + 296u));
    bytes(a)[156] = 0u;
    const unsigned result = norrath_actor_animation_add(pointer(a + 152u), header, 1);
    write_word(a + 208u, read_word(a + 208u) | 1u);
    return result;
}
