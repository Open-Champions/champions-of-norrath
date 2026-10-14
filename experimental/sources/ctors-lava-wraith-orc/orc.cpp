#include "../ctors-lava-wraith-detail/detail.hpp"
// __3OrcG6Point3sPPcPcc at 0x226598, 2588 bytes.
// objectERand runs first on the not yet constructed object (scale = draw % 14
// + 4 as an unsigned remainder). Creature receives the snapshot, the
// sign-extended type, the resource map 0x42c8e0 (variant zero) or 0x42ca58
// (any other signed-char variant), that scale and the argument list. View
// 0x44b720 goes to +72, +1128 is cleared. Halves at +1100, +1092, +1084 and
// +1076 become -1; the item description at +1040 gets -1 bytes at +1072/+1073
// and zero halves at +1056..+1062 and +1064..+1070. Variant 0 loads the
// (name, resource) table at 0x42cbd0 and variant 1 the one at 0x42ccf0: the
// first pair holds a lump name and a stamp; when the stamp differs from the
// word at 0x483f88, lumpFindResource (lump name, name) fills each following
// pair until a null name and the stamp is rewritten; the table address goes to
// +1136. Byte +40 becomes 40, the low word of the doubleword at +160 gains
// 0x00040400, 80.0f goes to +176, the word at 0x4844a4 to +248, the literal
// 1.2f (0x3f999999) to +168, 18.0f to +28, and +236, +572 (half) and +1120 are
// cleared. A draw mod 6 goes to +1116 (a 3 becomes 0). The name is compared
// case-blind with twelve texts; "orcrecruit", "orcsoldier", "orcleader",
// "orcleader1", "orcarcher" and "orcshaman" set +1116 to 0, 1, 3, 2, 4, 5 in the
// order of the views declared below, and the "orc2..." texts do the same. +244
// and +972 are cleared, a draw s is taken and +1116 selects:
//   0 and 4: +244 gets 1 or 2 (bit 0 of s, bit 1 picks), else 0x100, 0x200 or
//      0x400 from bits 1 and 2 of s; with 0, bits 0x32 of s allow a draw whose
//      clear low bit sets +1112 = 8; with 4 a roll (draw times 2^-16 below the
//      literal 0.1f) sets +208 = draw % 3 + 1, and +1124 becomes 1.
//   1: a roll sets +208 = (draw & 1) + 1; +244 gains 0x400000, 0x800000 (bit 0
//      of s), 0x200000 (bit 1), 0x2010 / 0x2008 / 0x2004 (bits 2, 3), 0x2000 and
//      1 << (15 + (draw & 3)); bits 0x16c of s set +1112 = (draw & 1) + 9.
//   2 and 3: +244 gains 1 << (5 + (draw & 1)), 0x1000 when that leaves bit 5 and
//      s bit 0, 0x00e04000 and 1 << (19 + (draw & 1)); +1112 = (draw & 1) + 11;
//      a roll sets +208 = (draw & 3) + 1.
//   5: +244 gains 0x20000000.
// setWeapon and initType(0) run, the 64-bit modelGetActiveChunks (word at
// model+12, +244) goes to +552 before the animation state at +288 is
// initialised, a draw mod 3 selects the header word at model + 16 * pick + 252
// to add, the slow run list is called and fourteen setAnimEventFlags calls
// precede finishConstruction (low V0 is its result).
extern "C" unsigned norrath_construct_Orc(void* object, const void* point, short type, char** arguments, char* name, signed char variant) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    const float scale = static_cast<float>(static_cast<int>(static_cast<unsigned>(norrath_game_object_e_rand(object)) % 14u + 4u));
    (void)norrath_creature_construct(object, &local, type,
                                     variant == 0 ? norrath_ctors_lava_wraith_view_42c8e0 : norrath_ctors_lava_wraith_view_42ca58,
                                     scale, arguments);
    const Address a = address(object);
    const Address item = a + 1040u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b720));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1100u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_word(a + 1128u, 0u);
    if (variant == 0 || variant == 1) {
        const Address table = address(variant == 0 ? norrath_ctors_lava_wraith_view_42cbd0 : norrath_ctors_lava_wraith_view_42ccf0);
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
        write_word(a + 1136u, static_cast<unsigned>(table));
    }
    write_byte(a + 40u, 40u);
    const unsigned low_word = read_word(a + 160u) | 0x00040400u;
    write_word(a + 176u, float_bits(80.0f));
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 28u, float_bits(18.0f));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 160u, low_word);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 1120u, 0u);
    unsigned pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 6u & 0xffffu;
    write_word(a + 1116u, pick);
    if (pick == 3u) write_word(a + 1116u, 0u);
#define NORRATH_ORC_IS(view) (norrath_strticmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_##view)) == 0)
    if (NORRATH_ORC_IS(4718e8)) write_word(a + 1116u, 0u);
    else if (NORRATH_ORC_IS(4718f8)) write_word(a + 1116u, 1u);
    else if (NORRATH_ORC_IS(471908)) write_word(a + 1116u, 3u);
    else if (NORRATH_ORC_IS(471918)) write_word(a + 1116u, 2u);
    else if (NORRATH_ORC_IS(471928)) write_word(a + 1116u, 4u);
    else if (NORRATH_ORC_IS(471938)) write_word(a + 1116u, 5u);
    else if (NORRATH_ORC_IS(471948)) write_word(a + 1116u, 0u);
    else if (NORRATH_ORC_IS(471958)) write_word(a + 1116u, 1u);
    else if (NORRATH_ORC_IS(471968)) write_word(a + 1116u, 3u);
    else if (NORRATH_ORC_IS(471978)) write_word(a + 1116u, 2u);
    else if (NORRATH_ORC_IS(471988)) write_word(a + 1116u, 4u);
    else if (NORRATH_ORC_IS(471998)) write_word(a + 1116u, 5u);
#undef NORRATH_ORC_IS
    write_word(a + 244u, 0u);
    write_word(a + 972u, 0u);
    const unsigned s = static_cast<unsigned>(norrath_game_object_e_rand(object));
    const unsigned kind = read_word(a + 1116u);
    const float unit = bits_float(0x37800000u), threshold = bits_float(0x3dccccccu);
    if (kind == 0u || kind == 4u) {
        unsigned mask;
        if ((s & 1u) != 0u) mask = read_word(a + 244u) | ((s & 2u) != 0u ? 1u : 2u);
        else if ((s & 2u) != 0u) mask = read_word(a + 244u) | 0x100u;
        else if ((s & 4u) != 0u) mask = read_word(a + 244u) | 0x200u;
        else mask = read_word(a + 244u) | 0x400u;
        write_word(a + 244u, mask);
        if (kind == 4u) {
            if (static_cast<float>(norrath_game_object_e_rand(object)) * unit < threshold) {
                write_word(a + 208u, static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u + 1u);
            }
            write_word(a + 1124u, 1u);
        } else if ((s & 0x32u) != 0u) {
            if ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) == 0u) write_word(a + 1112u, 8u);
        }
    } else if (kind == 1u) {
        if (static_cast<float>(norrath_game_object_e_rand(object)) * unit < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 1u);
        }
        unsigned mask = read_word(a + 244u) | 0x400000u;
        write_word(a + 244u, mask);
        if ((s & 1u) != 0u) {
            mask = mask | 0x800000u;
            write_word(a + 244u, mask);
        }
        if ((s & 2u) != 0u) write_word(a + 244u, read_word(a + 244u) | 0x200000u);
        const unsigned base = read_word(a + 244u);
        unsigned extra;
        if ((s & 4u) != 0u) extra = base | 0x2010u;
        else if ((s & 8u) != 0u) extra = base | 0x2008u;
        else extra = base | 0x2004u;
        write_word(a + 244u, base | 0x2000u);
        write_word(a + 244u, extra);
        write_word(a + 244u, read_word(a + 244u) | 0x2000u);
        const unsigned bit = 1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 15u);
        write_word(a + 244u, read_word(a + 244u) | bit);
        if ((s & 0x16cu) != 0u) write_word(a + 1112u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 9u);
    } else if (kind == 2u || kind == 3u) {
        unsigned mask = read_word(a + 244u) | (1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 5u));
        write_word(a + 244u, mask);
        if ((mask & 0x20u) != 0u && (s & 1u) != 0u) write_word(a + 244u, mask | 0x1000u);
        write_word(a + 244u, read_word(a + 244u) | 0x00e04000u);
        write_word(a + 244u, read_word(a + 244u) | (1u << ((static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 19u)));
        write_word(a + 1112u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 1u) + 11u);
        if (static_cast<float>(norrath_game_object_e_rand(object)) * unit < threshold) {
            write_word(a + 208u, (static_cast<unsigned>(norrath_game_object_e_rand(object)) & 3u) + 1u);
        }
    } else if (kind == 5u) {
        write_word(a + 244u, read_word(a + 244u) | 0x20000000u);
    }
    norrath_orc_set_weapon(object);
    norrath_orc_init_type(object, 0);
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    const unsigned header_pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 3u;
    norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + (header_pick << 4) + 252u), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float c03 = bits_float(0x3e999999u), c02 = bits_float(0x3e4cccccu), c06 = bits_float(0x3f199999u), c04 = bits_float(0x3ecccccc);
    norrath_creature_set_anim_event_flags(object, 19, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 20, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 21, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 22, 1.0f, 1, 2.0f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 23, c03, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 24, c03, 2, c06, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 25, c02, 2, 0.5f, 2, 1.0f, 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 26, c02, 2, 0.5f, 2, 1.0f, 1, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 3, 3.0f, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 4, 1.0f, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, 1.0f, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, c04, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 7, 1.0f, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 8, 1.0f, 1, 1.5f, 2, 0.0f, 0, 0.0f, 0);
    return norrath_creature_finish_construction(object);
}
