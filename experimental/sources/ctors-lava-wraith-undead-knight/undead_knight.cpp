#include "../ctors-lava-wraith-detail/detail.hpp"
// __12UndeadKnightG6Point3sPPc at 0x23c330, 772 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d578, scale 10.0f and the argument list. +960 and +964 are cleared and
// view 0x44fc18 goes to +72. Halves at +1100, +1092, +1084 and +1076 become -1;
// the item description at +1040 gets -1 bytes at +1072/+1073 and zero halves at
// +1056..+1062 and +1064..+1070. Byte +40 becomes 20, the doubleword at +160
// gains 0x0000002020020000 (low word 0x20020000, high word 0x20), the literal
// 1.2f (0x3f999999) goes to +168, byte +172 becomes 16, the word at 0x4844a4 to
// +248, 20.0f to +28, 1 to +968 and +1112, the literal -0.2f (0xbe4ccccc) to
// +88, the literal 1.3f (0x3fa66666) to +224, 1.0f to +96 and +100, 0.3f
// (0x3e999999) to +104, and +176, +192, +236 and +572 (half) are cleared.
// setParameters receives 24.0f, 1.4f (0x3fb33333), 1.95f (0x3ff99999), 4.2f
// (0x40866666), 10.5f and 1. The animation state at +288 is initialised and gets
// the header word at model+84, and the slow run list is called. Six
// setAnimEventFlags calls follow (see the body). finishConstruction runs last;
// low V0 is its machine result.
extern "C" unsigned norrath_construct_UndeadKnight(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d578, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1040u;
    write_word(a + 960u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fc18));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1100u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 20u);
    const unsigned low_word = read_word(a + 160u) | 0x20020000u;
    const unsigned high_word = read_word(a + 164u) | 0x20u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(20.0f));
    write_word(a + 1112u, 1u);
    write_word(a + 88u, 0xbe4cccccu);
    write_word(a + 224u, 0x3fa66666u);
    write_word(a + 968u, 1u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_byte(a + 172u, 16u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 100u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    norrath_creature_set_parameters(object, 24.0f, bits_float(0x3fb33333u), bits_float(0x3ff99999u), bits_float(0x40866666u), 10.5f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 84u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float c02 = bits_float(0x3e4cccccu), c06 = bits_float(0x3f199999u), c03 = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 3, c02, 1, c06, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, c02, 1, c06, 2, 1.0f, 2, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, c03, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, c03, 1, bits_float(0x3f4cccccu), 1, bits_float(0x3fccccccu), 4, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 8, 2.0f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
