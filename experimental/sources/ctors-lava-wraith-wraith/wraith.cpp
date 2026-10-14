#include "../ctors-lava-wraith-detail/detail.hpp"
// __6WraithG6Point3sPPc at 0x203cf8, 528 bytes.
// objectERand runs first, on the not yet constructed object. Creature then
// receives a point snapshot, the sign-extended type, the resource map at
// 0x42bcb8, the scale (rand % 14 + 4 as an unsigned remainder, converted from
// int) and the argument list. Stores: byte +40 = 20, view 0x44aea0 at +72,
// floats 1.0f at +96/+100, 0.3f (literal word 0x3e999999) at +104, 1.2f
// (0x3f999999) at +168, 140.0f at +176, 0.0f at +192, 18.0f at +28, 600.0f at
// +788, byte +960 = 100, byte +172 = 16, the word at 0x4844a4 at +248, zeros at
// +236, +572 (half) and +968, and the doubleword at +160 gains 0x0000000400200000.
// setParameters receives 1.4f (0x3fb33333) twice, 1.3f (0x3fa66666), 1.0f twice
// and 1. The animation state at +288 is initialised and gets the header word at
// model+76. The slow run list is called, setAnimEventFlags runs with (3, 0.3f, 1)
// and (5, 0.4f (0x3ecccccc), 1) and zeros, and the object is linked into the
// list head at 0x42bd18 + 4 * count (the word at 0x4852a8), then the count is
// incremented before finishConstruction. Low V0 is finishConstruction's result.
extern "C" unsigned norrath_construct_Wraith(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    const unsigned random = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const float scale = static_cast<float>(static_cast<int>(random % 14u + 4u));
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42bcb8, scale, arguments);
    const Address a = address(object);
    write_word(a + 968u, 0u);
    write_byte(a + 40u, 20u);
    write_word(a + 176u, float_bits(140.0f));
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(18.0f));
    write_byte(a + 960u, 100u);
    write_word(a + 968u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44aea0));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 192u, float_bits(0.0f));
    write_word(a + 100u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    norrath_creature_set_parameters(object, bits_float(0x3fb33333u), bits_float(0x3fb33333u), bits_float(0x3fa66666u), 1.0f, 1.0f, 1);
    write_word(a + 788u, float_bits(600.0f));
    const unsigned high = read_word(a + 164u) | 0x4u;
    const unsigned low_word = read_word(a + 160u) | 0x00200000u;
    write_byte(a + 172u, 16u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 76u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    norrath_creature_set_anim_event_flags(object, 3, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, bits_float(0x3ecccccc), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    const unsigned count = read_word(address(norrath_ctors_lava_wraith_view_4852a8));
    (void)norrath_list_oneway_link(pointer(address(norrath_ctors_lava_wraith_view_42bd18) + (count << 2)), object, object);
    write_word(address(norrath_ctors_lava_wraith_view_4852a8), read_word(address(norrath_ctors_lava_wraith_view_4852a8)) + 1u);
    return norrath_creature_finish_construction(object);
}
