#include "../ctors-lava-wraith-detail/detail.hpp"
// __5MummyG6Point3sPPcPc at 0x236b68, 1600 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42d2b0, scale 10.0f and the argument list. View 0x44fa38 goes to +72.
// Halves at +1092, +1084, +1076 and +1068 become -1; the item description at
// +1032 gets -1 bytes at +1064/+1065 and zero halves at +1048..+1054 and
// +1056..+1062. Byte +40 becomes 30, the doubleword at +160 gains
// 0x0000000400010000 (low word 0x00010000, high word 4), the literal 1.2f
// (0x3f999999) goes to +168, the word at 0x4844a4 to +248, byte +172 becomes 16,
// 15.0f goes to +28, and +176, +192, +236, +572 (half), +1104, +1108 and +1112
// are cleared. The name is compared case-blind with ten texts, pairwise, to
// choose a variant stored at +960: the pair at 0x472c10/0x472c28 gives 2, the
// pair at 0x472c38/0x472c50 gives 1, 0x472c60/0x472c78 gives 3,
// 0x472c88/0x472ca0 gives 4, 0x472cb0/0x472cc8 gives 5, anything else 0. +244
// is cleared and the variant selects a mask for +244 (this is the original
// jump table at 0x472ce0, six entries):
//   0: 1 << (draw % 3); then, if a second draw times 2^-16 is below the
//      literal 0.1f (0x3dcccccc), +208 = (third draw & 1) + 1.
//   1: 8.      3: 256.      5: 2048.
//   2: 1 << (4 + (draw & 3)); same second-draw rule, +208 = (draw & 1) + 1.
//   4: 1 << (9 + (draw & 1)); same rule with +208 = (draw & 3) + 1.
// Any variant above 5 leaves +244 at zero. The 64-bit modelGetActiveChunks
// (word at model+12, +244) goes to +552 just before setWeapon; initType(0), the
// animation state at +288 and setState(0) follow, then the slow run list and
// twelve setAnimEventFlags calls (see the body). finishConstruction runs last;
// low V0 is its machine result.
extern "C" unsigned norrath_construct_Mummy(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42d2b0, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1032u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fa38));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1092u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 30u);
    const unsigned low_word = read_word(a + 160u) | 0x10000u;
    const unsigned high_word = read_word(a + 164u) | 0x4u;
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(15.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_byte(a + 172u, 16u);
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 1104u, 0u);
    write_word(a + 1108u, 0u);
    write_word(a + 1112u, 0u);
    unsigned variant = 0u;
    if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c10)) == 0 ||
        norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c28)) == 0) {
        variant = 2u;
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c38)) == 0 ||
               norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c50)) == 0) {
        variant = 1u;
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c60)) == 0 ||
               norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c78)) == 0) {
        variant = 3u;
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472c88)) == 0 ||
               norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472ca0)) == 0) {
        variant = 4u;
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472cb0)) == 0 ||
               norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472cc8)) == 0) {
        variant = 5u;
    }
    write_word(a + 960u, variant);
    write_word(a + 244u, 0u);
    const float threshold = bits_float(0x3dccccccu);
    if (variant == 0u) {
        write_word(a + 244u, 1u << (static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u));
        const float roll = static_cast<float>(norrath_game_object_e_rand(object)) * bits_float(0x37800000u);
        if (roll < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 1u);
        }
    } else if (variant == 2u) {
        write_word(a + 244u, 1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 4u));
        const float roll = static_cast<float>(norrath_game_object_e_rand(object)) * bits_float(0x37800000u);
        if (roll < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 1u);
        }
    } else if (variant == 1u) {
        write_word(a + 244u, 8u);
    } else if (variant == 3u) {
        write_word(a + 244u, 256u);
    } else if (variant == 4u) {
        write_word(a + 244u, 1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 9u));
        const float roll = static_cast<float>(norrath_game_object_e_rand(object)) * bits_float(0x37800000u);
        if (roll < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
        }
    } else if (variant == 5u) {
        write_word(a + 244u, 2048u);
    }
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_mummy_set_weapon(object);
    norrath_mummy_init_type(object, 0);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_mummy_set_state(object, 0);
    (void)norrath_actor_add_slow_run_list(object);
    const float c01 = bits_float(0x3dccccccu), c02 = bits_float(0x3e4cccccu);
    norrath_creature_set_anim_event_flags(object, 3, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, bits_float(0x3e199999u), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 32, c02, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 33, 1.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 34, c02, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 15, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 16, c01, 1, 0.5f, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 17, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 18, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 19, c01, 1, 0.5f, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 43, c02, 1, bits_float(0x3f4cccccu), 2, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
