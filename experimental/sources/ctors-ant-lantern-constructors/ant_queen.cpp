#include "detail.hpp"
// __8AntQueenG6Point3sPPc at 0x206c08, 672 bytes.
// Observed: eRand is called first; the Creature scale is (eRand % 14) + 4 as a
// single. Creature receives the snapshot, the short, the table at 0x42bf58 as its
// map and the argument list. Byte 40 is 55, bits 0x20001000 go into the low word
// at +160 and 0x80 into the high word at +164, 140.0 goes to +176, the singles at
// 0x4812e0 and 0x4812e4 to +168 and +88, 45.0 to +28, the word at 0x4844a4 to +248,
// _vt$8AntQueen (0x44c440) to +72, 0.0 to +192, 180 to +972, byte 172 is 19 and
// +236 and half 572 are cleared. A second eRand r gives +976 = (word at +808) +
// (r % 180) + 180. setParameters receives 48.0, the single at 0x4812e8, 4.0, the
// single at 0x4812ec, 10.5 and 1. Word 244 is cleared, the eight-byte
// modelGetActiveChunks(model->+12, 0) is stored at +552, the animation state at
// +288 is initialised and receives the header word at model+124 with flag 1, and
// the object joins the slow run list. Five setAnimEventFlags calls follow (indexes
// 3, 4, 5, 6, 19) and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_AntQueen(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const float low = read_single(norrath_ctors_ant_lantern_view_4812d8);
    const unsigned first_roll = norrath_actor_random(object);
    const float lead = read_single(norrath_ctors_ant_lantern_view_4812dc);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42bf58,
                                     static_cast<float>(static_cast<int>(first_roll % 14u + 4u)), arguments);
    const Address a = address(object);
    bytes(a)[40] = 55u;
    write_word(a + 176u, 0x430c0000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_4812e0)));
    write_word(a + 28u, 0x42340000u);
    write_word(a + 88u, read_word(address(norrath_ctors_ant_lantern_view_4812e4)));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44c440));
    write_word(a + 160u, read_word(a + 160u) | 0x20001000u);
    write_word(a + 164u, read_word(a + 164u) | 0x80u);
    write_word(a + 192u, 0u);
    write_word(a + 972u, 180u);
    bytes(a)[172] = 19u;
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    const unsigned second_roll = norrath_actor_random(object);
    write_word(a + 976u, read_word(a + 808u) + second_roll % 180u + 180u);
    (void)norrath_actor_set_parameters(object, 48.0f, read_single(norrath_ctors_ant_lantern_view_4812e8), 4.0f,
                                       read_single(norrath_ctors_ant_lantern_view_4812ec), 10.5f, 1);
    write_word(a + 244u, 0u);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(address(model) + 12u)), 0);
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 124u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 3, low, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 19, low, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
