#include "detail.hpp"
// __8HackDoorPcG6Point3sPPc at 0x2264e8, 96 bytes.
// Observed: the 12 point bytes are copied before __8AutoPropPcG6Point3sPPc.
// The short is sign-extended and the name, object, and argument list are passed
// through. After that call, _vt$8HackDoor is stored at object+72 and 1 is ORed
// into the word at object+12. Low V0 is AutoProp's low V0.
extern "C" unsigned norrath_construct_HackDoor(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    const unsigned result = norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_44ee08));
    write_word(a + 12u, read_word(a + 12u) | 1u);
    return result;
}
