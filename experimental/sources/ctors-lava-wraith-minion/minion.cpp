#include "../ctors-lava-wraith-detail/detail.hpp"
// __6MinionG6Point3sPPc at 0x233878, 852 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d138, scale 10.0f and the argument list. Bytes +40 and +41 become 100
// and 50, the doubleword at +160 gains 0x0000000220040000 (low word 0x20040000,
// high word 2), view 0x44f5b8 goes to +72, the literal 1.2f (0x3f999999) to
// +168, 45.0f to +28, the word at 0x4844a4 to +248, the literal 1.3f
// (0x3fa66666) to +224, 1 to +964, 0.5f to +88, +92, +96, +100 and +104, and
// +176, +192, +236, +256, +572 (half), +960, +968, +980 and +984 are cleared.
// setParameters receives 34.0f, 1.4f (0x3fb33333), 2.0f, 4.2f (0x40866666),
// 10.5f and 1. The animation state at +288 is initialised and gets the header
// word at model+108, and the slow run list is called. Eleven setAnimEventFlags
// calls follow (see the body). finishConstruction runs last; low V0 is its
// machine result.
extern "C" unsigned norrath_construct_Minion(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d138, 10.0f, arguments);
    const Address a = address(object);
    const float half = 0.5f;
    write_byte(a + 40u, 100u);
    write_byte(a + 41u, 50u);
    const unsigned low_word = read_word(a + 160u) | 0x20040000u;
    const unsigned high_word = read_word(a + 164u) | 0x2u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(45.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 224u, 0x3fa66666u);
    write_word(a + 964u, 1u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44f5b8));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 100u, float_bits(half));
    write_word(a + 96u, float_bits(half));
    write_word(a + 88u, float_bits(half));
    write_word(a + 92u, float_bits(half));
    write_word(a + 104u, float_bits(half));
    write_word(a + 960u, 0u);
    write_word(a + 968u, 0u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 980u, 0u);
    write_word(a + 984u, 0u);
    norrath_creature_set_parameters(object, 34.0f, bits_float(0x3fb33333u), 2.0f, bits_float(0x40866666u), 10.5f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 108u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float c03 = bits_float(0x3e999999u), c001 = bits_float(0x3c23d70au);
    norrath_creature_set_anim_event_flags(object, 4, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 7, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 3, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 8, 2.0f, 1, 2.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 16, bits_float(0x3f333333u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 15, half, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 17, bits_float(0x3fd33333u), 1, c001, 64, bits_float(0x3fd33333u), 128, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 19, c001, 16, 4.0f, 32, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
