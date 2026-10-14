#include "detail.hpp"
// __10TeleporterG6Point3sPPc at 0x21ce00, 120 bytes.
// Scale is the single loaded from 0x4816d4. Creature receives a point snapshot,
// the sign-extended short, a null resource map, that scale, and the argument list.
// Then half 572 is cleared, _vt$10Teleporter (0x448e30) is stored at object+72,
// and 0xf is ORed into object+12. Low V0 is the slow-run-list result.
extern "C" unsigned norrath_construct_Teleporter(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_actors_b;
    const float scale = read_float(address(norrath_actors_b_view_4816d4));
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, nullptr, scale, arguments);
    const Address a = address(object);
    write_half(a + 572u, 0u);
    write_word(a + 72u, address_bits(norrath_actors_b_view_448e30));
    write_word(a + 12u, read_word(a + 12u) | 0xfu);
    return norrath_actor_add_slow_run_list(object);
}
