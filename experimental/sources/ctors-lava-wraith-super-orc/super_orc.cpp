#include "../ctors-lava-wraith-detail/detail.hpp"
// __8SuperOrcG6Point3sPPc at 0x229cf8, 664 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42ce60, scale 10.0f and the argument list. View 0x44b260 goes to +72.
// Halves at +1100, +1092, +1084 and +1076 become -1; the item description at
// +1040 gets -1 bytes at +1072/+1073 and zero halves at +1056..+1062 and
// +1064..+1070. Byte +40 becomes 80, the doubleword at +160 gains
// 0x0000002020000000 (low word 0x20000000, high word 0x20), 20.0f goes to +28,
// the word at 0x4844a4 to +248, the literal 1.2f (0x3f999999) to +168, the
// literal 1.3f (0x3fa66666) to +224, byte +172 = 9, +216 = 1, and +176, +192,
// +236, +572 (half), +964 and +968 are cleared. setParameters receives 24.0f,
// the literals 1.4f (0x3fb33333), 1.95f (0x3ff99999), 4.2f (0x40866666), 10.5f
// and 1. +244 is cleared and the 64-bit modelGetActiveChunks (word at model+12,
// 0) goes to +552. The animation state at +288 is initialised and gets the
// header word at model+68, and the slow run list is called. setAnimEventFlags
// runs for 3 (0.1f, 1, zeros), 4 (0.3f, 1, 0.8f, 2, 1.3f, 4, 0.0f, 0),
// 5 (1.05f, 1, zeros) and 6 (0.3f, 1, 0.7f, 2, zeros). 0.5f goes to +88, +92,
// +96, +100 and +104 and finishConstruction runs; low V0 is its result.
extern "C" unsigned norrath_construct_SuperOrc(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42ce60, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1040u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b260));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1100u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 80u);
    const unsigned low_word = read_word(a + 160u) | 0x20000000u;
    const unsigned high_word = read_word(a + 164u) | 0x20u;
    write_word(a + 28u, float_bits(20.0f));
    write_byte(a + 172u, 9u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 216u, 1u);
    write_word(a + 224u, 0x3fa66666u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 968u, 0u);
    norrath_creature_set_parameters(object, 24.0f, bits_float(0x3fb33333u), bits_float(0x3ff99999u), bits_float(0x40866666u), 10.5f, 1);
    write_word(a + 244u, 0u);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u), 0);
    norrath_actor_animation_initialize(bytes(a + 288u));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 68u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float third = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 3, bits_float(0x3dccccccu), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, third, 1, bits_float(0x3f4cccccu), 2, bits_float(0x3fa66666u), 4, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, bits_float(0x3f866666u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, third, 1, bits_float(0x3f333333u), 2, 0.0f, 0, 0.0f, 0);
    write_word(a + 104u, float_bits(0.5f));
    write_word(a + 88u, float_bits(0.5f));
    write_word(a + 92u, float_bits(0.5f));
    write_word(a + 96u, float_bits(0.5f));
    write_word(a + 100u, float_bits(0.5f));
    return norrath_creature_finish_construction(object);
}
