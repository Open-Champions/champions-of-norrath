#include "../ctors-lava-wraith-detail/detail.hpp"
// __10SeaMonsterG6Point3sPPc at 0x1f8088, 532 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42b7e8, scale 10.0f and the argument list. Bytes +40/+41 become 65 and
// 50, view 0x44e2b0 goes to +72, the doubleword at +160 gains 0x0000008000040000
// (high word 0x80, low word 0x00040000), the word at 0x4844a4 goes to +248, the
// literal-pool float 0x3f999999 (1.2f, exactly 0x3f999999) to +168, 35.0f to +28, 1.0f to +224,
// 10.0f to +228, and +176, +192, +236, +256, +572 (half) and +960 are cleared.
// setParameters receives 9.0f, 1.2f (word 0x3f999999), 2.0f, 1.0f, 1.0f and 1. The animation
// state at +288 is initialised and gets the header word at model+60, the slow
// run list is called, and setAnimEventFlags runs for 4, 5 and 6 with 0.3f
// (0x3e999999), 1 and zeros. After finishConstruction, if bit 0 of the low
// word at +160 is set, that word gains 0x20000000, +224 becomes the literal
// float 0x3f933333 (1.15f) and +28 gains 5.0f. Low V0 is that updated low word,
// or zero when the bit is clear; it is not a callee result.
extern "C" unsigned norrath_construct_SeaMonster(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42b7e8, 10.0f, arguments);
    const Address a = address(object);
    write_byte(a + 40u, 65u);
    write_byte(a + 41u, 50u);
    const unsigned low_word = read_word(a + 160u) | 0x00040000u;
    const unsigned high_word = read_word(a + 164u) | 0x80u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(35.0f));
    write_word(a + 224u, float_bits(1.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44e2b0));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 228u, float_bits(10.0f));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 960u, 0u);
    norrath_creature_set_parameters(object, 9.0f, bits_float(0x3f999999u), 2.0f, 1.0f, 1.0f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 60u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    norrath_creature_set_anim_event_flags(object, 4, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, bits_float(0x3e999999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    (void)norrath_creature_finish_construction(object);
    const unsigned flags = read_word(a + 160u);
    if ((flags & 1u) == 0u) return 0u;
    const float raised = bits_float(read_word(a + 28u)) + 5.0f;
    const unsigned updated = flags | 0x20000000u;
    write_word(a + 160u, updated);
    write_word(a + 224u, 0x3f933333u);
    write_word(a + 28u, float_bits(raised));
    return updated;
}
