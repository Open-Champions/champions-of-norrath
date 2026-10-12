#include "../ctors-lava-wraith-detail/detail.hpp"
// __11LavaMonsterG6Point3sPPcPc at 0x1eb4e0, 1340 bytes.
// Creature receives a point snapshot, the sign-extended type, the resource map
// at 0x42b010, scale 10.0f and the argument list. View 0x44b810 goes to +72.
// Four slot halves at +1036, +1028, +1020 and +1012 become -1; the item
// description at +976 gets -1 bytes at +1008/+1009 and zero halves at
// +992..+998 and +1000..+1006. Byte +40 becomes 40, the doubleword at +160
// gains 0x0000004000020800 (low word 0x00020800, high word 0x40), byte +172
// becomes 11, 30.0f goes to +28, the word at 0x4844a4 to +248, the literal
// -0.2f (0xbe4ccccc) to +92, the literal 1.2f (0x3f999999) to +168, 1.0f to +88,
// 13 (half) to +572, and +176, +192, +236 and +960 are cleared. An objectERand
// draw (signed int) times 2^-16 times 0.2f (0x3e4ccccc) plus 0.9f (0x3f666666)
// goes to +224. +1048 is cleared, and becomes 1 when the name equals "lb_block"
// (the text at 0x46d7b8) case-blind. setParameters receives 3.0f, 1.2f, 1.0f,
// 1.0f, 1.0f and 1.// +244, getModel runs, and the 64-bit modelGetActiveChunks (that model, +244)
// is stored at +552 before the animation state at +288 is initialised. With
// +1048 equal to 1, +572 becomes zero and the header word at model+220 is
// added (flag 1); otherwise +572 stays 13 and the word at model+204 is added.
// A third draw (times 2^-16) below 1.1f runs a block: the object whose pointer
// is the word at 0x34b474 gets a virtual call through the table entry at
// (word at owner+96) + 96, a half this-adjustment followed by a function word,
// with (0, int(float at 0x484cac)); its int result R replaces a local. A fourth
// draw gives extra = int((float)R * 0.2f * (draw * 2^-16)). The first of the
// four slots at +1012 + 8*i whose half is -1 or 21 is stored as: half 21, then
// halves 0 at +6, R at +2 and R + extra at +4. The slow run list is called and
// nine setAnimEventFlags calls follow (see the body), then objectUpdateInGrid
// runs, 1.0f goes to +968 and half of the truncated float at +560 goes to +964
// (the integer divided by two, rounding toward zero) before finishConstruction;
// low V0 is its machine result.
extern "C" unsigned norrath_construct_LavaMonster(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_lava_wraith_view_42b010, 10.0f, arguments);
    const Address a = address(object);
    const Address item = a + 976u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44b810));
    for (unsigned slot = 0u; slot < 4u; ++slot) write_half(a + 1036u - 8u * slot, 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_byte(a + 40u, 40u);
    const unsigned low_word = read_word(a + 160u) | 0x00020800u;
    const unsigned high_word = read_word(a + 164u) | 0x40u;
    write_word(a + 28u, float_bits(30.0f));
    write_word(a + 160u, low_word);
    write_word(a + 164u, high_word);
    write_word(a + 92u, 0xbe4cccccu);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_byte(a + 172u, 11u);
    write_word(a + 176u, 0u);
    write_word(a + 168u, 0x3f999999u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 13u);
    write_word(a + 960u, 0u);
    write_word(a + 88u, float_bits(1.0f));
    float spread = static_cast<float>(norrath_game_object_e_rand(object));
    spread = spread * bits_float(0x37800000u);
    spread = spread * bits_float(0x3e4cccccu);
    spread = spread + bits_float(0x3f666666u);
    write_word(a + 1048u, 0u);
    write_word(a + 224u, float_bits(spread));
    if (norrath_factory_strcasecmp(name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_46d7b8)) == 0) {
        write_word(a + 1048u, 1u);
    }
    norrath_creature_set_parameters(object, 3.0f, bits_float(0x3f999999u), 1.0f, 1.0f, 1.0f, 1);
    const unsigned model_pick = static_cast<unsigned>(norrath_game_object_e_rand(object)) % 15u;
    write_word(a + 244u, model_pick);
    void* const model = norrath_lava_monster_get_model(object);
    const unsigned long long chunks = norrath_actor_model_active_chunks(model, static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    norrath_actor_animation_initialize(bytes(a + 288u));
    if (read_word(a + 1048u) == 1u) {
        write_half(a + 572u, 0u);
        norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 220u), 1);
    } else {
        write_half(a + 572u, 13u);
        norrath_actor_animation_add(bytes(a + 288u), load_pointer(address(load_pointer(a + 36u)) + 204u), 1);
    }
    const float draw = static_cast<float>(norrath_game_object_e_rand(object)) * bits_float(0x37800000u);
    if (draw < bits_float(0x3f8cccccu)) {
        typedef int (*Method)(void*, int, int);
        const Address owner = address(load_pointer(address(norrath_ctors_lava_wraith_view_34b474)));
        const int amount = static_cast<int>(bits_float(read_word(address(norrath_creature_view_484cac))));
        const Address entry = address(load_pointer(owner + 96u)) + 96u;
        const Method method = reinterpret_cast<Method>(load_pointer(entry + 4u));
        const int base = method(bytes(owner + static_cast<Address>(read_signed_half(entry))), 0, amount);
        float extra = static_cast<float>(base);
        extra = extra * bits_float(0x3e4cccccu);
        float roll = static_cast<float>(norrath_game_object_e_rand(object));
        roll = roll * bits_float(0x37800000u);
        extra = extra * roll;
        const int total = base + static_cast<int>(extra);
        for (unsigned index = 0u; index < 4u; ++index) {
            const Address slot = a + 1012u + 8u * index;
            const int mark = read_signed_half(slot);
            if (mark == -1 || mark == 21) {
                write_half(slot, 21u);
                write_half(slot + 6u, 0u);
                write_half(slot + 2u, static_cast<unsigned>(base));
                write_half(slot + 4u, static_cast<unsigned>(total));
                break;
            }
        }
    }
    (void)norrath_actor_add_slow_run_list(object);
    const float c06 = bits_float(0x3f199999u);
    norrath_creature_set_anim_event_flags(object, 17, c06, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 18, c06, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 19, 0.5f, 1, 1.0f, 1, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 20, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 21, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 23, 1.0f, 1, 3.0f, 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 24, 1.0f, 1, bits_float(0x40466666u), 2, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 5, bits_float(0x3f8cccccu), 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_creature_set_anim_event_flags(object, 6, 0.5f, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    norrath_object_update_in_grid(object);
    const int whole = static_cast<int>(bits_float(read_word(a + 560u)));
    write_word(a + 968u, float_bits(1.0f));
    const int halved = (whole + static_cast<int>(static_cast<unsigned>(whole) >> 31)) >> 1;
    write_word(a + 964u, static_cast<unsigned>(halved));
    return norrath_creature_finish_construction(object);
}
