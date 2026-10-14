#include "../ctors-lava-wraith-detail/detail.hpp"
// __11VampireLordG6Point3sPPc at 0x2406c0, 1020 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d700, scale 10.0f and the argument list. +1028 is cleared and view
// 0x44fd08 goes to +72. Halves at +1156, +1148, +1140 and +1132 become -1; the
// item description at +1096 gets -1 bytes at +1128/+1129 and zero halves at
// +1112..+1118 and +1120..+1126. The doubleword at +160 gains 0x0000020420000000
// (low word 0x20000000, high word 0x204). The int at +808 plus 300 goes to +976
// and +988. Byte +172 becomes 16, the word at 0x4844a4 goes to +248, the literal
// 1.3f (0x3fa66666) to +168, 18.0f to +28, the literal -0.2f (0xbe4ccccc) to
// +88, 0.3f (0x3e999999) to +104, 1.0f to +96 and +100, and +176, +236, +572
// (half), +964, +968, +980, +984, +992, +1008, +1012, +1016, +1020 and +1024
// are cleared. setParameters receives 24.0f, 1.4f (0x3fb33333), 1.95f
// (0x3ff99999), 4.2f (0x40866666), 10.5f and 1. The animation state at +288 is
// initialised and gets the header word at model+44, its time is randomised and
// the slow run list is called. lumpLoad runs for the text at 0x4858c0. Nine
// setAnimEventFlags calls follow, listed in the body. +244 and +960 are
// cleared; when the int tag at 0x4858c8 is zero, +244 gains 2. The 64-bit
// modelGetActiveChunks (word at model+12, +244) is stored at +552 just before
// finishConstruction, whose machine result is low V0.
extern "C" unsigned norrath_construct_VampireLord(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d700, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1096u;
    write_word(a + 1028u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fd08));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1156u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    const unsigned low_word = read_word(a + 160u) | 0x20000000u;
    const unsigned high_word = read_word(a + 164u) | 0x204u;
    const unsigned bump = read_word(a + 808u) + 300u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 168u, 0x3fa66666u);
    write_word(a + 28u, float_bits(18.0f));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 88u, 0xbe4cccccu);
    write_byte(a + 172u, 16u);
    write_word(a + 988u, bump);
    write_word(a + 976u, bump);
    write_word(a + 176u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 100u, float_bits(1.0f));
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 104u, 0x3e999999u);
    write_word(a + 964u, 0u);
    write_word(a + 980u, 0u);
    write_word(a + 984u, 0u);
    write_word(a + 992u, 0u);
    write_word(a + 968u, 0u);
    write_word(a + 1008u, 0u);
    write_word(a + 1012u, 0u);
    write_word(a + 1020u, 0u);
    write_word(a + 1016u, 0u);
    write_word(a + 1024u, 0u);
    norrath_creature_set_parameters(object, 24.0f, bits_float(0x3fb33333u), bits_float(0x3ff99999u), bits_float(0x40866666u), 10.5f, 1);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 44u), 1);
    norrath_actor_animation_randomize_time(bytes(a + 288u));
    (void)norrath_actor_add_slow_run_list(object);
    (void)norrath_lump_load(reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4858c0));
    const float c03 = bits_float(0x3e999999u), c07 = bits_float(0x3f333333u), c12 = bits_float(0x3f999999u), c20 = 2.0f;
    norrath_creature_set_anim_event_flags(object, 18, c03, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 13, c03, 1, bits_float(0x3effffdeu), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, bits_float(0x3e4cccccu), 1, bits_float(0x3f1999aau), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 15, c07, 1, bits_float(0x3f999913u), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 12, bits_float(0x3f666666u), 1, c12, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 9, 0.5f, 1, c20, 2, c03, 4, bits_float(0x3eccccccu), 8);
    norrath_creature_set_anim_event_flags(object, 8, c12, 1, bits_float(0x3fe66666u), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, c07, 1, 1.0f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 20, c20, 64, bits_float(0x40066666u), 128, 0.0f, 0, 0.0f, 0);
    write_word(a + 244u, 0u);
    write_word(a + 960u, 0u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4858c8), 0) == 0) {
        write_word(a + 244u, read_word(a + 244u) | 0x2u);
    }
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    return norrath_creature_finish_construction(object);
}
