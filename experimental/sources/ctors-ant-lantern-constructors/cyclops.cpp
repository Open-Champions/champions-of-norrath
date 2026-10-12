#include "detail.hpp"
// __7CyclopsG6Point3sPPci at 0x202678, 912 bytes.
// Observed: eRand is called first; the scale handed to Creature is the integer
// (eRand % 14) + 4 converted to a single. Creature also receives the point
// snapshot, the short, the 216-byte record at 0x42bb08 + 216 * the int argument
// as its map, and the argument list. Byte 40 is 55 and _vt$7Cyclops (0x44bf90) is
// stored at +72. Bit 0x20040000 is ORed into the low word at +160 and 0x2 into the
// high word at +164. The global single at 0x481160 goes to +168, 35.0 to +28 and
// the word at 0x4844a4 to +248. +176, +236, +960, +244 and half 572 are cleared.
// setParameters gets the singles at 0x481164, 0x481168 and 0x48116c, 1.0, 1.0 and
// 1. A second eRand fills +244: bits 1, 2, 4 and 8 of the roll set those bits in
// order; bit 0x10 sets 0x10, but when it is clear the word is REPLACED by 0x20
// (the original ORs 0x20 into an already-zero register, so earlier bits are
// lost). The 64-bit modelGetActiveChunks(model->+12, word at +244) is stored at
// +552; the animation state at +288 is initialised with the header word at
// model+108 (flag 1); the object joins the slow run list; eight setAnimEventFlags
// calls (indexes 3, 4, 5, 6, 8, 9, 16, 18) precede finishConstruction, whose V0
// is returned.
extern "C" unsigned norrath_construct_Cyclops(void* object, const void* point, short type, char** arguments, int kind) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned roll = norrath_actor_random(object);
    const float scale = static_cast<float>(static_cast<int>(roll % 14u + 4u));
    (void)norrath_creature_construct(object, &local, type,
                                     pointer(address(norrath_ctors_ant_lantern_view_42bb08) + static_cast<unsigned>(kind) * 216u),
                                     scale, arguments);
    const Address a = address(object);
    bytes(a)[40] = 55u;
    write_word(a + 160u, read_word(a + 160u) | 0x20040000u);
    write_word(a + 164u, read_word(a + 164u) | 0x00000002u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_481160)));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bf90));
    write_word(a + 28u, 0x420c0000u);
    write_word(a + 176u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 960u, 0u);
    (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_481164),
                                       read_single(norrath_ctors_ant_lantern_view_481168),
                                       read_single(norrath_ctors_ant_lantern_view_48116c), 1.0f, 1.0f, 1);
    write_word(a + 244u, 0u);
    const unsigned bits = norrath_actor_random(object);
    if ((bits & 1u) != 0u) write_word(a + 244u, read_word(a + 244u) | 1u);
    if ((bits & 2u) != 0u) write_word(a + 244u, read_word(a + 244u) | 2u);
    if ((bits & 4u) != 0u) write_word(a + 244u, read_word(a + 244u) | 4u);
    if ((bits & 8u) != 0u) write_word(a + 244u, read_word(a + 244u) | 8u);
    write_word(a + 244u, (bits & 0x10u) != 0u ? read_word(a + 244u) | 0x10u : 0x20u);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    const float lead = read_single(norrath_ctors_ant_lantern_view_481170);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 108u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, 0.5f, 1, 1.5f, 2, 2.5f, 4, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, read_single(norrath_ctors_ant_lantern_view_481174), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 9, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 16, read_single(norrath_ctors_ant_lantern_view_481178), 4,
                                          read_single(norrath_ctors_ant_lantern_view_48117c), 8, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 18, 3.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
