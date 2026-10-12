#include "../ctors-lava-wraith-detail/detail.hpp"
// __9MummyKingG6Point3sPPc at 0x239a88, 732 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d480, scale 10.0f and the argument list. View 0x44f798 goes to +72.
// Halves at +1092, +1084, +1076 and +1068 become -1; the item description at
// +1032 gets -1 bytes at +1064/+1065 and zero halves at +1048..+1054 and
// +1056..+1062. Words +1108..+1120, +1124 and +1136..+1148 are cleared. Bytes
// +40 and +41 become 50, the doubleword at +160 gains 0x0000000820010000 (low
// word 0x20010000, high word 8), the literal 1.2f (0x3f999999) goes to +168,
// 25.0f to +28, the word at 0x4844a4 to +248, the literal -0.2f (0xbe4ccccc) to
// +88, 0.7f (0x3f333333) to +100, 0.6f (0x3f199999) to +96, 1.0f to +224, 14 to
// +960, 0.3f (0x3e999999) to +104, and +176, +192, +236, +572 (half), +256,
// +1104 and +1132 are cleared. setParameters receives 34.0f, 1.4f (0x3fb33333),
// 2.5f, 4.2f (0x40866666), 10.5f and 1. setAnimEventFlags runs for 3 (0.3f, 1,
// 0.9f, 1), 4 (0.3f, 1), 10 (0.8f, 1) and 5 (0.1f, 1). The animation state at
// +288 is initialised and gets the header word at model+68, and the slow run
// list is called; low V0 is its result.
extern "C" unsigned norrath_construct_MummyKing(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d480, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1032u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44f798));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1092u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    for (unsigned word = 0u; word < 4u; ++word) write_word(a + 1108u + 4u * word, 0u);
    write_word(a + 1124u, 0u);
    for (unsigned word = 0u; word < 4u; ++word) write_word(a + 1136u + 4u * word, 0u);
    write_byte(a + 41u, 50u);
    const unsigned low_word = read_word(a + 160u) | 0x20010000u;
    const unsigned high_word = read_word(a + 164u) | 0x8u;
    write_byte(a + 40u, 50u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(25.0f));
    write_word(a + 88u, 0xbe4cccccu);
    write_word(a + 100u, 0x3f333333u);
    write_word(a + 96u, 0x3f199999u);
    write_word(a + 224u, float_bits(1.0f));
    write_word(a + 960u, 14u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 1132u, 0u);
    write_word(a + 1104u, 0u);
    norrath_creature_set_parameters(object, 34.0f, bits_float(0x3fb33333u), 2.5f, bits_float(0x40866666u), 10.5f, 1);
    const float third = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 3, third, 1, bits_float(0x3f666666u), 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, third, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 10, bits_float(0x3f4cccccu), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, bits_float(0x3dccccccu), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 68u), 1);
    return norrath_actor_add_slow_run_list(object);
}
