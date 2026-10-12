#include "../ctors-lava-wraith-detail/detail.hpp"
// __8SkelightG6Point3sPPcci at 0x246310, 648 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42dbf0, scale 10.0f and the argument list. View 0x44fdf8 goes to +72.
// Halves at +1092, +1084, +1076 and +1068 become -1; the item description at
// +1032 gets -1 bytes at +1064/+1065 and zero halves at +1048..+1054 and
// +1056..+1062. Byte +40 becomes 20, the low word of the doubleword at +160
// gains 0x00010000, the word at 0x4844a4 goes to +248, the literal float
// 0x3f999999 to +168, 15.0f to +28, 4 to +960, 1.0f to +96 and +100, and +176,
// +192, +236 and +572 (half) are cleared. When the signed-char flag is nonzero,
// objectERand (unsigned, mod 3) + 1 goes to +208. initWeapon runs. The int goes
// to +1104. The float word at 0x484cac is saved, replaced by the int converted
// to float for the duration of setParameters (1.0f five times and 1), then
// restored. +244 is cleared, the 64-bit modelGetActiveChunks (word at model+12,
// 0) goes to +552, the animation state at +288 is initialised and gets the
// header word at model+60 with flag 12, setState(13) and the run list are
// called, and +792 gets -1 as a half. setAnimEventFlags runs for 3 with
// (0.2f (0x3e4ccccc), 1, zeros) and for 4 with (0.2f, 1, 1.0f, 1, zeros). After
// finishConstruction +236 becomes 1. Low V0 is finishConstruction's result.
extern "C" unsigned norrath_construct_Skelight(void* object, const void* point, short type, char** arguments, signed char flag, int value) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42dbf0, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1032u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fdf8));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1092u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 20u);
    const unsigned low_word = read_word(a + 160u) | 0x10000u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(15.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 160u, low_word);
    write_word(a + 960u, 4u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 100u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    if (flag != 0) {
        write_word(a + 208u, static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u + 1u);
    }
    norrath_skelight_init_weapon(object);
    write_word(a + 1104u, static_cast<unsigned>(value));
    const unsigned saved = read_word(address(norrath_creature_view_484cac));
    write_word(address(norrath_creature_view_484cac), float_bits(static_cast<float>(value)));
    norrath_creature_set_parameters(object, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
    write_word(address(norrath_creature_view_484cac), saved);
    write_word(a + 244u, 0u);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u), 0);
    norrath_actor_animation_initialize(bytes(a + 288u));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 60u), 12);
    norrath_skelight_set_state(object, 13);
    (void)norrath_actor_add_run_list(object);
    write_half(a + 792u, 0xffffu);
    const float fifth = bits_float(0x3e4cccccu);
    norrath_creature_set_anim_event_flags(object, 3, fifth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, fifth, 1, 1.0f, 1, 0.0f, 0, 0.0f, 0);
    const unsigned result = norrath_creature_finish_construction(object);
    write_word(a + 236u, 1u);
    return result;
}
