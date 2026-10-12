#include "../ctors-lava-wraith-detail/detail.hpp"
// __11MaleDarkElfG6Point3sPPcPc at 0x286920, 1976 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x438c70, scale 10.0f and the argument list. View 0x450440 goes to +72.
// Halves at +1100, +1092, +1084 and +1076 become -1; the item description at
// +1040 gets -1 bytes at +1072/+1073 and zero halves at +1056..+1062 and
// +1064..+1070. Byte +40 becomes 20, the literal 1.2f (0x3f999999) goes to
// +168, byte +172 becomes 9, 15.0f goes to +28, the word at 0x4844a4 to +248,
// +176, +192, +236 and +572 (half) are cleared. A draw mod 6 is stored as the
// byte +960, then the name is compared case-blind in turn with the texts at
// 0x477568 (0), 0x477580 (1), 0x477598 (2), 0x4775b0 (3), 0x4775c8 (4) and
// 0x4775d8 (5); the first match overwrites the byte. +244, +964, +968, +1112
// and +208 are cleared and the signed byte +960 selects (the original jump
// table at 0x4775f0, six entries; larger values skip the switch):
//   0: +964 = 64, +244 = (1 << (draw & 3)) | 0x08000000; a roll (draw times
//      2^-16 below the literal 0.1f 0x3dcccccc) sets +208 = draw % 5 + 1.
//   1: +964 = 52, +244 = 1 << (draw % 6 + 4); roll sets +208 = (draw & 3) + 1.
//   2: +964 = 62, +244 = 1 << (draw % 5 + 16), +1112 = (draw % 3) * 2 + 46;
//      roll sets +208 = (draw & 3) + 1.
//   3: +964 = 58, +244 = 1 << (draw % 6 + 10); roll sets +208 = (draw & 3) + 1.
//   4: +964 = 54, +968 = 56, +244 = 1 << (draw % 6 + 21); no roll.
//   5: +964 = 62, +968 = 60, +244 = 1 << (draw % 6 + 21); roll sets
//      +208 = draw % 3 + 1.
// initType(0) runs, the 64-bit modelGetActiveChunks (word at model+12, +244) is
// stored at +552 before the animation state at +288 is initialised, setState(0)
// and the slow run list follow, then 17 setAnimEventFlags calls (see the body)
// and finishConstruction, whose machine result is low V0.
extern "C" unsigned norrath_construct_MaleDarkElf(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_438c70, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1040u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_450440));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1100u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 20u);
    write_word(a + 168u, 0x3f999999u);
    write_byte(a + 172u, 9u);
    write_word(a + 28u, float_bits(15.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    const unsigned initial = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u;
    write_byte(a + 960u, initial);
    if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_477568)) == 0) {
        write_byte(a + 960u, 0u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_477580)) == 0) {
        write_byte(a + 960u, 1u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_477598)) == 0) {
        write_byte(a + 960u, 2u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4775b0)) == 0) {
        write_byte(a + 960u, 3u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4775c8)) == 0) {
        write_byte(a + 960u, 4u);
    } else if (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4775d8)) == 0) {
        write_byte(a + 960u, 5u);
    }
    const unsigned variant = static_cast<unsigned>(static_cast<int>(static_cast<signed char>(read_byte(a + 960u))));
    write_word(a + 244u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 968u, 0u);
    write_word(a + 1112u, 0u);
    write_word(a + 208u, 0u);
    const float threshold = bits_float(0x3dccccccu);
    const float scale = bits_float(0x37800000u);
    if (variant == 0u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u;
        write_word(a + 964u, 64u);
        write_word(a + 244u, (1u << shift) | 0x08000000u);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < threshold) {
            write_word(a + 208u, static_cast<unsigned>(norrath_game_object_e_rand(object)) % 5u + 1u);
        }
    } else if (variant == 1u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u + 4u;
        write_word(a + 964u, 52u);
        write_word(a + 244u, 1u << shift);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
        }
    } else if (variant == 2u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 5u + 16u;
        write_word(a + 964u, 62u);
        write_word(a + 244u, 1u << shift);
        write_word(a + 1112u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u) * 2u + 46u);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
        }
    } else if (variant == 3u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u + 10u;
        write_word(a + 964u, 58u);
        write_word(a + 244u, 1u << shift);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
        }
    } else if (variant == 4u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u + 21u;
        write_word(a + 964u, 54u);
        write_word(a + 968u, 56u);
        write_word(a + 244u, 1u << shift);
    } else if (variant == 5u) {
        const unsigned shift = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u + 21u;
        write_word(a + 964u, 62u);
        write_word(a + 968u, 60u);
        write_word(a + 244u, 1u << shift);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < threshold) {
            write_word(a + 208u, static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u + 1u);
        }
    }
    norrath_male_dark_elf_init_type(object, 0);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_male_dark_elf_set_state(object, 0);
    (void)norrath_actor_add_slow_run_list(object);
    const float c01 = bits_float(0x3dccccccu), c02 = bits_float(0x3e4cccccu), c06 = bits_float(0x3f199999u);
    norrath_creature_set_anim_event_flags(object, 19, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 20, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 21, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 13, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 14, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 15, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 28, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 29, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 34, c02, 1, bits_float(0x3f8cccccu), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 11, 0.5f, 2, c06, 4, 1.0f, 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 12, c02, 2, c06, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 3, c01, 1, c02, 1, bits_float(0x3eccccccu), 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, c01, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 38, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 40, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 42, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 44, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
