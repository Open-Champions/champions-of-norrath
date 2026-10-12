#include "../ctors-lava-wraith-detail/detail.hpp"
// __14WoodElfSoldierG6Point3sPPcPc at 0x247400, 944 bytes.
// The point snapshot is taken first, then objectERand runs on the not yet
// constructed object; the scale is (rand % 14 + 4), unsigned remainder, as a
// float. Creature receives the snapshot, the sign-extended type, the resource
// map at 0x42dc40, that scale and the argument list. Byte +40 becomes 40, flags
// gain 0x01000000, the doubleword at +160 gains 0x0000000800040000 (low word
// 0x00040000, high word 8), the word at 0x4844a4 goes to +248, 100.0f to +176,
// the literal 1.2f (0x3f999999) to +168, view 0x44af90 to +72, 20.0f to +28,
// and +236, +572 (half), +960 and +988 are cleared. setParameters receives 2.0f,
// 2.0f, 0.3f (0x3e999999), 0.3f, 1.0f and 1. +792 becomes -1 as a half and +968
// is cleared before the next random draw. A random value mod 3 goes to +964;
// when it was nonzero a second draw mod 3 resets +964 to zero unless that draw
// is zero. The name is then compared case-blind with four texts (0x474540,
// 0x474550, 0x474560, 0x474570): a match stores 0, 1, 2 or 3 at +964, and no
// match keeps the random value. initType runs, the 64-bit modelGetActiveChunks
// (word at model+12, +244) goes to +552, the animation state at +288 is
// initialised and setState(0) runs, and +256 is cleared. For type 3 +236
// becomes 1 and the run list is called, otherwise the slow run list is called.
// Seven setAnimEventFlags calls follow (see the body). finishConstruction runs
// last; low V0 is its machine result.
extern "C" unsigned norrath_construct_WoodElfSoldier(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    const unsigned draw = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const float scale = static_cast<float>(static_cast<int>(draw % 14u + 4u));
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42dc40, scale, arguments);
    const Address a = address(object);
    write_byte(a + 40u, 40u);
    const unsigned low_word = read_word(a + 160u) | 0x00040000u;
    const unsigned high_word = read_word(a + 164u) | 0x8u;
    const unsigned flags = read_word(a + 12u) | 0x01000000u;
    write_word(a + 176u, float_bits(100.0f));
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44af90));
    write_word(a + 28u, float_bits(20.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 12u, flags);
    write_word(a + 960u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 988u, 0u);
    norrath_creature_set_parameters(object, 2.0f, 2.0f, bits_float(0x3e999999u), bits_float(0x3e999999u), 1.0f, 1);
    write_half(a + 792u, 0xffffu);
    write_word(a + 968u, 0u);
    const unsigned pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u & 0xffffu;
    write_word(a + 964u, pick);
    if (pick != 0u && static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u != 0u) {
        write_word(a + 964u, 0u);
    }
    if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_474540)) == 0) {
        write_word(a + 964u, 0u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_474550)) == 0) {
        write_word(a + 964u, 1u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_474560)) == 0) {
        write_word(a + 964u, 2u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_474570)) == 0) {
        write_word(a + 964u, 3u);
    }
    norrath_wood_elf_soldier_init_type(object);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_wood_elf_soldier_set_state(object, 0);
    write_word(a + 256u, 0u);
    if (read_word(a + 964u) == 3u) {
        write_word(a + 236u, 1u);
        (void)norrath_actor_add_run_list(object);
    } else {
        (void)norrath_actor_add_slow_run_list(object);
    }
    const float c03 = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 17, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 24, bits_float(0x3ef5c28fu), 1, bits_float(0x3f8cccccu), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 25, bits_float(0x40499999u), 1, bits_float(0x40799999u), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 34, c03, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 27, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 35, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 38, 0.5f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
