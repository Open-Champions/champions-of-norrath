#include "../ctors-lava-wraith-detail/detail.hpp"
// __8SkeletonG6Point3sPPcPci at 0x243868, 2640 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// 0x42d7d0 + 64 * kind, scale 10.0f and the argument list. +960 and +964 are
// cleared and view 0x44fee8 goes to +72. Halves at +1108, +1100, +1092 and
// +1084 become -1; the item description at +1048 gets -1 bytes at +1080/+1081
// and zero halves at +1064..+1070 and +1072..+1078. The table at 0x42d8d0 is a
// list of (name, resource) pairs: its first pair holds a lump name and a load
// stamp; when that stamp differs from the word at 0x483f88, lumpFindResource
// (lump name, name) fills every following pair's resource until a null name and
// the stamp is rewritten from the word at 0x483f88. Byte +40 becomes 20, the low
// word of the doubleword at +160 gains 0x10000, byte +172 becomes 16, the
// literal 1.2f (0x3f999999) goes to +168, 18.0f to +28, the word at 0x4844a4 to
// +248, 1.0f to +96 and +100, and +176, +192, +236 and +572 (half) are cleared.
// A draw mod 5 goes to +968 and byte +972 is cleared; the name is then compared
// case-blind with seventeen texts. Variants (+972 byte, +968 word):
//   skeletonred (2, keep), skeletonblue (1, keep), swordohwred (2, 0),
//   swordohwblue (1, 0), sworddhwred (2, 1), sworddhwblue (1, 1), axered (2, 2),
//   axeblue (1, 2), spearred (2, 3), spearblue (1, 3), bowred and bowblue (both
//   2, 4), swordohw (keep, 0), sworddhw (keep, 1), axe (keep, 2), spear (keep, 3)
//   and bow (keep, 4), all with the "skeleton" prefix. +208 is cleared. A value
//   of +968 above 4 skips the switch; 0..4 pick (all rolls compare draw times
//   2^-16 with a literal about 0.1f):
//   0: roll -> +208 = 1; a draw's low bit picks +976 = 77 (set) or 79; another
//      draw gives +1120 = 91 + 2 * low bit.
//   1: roll -> +208 = 1; +976 = 89.
//   2: roll -> +208 = 1; a draw's low bit picks +976 = 85 (set) or 87.
//   3: roll -> +208 = (draw & 1) + 1; a draw's low bit picks +976 = 95 or 97.
//   4: roll -> +208 = (draw & 3) + 1; +1124 = 0; +976 = 83.
// +244 is cleared; a draw s: bit 0 adds 1 << (draw % 3); +968 == 4 adds 0x80,
// otherwise bit 1 of s adds 1 << (draw % 12 + 9); unless bit 14 is set a draw
// adds 1 << (draw % 5 + 21); a final draw adds 1 << ((draw & 1) + 5). The 64-bit
// modelGetActiveChunks (word at model+12, +244) goes to +552, initType(0) runs,
// the animation state at +288 is initialised and gets the header word at
// 0x42d8d0 + 104 * (+968) + 68, setState(0) and the slow run list follow and
// fifteen setAnimEventFlagsResMap calls (see the body) precede finishConstruction,
// whose machine result is low V0.
extern "C" unsigned norrath_construct_Skeleton(void* object, const void* point, short type, char** arguments, char* name, int kind) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    const Address table = address(norrath_ctors_lava_wraith_view_42d8d0);
    (void)norrath_creature_construct(object, &local, type,
                                     pointer(address(norrath_ctors_lava_wraith_view_42d7d0) + (static_cast<unsigned>(kind) << 6)),
                                     10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 1048u;
    write_word(a + 960u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44fee8));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1108u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    const unsigned stamp = read_word(address(norrath_ctors_lava_wraith_view_483f88));
    if (read_word(table + 4u) != stamp) {
        unsigned fresh = stamp;
        if (read_word(table + 8u) != 0u) {
            for (Address entry = table + 8u; read_word(entry) != 0u; entry += 8u) {
                store_pointer(entry + 4u, norrath_lump_find_resource(static_cast<char*>(load_pointer(table)),
                                                                     static_cast<char*>(load_pointer(entry))));
            }
            fresh = read_word(address(norrath_ctors_lava_wraith_view_483f88));
        }
        write_word(table + 4u, fresh);
    }
    write_byte(a + 40u, 20u);
    const unsigned low_word = read_word(a + 160u) | 0x10000u;
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_byte(a + 172u, 16u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(18.0f));
    write_word(a + 160u, low_word);
    write_word(a + 96u, float_bits(1.0f));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 100u, float_bits(1.0f));
    const unsigned pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 5u;
    write_byte(a + 972u, 0u);
    write_word(a + 968u, pick);
#define NORRATH_SKELETON_IS(view) (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_##view)) == 0)
    if (NORRATH_SKELETON_IS(473f20)) {
        write_byte(a + 972u, 2u);
    } else if (NORRATH_SKELETON_IS(473f30)) {
        write_byte(a + 972u, 1u);
    } else if (NORRATH_SKELETON_IS(473f40)) {
        write_word(a + 968u, 0u);
        write_byte(a + 972u, 2u);
    } else if (NORRATH_SKELETON_IS(473f58)) {
        write_word(a + 968u, 0u);
        write_byte(a + 972u, 1u);
    } else if (NORRATH_SKELETON_IS(473f70)) {
        write_byte(a + 972u, 2u);
        write_word(a + 968u, 1u);
    } else if (NORRATH_SKELETON_IS(473f88)) {
        write_byte(a + 972u, 1u);
        write_word(a + 968u, 1u);
    } else if (NORRATH_SKELETON_IS(473fa0)) {
        write_byte(a + 972u, 2u);
        write_word(a + 968u, 2u);
    } else if (NORRATH_SKELETON_IS(473fb0)) {
        write_byte(a + 972u, 1u);
        write_word(a + 968u, 2u);
    } else if (NORRATH_SKELETON_IS(473fc0)) {
        write_byte(a + 972u, 2u);
        write_word(a + 968u, 3u);
    } else if (NORRATH_SKELETON_IS(473fd8)) {
        write_byte(a + 972u, 1u);
        write_word(a + 968u, 3u);
    } else if (NORRATH_SKELETON_IS(473ff0) || NORRATH_SKELETON_IS(474000)) {
        write_byte(a + 972u, 2u);
        write_word(a + 968u, 4u);
    } else if (NORRATH_SKELETON_IS(474010)) {
        write_word(a + 968u, 0u);
    } else if (NORRATH_SKELETON_IS(474028)) {
        write_word(a + 968u, 1u);
    } else if (NORRATH_SKELETON_IS(474040)) {
        write_word(a + 968u, 2u);
    } else if (NORRATH_SKELETON_IS(474050)) {
        write_word(a + 968u, 3u);
    } else if (NORRATH_SKELETON_IS(474060)) {
        write_word(a + 968u, 4u);
    }
#undef NORRATH_SKELETON_IS
    const unsigned variant = read_word(a + 968u);
    write_word(a + 208u, 0u);
    const float scale = bits_float(0x37800000u);
    if (variant < 5u) {
        if (variant == 0u) {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < bits_float(0x3dccca2du)) write_word(a + 208u, 1u);
            const unsigned pick_weapon = (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) != 0u ? 77u : 79u;
            write_word(a + 976u, pick_weapon);
            write_word(a + 1120u, ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) << 1) + 91u);
        } else if (variant == 1u) {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < bits_float(0x3dccca2du)) write_word(a + 208u, 1u);
            write_word(a + 976u, 89u);
        } else if (variant == 2u) {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < bits_float(0x3dcccab3u)) write_word(a + 208u, 1u);
            write_word(a + 976u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) != 0u ? 85u : 87u);
        } else if (variant == 3u) {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < bits_float(0x3dcccccc)) {
                write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 1u);
            }
            write_word(a + 976u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) != 0u ? 95u : 97u);
        } else {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * scale < bits_float(0x3dcccbc0u)) {
                write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
            }
            write_word(a + 1124u, 0u);
            write_word(a + 976u, 83u);
        }
    }
    write_word(a + 244u, 0u);
    const unsigned bits = static_cast<unsigned>(norrath_game_object_e_rand(object));
    if ((bits & 1u) != 0u) {
        const unsigned bit = 1u << (static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u);
        write_word(a + 244u, read_word(a + 244u) | bit);
    }
    if (read_word(a + 968u) == 4u) {
        write_word(a + 244u, read_word(a + 244u) | 0x80u);
    } else if ((bits & 2u) != 0u) {
        const unsigned bit = 1u << (static_cast<unsigned>(norrath_game_object_e_rand(object)) % 12u + 9u);
        write_word(a + 244u, read_word(a + 244u) | bit);
    }
    if ((read_word(a + 244u) & 0x4000u) == 0u) {
        const unsigned bit = 1u << (static_cast<unsigned>(norrath_game_object_e_rand(object)) % 5u + 21u);
        write_word(a + 244u, read_word(a + 244u) | bit);
    }
    const unsigned last = 1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 5u);
    const unsigned mask = read_word(a + 244u) | last;
    write_word(a + 244u, mask);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(mask));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_skeleton_init_type(object, 0);
    norrath_actor_animation_initialize(bytes(a + 288u));
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(table + 104u * read_word(a + 968u) + 68u), 1);
    norrath_skeleton_set_state(object, 0);
    (void)norrath_actor_add_slow_run_list(object);
    void* const map = pointer(table);
    const float c03 = bits_float(0x3e999999u), c09 = bits_float(0x3f666666u), c02 = bits_float(0x3e4cccccu), c13 = bits_float(0x3fa66666u);
    norrath_creature_set_anim_event_flags_res_map(object, map, 1, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 2, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 3, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 14, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 15, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 16, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 27, 1, 1, 0, 0, c03, c09, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 28, 1, 1, 0, 0, c03, c09, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 29, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 40, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 41, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 42, 1, 0, 0, 0, c03, 0.0f, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 53, 2, 1, 0, 0, c02, c13, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 54, 2, 1, 0, 0, c02, c13, 0.0f, 0.0f);
    norrath_creature_set_anim_event_flags_res_map(object, map, 55, 2, 1, 0, 0, c02, c13, 0.0f, 0.0f);
    return norrath_creature_finish_construction(object);
}
