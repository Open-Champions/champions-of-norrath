#include "../ctors-lava-wraith-detail/detail.hpp"
// __11SpiderQueenG6Point3sPPc at 0x20b2f0, 1024 bytes.
// The point snapshot is taken first. objectERand then runs twice (on the not
// yet constructed object); the scale is ((first & 7) + (second & 7) + 32),
// converted from int. Creature receives the snapshot, the sign-extended type,
// the resource map at 0x42c228, that scale and the argument list. View 0x44b350
// goes to +72, the Web constructor runs on +968 with (41, 240), and the low
// half of the type goes to +56 after it returns. Byte +40 becomes 100, the doubleword at +160
// gains 0x0000008020002000 (low word 0x20002000, high word 0x80), the word at
// 0x4844a4 goes to +248, byte +172 becomes 19, 120.0f goes to +176, the literal
// 0.7f (0x3f333333) to +168, 70.0f to +28, 2000.0f to +788, -50.0f to +228, 1.0f
// to +96 and +224, and +236, +572 (half) and +964 are cleared. setParameters
// receives 68.0f, 1.4f (0x3fb33333), 4.0f, 4.2f (0x40866666), 10.5f and 1.
// Words +1028..+1056 are cleared. The animation state at +288 is initialised,
// gets the header word at model+92 and its time is randomised. The 64-bit
// modelGetActiveChunks (word at model+12, 0) is stored at +552 right before the
// slow run list call. Ten setAnimEventFlags calls follow (see the body).
// finishConstruction runs last; low V0 is its machine result.
extern "C" unsigned norrath_construct_SpiderQueen(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    const unsigned first = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const unsigned second = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const float scale = static_cast<float>(static_cast<int>((first & 7u) + (second & 7u) + 32u));
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42c228, scale, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b350));
    norrath_web_construct(bytes(a + 968u), 41, 240);
    write_half(a + 56u, static_cast<unsigned>(type));
    write_byte(a + 40u, 100u);
    const unsigned low_word = read_word(a + 160u) | 0x20002000u;
    const unsigned high_word = read_word(a + 164u) | 0x80u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_byte(a + 172u, 19u);
    write_word(a + 176u, float_bits(120.0f));
    write_word(a + 168u, 0x3f333333u);
    write_word(a + 28u, float_bits(70.0f));
    write_word(a + 788u, float_bits(2000.0f));
    write_word(a + 228u, float_bits(-50.0f));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 224u, float_bits(1.0f));
    write_word(a + 964u, 0u);
    norrath_creature_set_parameters(object, 68.0f, bits_float(0x3fb33333u), 4.0f, bits_float(0x40866666u), 10.5f, 1);
    for (unsigned word = 0u; word < 8u; ++word) write_word(a + 1056u - 4u * word, 0u);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 92u), 1);
    norrath_actor_animation_randomize_time(bytes(a + 288u));
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u), 0);
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    (void)norrath_actor_add_slow_run_list(object);
    const float c03 = bits_float(0x3e999999u), c13 = bits_float(0x3fa66666u), c02 = bits_float(0x3e4cccccu), c15 = 1.5f;
    norrath_creature_set_anim_event_flags(object, 16, c03, 1, 0.5f, 1, bits_float(0x3f333333u), 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 17, c13, 1, 2.5f, 1, bits_float(0x40266666u), 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 12, c02, 1, bits_float(0x3f666666u), 2, bits_float(0x3f8cccccu), 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 21, bits_float(0x3eccccccu), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 13, 0.5f, 1, c15, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, 0.5f, 1, c15, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 15, 0.5f, 1, c15, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, c02, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, c03, 1, c13, 2, 2.0f, 4, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, 6.0f, 8, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
