#include "detail.hpp"
// __5DemonG6Point3sPPc at 0x1e9bc0, 740 bytes.
// Observed: Creature receives the point snapshot, the short, the table at 0x42af70
// as its map, the single 10.0 and the argument list. _vt$5Demon (0x44dd68) goes
// to object+72. Halves at 1092, 1084, 1076 and 1068 are 0xffff; bytes 1064 and
// 1065 are 0xff; the 16 bytes from 1048 to 1063 are zero. Bit 0x00040000 is ORed
// into the low word at +160 and 0x2 into the high word at +164. Singles: 20.0 to
// +28, -0.25 to +92, 1.0 to +88 and +224, the global at 0x480ba8 to +168. The
// word at 0x4844a4 goes to +248; +176, +192, +236, +256, +964 and half 572 are
// cleared. eRand is scaled by 2^-16 as a signed integer; below 0.5 a second eRand
// stores (value & 3) + 1 at +208. initAttackVariant__5Demon runs, 17 goes to +960
// and setParameters gets the globals at 0x480bac and 0x480bb0, three 1.0 values
// and 1. The animation state at +288 is initialised and receives the header word
// at model+68 (flag 1); the object joins the slow run list. Five calls to
// setAnimEventFlags follow (indexes 3, 4, 5, 14, 15) and V0 is
// finishConstruction's.
extern "C" unsigned norrath_construct_Demon(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42af70, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44dd68));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1092u - index * 8u, 0xffffu);
    bytes(a)[1065] = 0xffu;
    bytes(a)[1064] = 0xffu;
    for (unsigned offset = 1048u; offset < 1064u; offset += 2u) write_half(a + offset, 0u);
    write_word(a + 160u, read_word(a + 160u) | 0x00040000u);
    write_word(a + 164u, read_word(a + 164u) | 0x00000002u);
    write_word(a + 28u, 0x41a00000u);
    write_word(a + 92u, 0xbe800000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480ba8)));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 88u, 0x3f800000u);
    const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
    if (roll < 0.5f) write_word(a + 208u, (norrath_actor_random(object) & 3u) + 1u);
    initAttackVariant__5Demon(object);
    void* const state = pointer(a + 288u);
    write_word(a + 224u, 0x3f800000u);
    write_word(a + 960u, 17u);
    (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_480bac),
                                       read_single(norrath_ctors_ant_lantern_view_480bb0), 1.0f, 1.0f, 1.0f, 1);
    norrath_actor_animation_initialize(state);
    const float lead = read_single(norrath_ctors_ant_lantern_view_480bb4);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 68u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 1, 0.5f, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 1, 0.5f, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 14, 1.0f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 15, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
