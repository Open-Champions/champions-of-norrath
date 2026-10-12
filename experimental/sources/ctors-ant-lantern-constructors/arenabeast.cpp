#include "detail.hpp"
// __10ArenabeastG6Point3sPPc at 0x235478, 888 bytes.
// Observed: Creature receives the snapshot, the short, the table at 0x42d200 as
// its map, the single 10.0 and the argument list. Bytes 40 and 41 are 85 and 80,
// bits 0x20040000 go into the low word at +160 and 2 into the high word at +164,
// 50.0 to +28, the singles at 0x481ab8 and 0x481abc to +104 and +96, 1.0 to +224,
// the word at 0x4844a4 to +248, _vt$10Arenabeast (0x44f6a8) to +72, the single at
// 0x481ab4 to +168, and +176, +192, +236, +256, +228 and half 572 are cleared.
// setParameters receives 60.0, the single at 0x481ac0, 2.0, the single at
// 0x481ac4, 10.5 and 1. The animation state at +288 is initialised and receives
// the header word at model+140 with flag 1; the object joins the slow run list.
// Eleven setAnimEventFlags calls follow (indexes 5, 6, 7, 8, 10, 11, 12, then 9
// with a2 = 17, 3 and 4 with a2 = 16, and 13 with a2 = 16). Word 964 is the word
// at +808 plus (eRand % 300) plus 300, stored before finishConstruction, whose V0
// is returned.
extern "C" unsigned norrath_construct_Arenabeast(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const float lead = read_single(norrath_ctors_ant_lantern_view_481ab4);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42d200, 10.0f, arguments);
    const Address a = address(object);
    bytes(a)[40] = 85u;
    bytes(a)[41] = 80u;
    write_word(a + 160u, read_word(a + 160u) | 0x20040000u);
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 28u, 0x42480000u);
    write_word(a + 104u, read_word(address(norrath_ctors_ant_lantern_view_481ab8)));
    write_word(a + 96u, read_word(address(norrath_ctors_ant_lantern_view_481abc)));
    write_word(a + 224u, 0x3f800000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44f6a8));
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_481ab4)));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 228u, 0u);
    (void)norrath_actor_set_parameters(object, 60.0f, read_single(norrath_ctors_ant_lantern_view_481ac0), 2.0f,
                                       read_single(norrath_ctors_ant_lantern_view_481ac4), 10.5f, 1);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 140u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 7, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 10, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, read_single(norrath_ctors_ant_lantern_view_481ac8), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 12, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 9, lead, 17, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 13, 2.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    const unsigned roll = norrath_actor_random(object);
    write_word(a + 964u, roll % 300u + read_word(a + 808u) + 300u);
    return norrath_actor_finish_construction(object);
}
