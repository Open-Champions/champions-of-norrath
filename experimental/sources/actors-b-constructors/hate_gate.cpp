#include "detail.hpp"
// __8HateGatePcG6Point3sPPc at 0x2261b8, 96 bytes.
// Same AutoProp point snapshot as HackDoor. Afterwards _vt$8HateGate
// (0x44f0a8) is stored at object+72 and 1 is ORed into the word at object+208.
// Low V0 remains AutoProp's low V0.
extern "C" unsigned norrath_construct_HateGate(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    const unsigned result = norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44f0a8));
    write_word(a + 208u, read_word(a + 208u) | 1u);
    return result;
}
