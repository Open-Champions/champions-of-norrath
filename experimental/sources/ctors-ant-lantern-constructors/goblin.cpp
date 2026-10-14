#include "detail.hpp"
// __6GoblinG6Point3sPPcPc at 0x22bb10, 1644 bytes. The last argument is a name
// string.
// Observed: eRand is called first; the Creature scale is (roll % 14) + 4 as a
// single. Creature receives the snapshot, the short, the table at 0x42cef0 as its
// map and the argument list. +984 is cleared and _vt$6Goblin (0x44b900) goes to
// +72; halves at 1124, 1116, 1108 and 1100 are 0xffff, bytes 1096 and 1097 are
// 0xff and the 16 bytes at 1080..1095 are zero. A cached resource table at 0x42cfc8
// holds a lump name pointer, a stamp word and then (name, resource) pairs ended by
// a zero name. When its stamp differs from the word at 0x483f88 and the first name
// is nonzero, every pair's resource is replaced by lumpFindResource(lump name,
// name) and the stamp is stored again from the global; with an empty list the old
// global value is stored. Byte 40 is 40, 0x40000 goes into the low word at +160,
// 60.0 to +176, the single at 0x481890 to +168, 18.0 to +28, the word at 0x4844a4
// to +248, and +236, half 572, +976, +1136 and +1140 are cleared. A roll gives
// +972 = roll % 5, with 3 turned into 0; the name is then compared (strticmp)
// with the texts at 0x471ed0, 0x471ee0, 0x471ef0, 0x471f00, 0x471f10 and 0x471f20
// in that order and the first match sets +972 to 0, 1, 3, 2, 4 and 5. +992, +988,
// +208 and +244 are cleared and a further roll drives +244, +992 and +208 by the
// value of +972: 0 sets bits from rolls, 1 and 2 and 3 and 4 and 5 set the
// constants read from the code (see the host contract for each). Then
// setWeapons__6Goblin, initType__6Goblinb(object, 0), the eight-byte chunk result at
// +552, the animation state at +288 with the header at model+108, the slow run
// list, six setAnimEventFlags calls, a 64-byte clear at +1000 and V0 from
// finishConstruction.
extern "C" unsigned norrath_construct_Goblin(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned scale_roll = norrath_actor_random(object);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42cef0,
                                     static_cast<float>(static_cast<int>(scale_roll % 14u + 4u)), arguments);
    const Address a = address(object);
    write_word(a + 984u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44b900));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1124u - index * 8u, 0xffffu);
    bytes(a)[1097] = 0xffu;
    bytes(a)[1096] = 0xffu;
    for (unsigned offset = 1080u; offset < 1096u; offset += 2u) write_half(a + offset, 0u);
    const Address table = address(norrath_ctors_ant_lantern_view_42cfc8);
    unsigned stamp = read_word(address(norrath_ctors_ant_lantern_view_483f88));
    if (read_word(table + 4u) != stamp) {
        if (read_word(table + 8u) != 0u) {
            Address entry = table + 8u;
            do {
                write_word(entry + 4u, address_bits(norrath_lump_find_resource(
                    reinterpret_cast<char*>(pointer(read_word(table))), reinterpret_cast<char*>(pointer(read_word(entry))))));
                entry += 8u;
            } while (read_word(entry) != 0u);
            stamp = read_word(address(norrath_ctors_ant_lantern_view_483f88));
        }
        write_word(table + 4u, stamp);
    }
    bytes(a)[40] = 40u;
    write_word(a + 160u, read_word(a + 160u) | 0x40000u);
    write_word(a + 176u, 0x42700000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_481890)));
    write_word(a + 28u, 0x41900000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 976u, 0u);
    write_word(a + 1136u, 0u);
    write_word(a + 1140u, 0u);
    const unsigned variant_roll = norrath_actor_random(object) % 5u;
    write_word(a + 972u, variant_roll & 0xffffu);
    if ((variant_roll & 0xffffu) == 3u) write_word(a + 972u, 0u);
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471ed0)) == 0) {
        write_word(a + 972u, 0u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471ee0)) == 0) {
        write_word(a + 972u, 1u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471ef0)) == 0) {
        write_word(a + 972u, 3u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471f00)) == 0) {
        write_word(a + 972u, 2u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471f10)) == 0) {
        write_word(a + 972u, 4u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471f20)) == 0) {
        write_word(a + 972u, 5u);
    }
    write_word(a + 992u, 0u);
    write_word(a + 988u, 0u);
    write_word(a + 208u, 0u);
    write_word(a + 244u, 0u);
    const unsigned roll = norrath_actor_random(object);
    const unsigned variant = read_word(a + 972u);
    const auto below = [&](const unsigned char* threshold) {
        const float scaled = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
        return scaled < read_single(threshold);
    };
    if (variant == 0u) {
        if ((roll & 1u) != 0u) write_word(a + 244u, read_word(a + 244u) | (1u << (norrath_actor_random(object) % 3u + 5u)));
        if ((roll & 2u) != 0u) write_word(a + 244u, read_word(a + 244u) | 2u);
    } else if (variant == 1u) {
        if ((roll & 1u) != 0u) write_word(a + 992u, 4u);
        const unsigned base = read_word(a + 244u) | (1u << (norrath_actor_random(object) % 3u + 5u));
        write_word(a + 244u, base | 2u);
        if ((roll & 2u) != 0u) write_word(a + 244u, base | 6u);
        if ((roll & 4u) != 0u) write_word(a + 244u, read_word(a + 244u) | 8u);
        if (below(norrath_ctors_ant_lantern_view_481894)) write_word(a + 208u, (norrath_actor_random(object) & 1u) + 1u);
    } else if (variant == 2u || variant == 3u) {
        write_word(a + 992u, 4u);
        const unsigned extra = 1u << ((norrath_actor_random(object) & 1u) + 8u);
        write_word(a + 244u, read_word(a + 244u) | extra | 0x1fu);
        if (below(norrath_ctors_ant_lantern_view_481898)) write_word(a + 208u, (norrath_actor_random(object) & 3u) + 1u);
    } else if (variant == 4u) {
        write_word(a + 244u, read_word(a + 244u) | 0x4000u);
        if (below(norrath_ctors_ant_lantern_view_48189c)) write_word(a + 208u, norrath_actor_random(object) % 3u + 1u);
    } else if (variant == 5u) {
        write_word(a + 244u, read_word(a + 244u) | 0x10002u);
    }
    setWeapons__6Goblin(object);
    initType__6Goblinb(object, 0);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const float lead = read_single(norrath_ctors_ant_lantern_view_4818a0);
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const float second = read_single(norrath_ctors_ant_lantern_view_4818a4);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 108u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 2, second, 2, 1.0f, 1, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, second, 1, read_single(norrath_ctors_ant_lantern_view_4818a8), 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 16, 1.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, 1.0f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, 1.0f, 1, 1.0f, 2, 0.0f, 0, 0.0f, 0);
    __builtin_memset(pointer(a + 1000u), 0, 64);
    return norrath_actor_finish_construction(object);
}
