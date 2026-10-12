#include "../ctors-lava-wraith-detail/detail.hpp"
// __7VampireG6Point3sPPc at 0x23fa78, 552 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d650, scale 10.0f and the argument list. The doubleword at +160 gains
// 0x0000000400000000 (high word 4). View 0x44b080 goes to +72, the word at
// 0x4844a4 to +248, the literal float 0x3f333333 to +168, byte +172 = 16, 18.0f
// to +28, the literal -0.2f (0xbe4cccccu) to +88, 0.3f (0x3e999999) to +104,
// 1.0f to +96 and +100, and +176, +236 and +572 (half) are cleared. setParameters
// receives 0.8f (0x3f4ccccc), 1.0f, 0.8f, 1.0f, 1.0f and 1. The animation state
// at +288 is initialised, gets the header word at model+124 and its time is
// randomised. The slow run list is called and setAnimEventFlags runs for 3, 4,
// 5 and 5 again with 0.1f (0x3dcccccc), 1 and zeros. objectERand & 7 selects a
// bit that is ORed into +244, the 64-bit modelGetActiveChunks (word at
// model+12, +244) goes to +552, then finishConstruction runs; low V0 is its
// machine result.
extern "C" unsigned norrath_construct_Vampire(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d650, 10.0f, arguments);
    const Address a = address(object);
    const unsigned low_word = read_word(a + 160u);
    const unsigned high_word = read_word(a + 164u) | 0x4u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 168u, 0x3f333333u);
    write_word(a + 28u, float_bits(18.0f));
    write_word(a + 88u, 0xbe4cccccu);
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 100u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b080));
    write_byte(a + 172u, 16u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 176u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    norrath_creature_set_parameters(object, bits_float(0x3f4cccccu), 1.0f, bits_float(0x3f4cccccu), 1.0f, 1.0f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 124u), 1);
    norrath_actor_animation_randomize_time(bytes(a + 288u));
    (void)norrath_actor_add_slow_run_list(object);
    const float tenth = bits_float(0x3dccccccu);
    norrath_creature_set_anim_event_flags(object, 3, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, tenth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    const unsigned bit = 1u << (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 7u);
    const unsigned mask = read_word(a + 244u) | bit;
    write_word(a + 244u, mask);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(mask));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    return norrath_creature_finish_construction(object);
}
