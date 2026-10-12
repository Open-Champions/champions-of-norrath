#include "../ctors-lava-wraith-detail/detail.hpp"
// __7UlthorkG6Point3sPPc at 0x1f5050, 776 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42b478, scale 10.0f and the argument list. View 0x44dfe0 goes to +72.
// Halves at +1092, +1084, +1076 and +1068 become -1; the item description at
// +1032 gets -1 bytes at +1064/+1065 and zero halves at +1048, +1050, +1052,
// +1054 and +1056..+1062. Byte +40 becomes 90, the doubleword at +160 gains
// 0x0000000200040000 (low word 0x00040000, high word 2), the literal float
// 0x3f999999 goes to +168, the word at 0x4844a4 to +248, 35.0f to +28, 1.0f to
// +92, the literal -0.3f (0xbe999999) to +88, and +176, +192, +236 and +256 and
// +572 (half) are cleared. A first objectERand, as a signed int, times 2^-16,
// times 0.3f (0x3e999999), plus 0.8f (0x3f4ccccc), goes to +224. A second
// objectERand, unsigned mod 3, goes to byte +1104. Remainder 2 sets byte +156
// to 6, +960 to 23 and +244 to 1; otherwise +244 is 0 and a third objectERand
// sets +960 to 19 + 2 * (low bit). setParameters receives 8.0f, 1.2f (word
// 0x3f999999), 2.0f, 1.0f, 1.0f and 1. The 64-bit modelGetActiveChunks (word
// at model+12, word at +244) goes to +552. The animation state at +288 is
// initialised and gets the header word at model+116, the slow run list is
// called, and setAnimEventFlags runs for 5, 6 and 7 with (0.2f, 1, 0.4f, 2,
// zeros) and for 18 with (0.3f, 1, zeros). Low V0 is finishConstruction's
// machine result.
extern "C" unsigned norrath_construct_Ulthork(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42b478, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1032u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44dfe0));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1092u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 90u);
    const unsigned low_word = read_word(a + 160u) | 0x00040000u;
    const unsigned high_word = read_word(a + 164u) | 0x2u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 28u, float_bits(35.0f));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 92u, float_bits(1.0f));
    write_word(a + 88u, 0xbe999999u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    float spread = static_cast<float>(norrath_game_object_e_rand(object));
    spread = spread * bits_float(0x37800000u);
    spread = spread * bits_float(0x3e999999u);
    spread = spread + bits_float(0x3f4cccccu);
    write_word(a + 224u, float_bits(spread));
    const unsigned pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u;
    write_word(a + 244u, 0u);
    write_byte(a + 1104u, pick);
    if (pick == 2u) {
        write_byte(a + 156u, 6u);
        write_word(a + 960u, 23u);
        write_word(a + 244u, 1u);
    } else {
        write_word(a + 960u, ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) << 1) + 19u);
    }
    norrath_creature_set_parameters(object, 8.0f, bits_float(0x3f999999u), 2.0f, 1.0f, 1.0f, 1);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 116u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float short_gain = bits_float(0x3e4ccccc), long_gain = bits_float(0x3ecccccc);
    norrath_creature_set_anim_event_flags(object, 5, short_gain, 1, long_gain, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, short_gain, 1, long_gain, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 7, short_gain, 1, long_gain, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 18, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
