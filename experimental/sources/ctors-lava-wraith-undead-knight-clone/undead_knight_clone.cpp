#include "../ctors-lava-wraith-detail/detail.hpp"
// __17UndeadKnightCloneG6Point3sPPc at 0x23d798, 1004 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d578 (the one UndeadKnight uses), scale 10.0f and the argument list.
// View 0x44fb28 goes to +72. Two empty counting loops follow in the original
// and store nothing. A light record at +2496 is filled in place: zero bytes at
// +2504/+2505, halves 255 at +2506 and +2514, 128 at +2508, +2510, +2516 and
// +2518, -16 at +2512 and +2520, X// 0x3891a2b3 at +2536 and +2532, zero at +2540; the point snapshot is copied to
// +2544 and lightAdd receives +2496. The item description at +2624 gets -1
// halves at +2684, +2676, +2668 and +2660, -1 bytes at +2656/+2657 and zero
// halves at +2640..+2646 and +2648..+2654. Byte +40 becomes 20, the doubleword
// at +160 gains 0x0000022020020000 (low word 0x20020000, high word 0x220), byte
// +172 becomes 16, the literal 1.2f (0x3f999999) goes to +168, the word at
// 0x4844a4 to +248, 20.0f to +28, 1 to +2556 and +2696, the literal -0.2f to
// +88, 0.6f (0x3f199999) to +96, 1.0f to +100, 0.3f (0x3e999999) to +104, the
// literal 1.3f (0x3fa66666) to +224, and +176, +192, +236 and +572 (half) are
// cleared. The text at levelName+81 is compared (strcmp) with the text at
// 0x4858b8. Equal: setParameters(1.4f, 1.2f, 1.0f, 1.0f, the literal 0.01f
// (0x3c23d70a), 1). Different: setParameters(1.4f, 1.2f, 1.0f, 1.0f, 1.2f, 1).
// The animation state at +288 is initialised and gets the header word at
// model+84; the slow run list is called. Five setAnimEventFlags calls follow.
// finishConstruction runs last; low V0 is its machine result.
extern "C" unsigned norrath_construct_UndeadKnightClone(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d578, 10.0f, arguments);
    const Address a = address(object);
    const Address light = a + 2496u;
    const Address item = a + 2624u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fb28));
    const PointBytes snapshot = snapshot_point(&local);
    write_byte(light + 9u, 0u);
    write_point(a + 2544u, snapshot);
    write_half(light + 18u, 255u);
    write_half(light + 22u, 128u);
    write_half(light + 10u, 255u);
    write_half(light + 12u, 128u);
    write_half(light + 14u, 128u);
    write_half(light + 20u, 128u);
    write_word(light + 36u, float_bits(120.0f));
    write_word(light + 40u, 0x3891a2b3u);
    write_half(light + 24u, 0xfff0u);
    write_word(light + 28u, float_bits(120.0f));
    write_word(light + 32u, 0x3891a2b3u);
    write_word(light + 44u, 0u);
    write_half(light + 16u, 0xfff0u);
    write_byte(light + 8u, 0u);
    norrath_light_add(pointer(light));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 2684u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 20u);
    const unsigned low_word = read_word(a + 160u) | 0x20020000u;
    const unsigned high_word = read_word(a + 164u) | 0x220u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 28u, float_bits(20.0f));
    write_byte(a + 172u, 16u);
    write_word(a + 2696u, 1u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 88u, 0xbe4cccccu);
    write_word(a + 96u, 0x3f199999u);
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 224u, 0x3fa66666u);
    write_word(a + 176u, 0u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 2556u, 1u);
    write_word(a + 100u, float_bits(1.0f));
    const float c12 = bits_float(0x3f999999u), c14 = bits_float(0x3fb33333u);
    if (strcmp(levelName + 81, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4858b8)) == 0) {
        norrath_creature_set_parameters(object, c14, c12, 1.0f, 1.0f, bits_float(0x3c23d70au), 1);
    } else {
        norrath_creature_set_parameters(object, c14, c12, 1.0f, 1.0f, c12, 1);
    }
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 84u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float c02 = bits_float(0x3e4cccccu), c06 = bits_float(0x3f199999u), c03 = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 3, c02, 1, c06, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, c02, 1, c06, 2, 1.0f, 2, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, c03, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, c03, 1, bits_float(0x3f4cccccu), 1, bits_float(0x3fccccccu), 4, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
