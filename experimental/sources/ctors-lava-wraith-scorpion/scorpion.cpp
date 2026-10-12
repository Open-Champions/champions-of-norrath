#include "../ctors-lava-wraith-detail/detail.hpp"
// __8ScorpionG6Point3sPPc at 0x1f2148, 720 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42b330, scale 10.0f and the argument list. Byte +40 becomes 70, the
// doubleword at +160 gains 0x0000008000001000, the word at 0x4844a4 goes to
// +248, the literal float 0x3f999999 to +168, 1 to +216, view 0x44b630 to +72,
// 30.0f to +28, byte +172 = 18, and +176, +192, +236, +244 and +572 (half) are
// cleared. setParameters receives 1.4f (0x3fb33333), 1.2f (+168's word), 1.0f
// twice more and 1. The float 0.4f (0x3ecccccc) goes to +96. A random
// remainder (objectERand, unsigned, mod 6) is converted to float, multiplied by
// 0.05f (0x3d4ccccc), widened with fptodp, added to the double 0x3fe8000000000000
// (0.75), narrowed with dptofp and stored at +224; +28 is multiplied by it.
// A second objectERand sets byte +960 to its low bit. The 64-bit
// modelGetActiveChunks (word at model+12, word at +244) goes to +552. The
// animation state at +288 is initialised and gets the header word at model+132,
// the slow run list is called, and setAnimEventFlags runs for 5, 6, 7 and 8
// with 0.3f (0x3e999999) and for 11 and 12 with 0.5f, each with 1 and zeros.
// Low V0 is finishConstruction's machine result.
extern "C" unsigned norrath_construct_Scorpion(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42b330, 10.0f, arguments);
    const Address a = address(object);
    write_byte(a + 40u, 70u);
    const unsigned low_word = read_word(a + 160u) | 0x1000u;
    const unsigned high_word = read_word(a + 164u) | 0x80u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 216u, 1u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b630));
    write_word(a + 28u, float_bits(30.0f));
    write_byte(a + 172u, 18u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    norrath_creature_set_parameters(object, bits_float(0x3fb33333u), bits_float(0x3f999999u), 1.0f, 1.0f, 1.0f, 1);
    write_half(a + 572u, 0u);
    write_word(a + 244u, 0u);
    write_word(a + 96u, 0x3eccccccu);
    const unsigned random = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const float step = static_cast<float>(static_cast<int>(random % 6u)) * bits_float(0x3d4cccccu);
    const unsigned long long widened = norrath_entity_fptodp(step);
    const unsigned long long sum = norrath_softdouble_add_2fb5c0(widened, 0x3fe8000000000000ull);
    const float factor = norrath_softdouble_to_float_2fbc20(sum);
    const float length = bits_float(read_word(a + 28u));
    write_word(a + 224u, float_bits(factor));
    write_word(a + 28u, float_bits(length * factor));
    write_byte(a + 960u, static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 132u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float three_tenths = bits_float(0x3e999999u);
    norrath_creature_set_anim_event_flags(object, 5, three_tenths, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, three_tenths, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 7, three_tenths, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 8, three_tenths, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 11, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 12, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
