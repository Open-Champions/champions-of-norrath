#include "detail.hpp"
// __11LavaTractorG6Point3sPPc at 0x284b88, 128 bytes.
// The short is sign-extended. Vehicle receives a point snapshot, that short,
// the argument list, and integer 0. _vt$11LavaTractor (0x450250) is stored at
// object+72. Low V0 is Reset__7Vehiclei(object, 0).
extern "C" unsigned norrath_construct_LavaTractor(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_actors_b;
    const PointBytes local = snapshot_point(point);
    (void)norrath_construct_Vehicle(object, &local, type, arguments, 0);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_actors_b_view_450250));
    return norrath_vehicle_reset(object, 0);
}
