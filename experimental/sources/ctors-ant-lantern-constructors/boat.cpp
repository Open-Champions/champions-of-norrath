#include "detail.hpp"
// __4BoatG6Point3sPPc at 0x2bfa20, 208 bytes.
// Vehicle receives a point snapshot, the sign-extended short, the argument list,
// and integer 1. _vt$4Boat (0x453328) is stored at object+72. worldCheckForWater
// receives the pointer loaded from `world` (0x4844a0), a snapshot of the 12 bytes
// at object+16, the immediate single 32, and a pointer to object+24. Those 12
// bytes are then copied to object+1632. Bit 0x00100000 is cleared in object+12
// before Reset__7Vehiclei(object, 0). Low V0 is Reset's low V0.
extern "C" unsigned norrath_construct_Boat(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_Vehicle(object, &local, type, arguments, 1);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_453328));
    const PointBytes position = snapshot_point(pointer(a + 16u));
    (void)norrath_world_check_for_water(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                       &position, 32.0f, reinterpret_cast<float*>(pointer(a + 24u)));
    write_word(a + 1632u, read_word(a + 16u));
    write_word(a + 1636u, read_word(a + 20u));
    write_word(a + 1640u, read_word(a + 24u));
    write_word(a + 12u, read_word(a + 12u) & 0xffefffffu);
    return norrath_vehicle_reset(object, 0);
}
