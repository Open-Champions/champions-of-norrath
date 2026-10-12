#include "../ctors-lava-wraith-detail/detail.hpp"
// __10WeaponRackG6Point3iPPc at 0x215f58, 448 bytes.
// GameObject receives a point snapshot, a null map and the argument list. View
// 0x448fb0 goes to +72. Halves at +140, +132, +124 and +116 become -1; the
// item description at +80 gets -1 bytes at +112/+113 and zero halves at +96,
// +98, +100, +102 and +104..+110. Flags gain 0x10001, the low half of the int
// goes to +56 and byte +40 becomes 40. The level is
//   treasureRamp * ((rand * 2^-16 * 0.1f) + 0.95f) * ((g * 0.2f + 0.8f) * 200)
// with rand from objectERand and g the float word at 0x484cac, multiplied once
// more by the float tag at 0x4853d0 when that string tag is present, and
// truncated toward zero. createRandom receives the description, that level, 0,
// -1 and 1. When it returns zero the fallback item::create runs with the text
// at 0x4853d8, the description, -1 and 0, and +84 becomes 40. Low V0 is the
// createRandom result when nonzero, else the constant 40 left by that store.
extern "C" unsigned norrath_construct_WeaponRack(void* object, const void* point, int type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const Address item = a + 80u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_448fb0));
    for (int slot = 0; slot < 4; ++slot) write_half(a + 140u - 8u * static_cast<unsigned>(slot), 0xffffu);
    write_byte(item + 33u, 0xffu);
    write_byte(item + 32u, 0xffu);
    for (unsigned offset = 16u; offset < 24u; offset += 2u) {
        write_half(item + offset, 0u);
        write_half(item + 8u + offset, 0u);
    }
    write_word(a + 12u, read_word(a + 12u) | 0x10001u);
    write_half(a + 56u, static_cast<unsigned>(type));
    write_byte(a + 40u, 40u);
    float level = norrath_player_treasure_ramp();
    const int random = norrath_game_object_e_rand(object);
    float spread = static_cast<float>(random) * bits_float(0x37800000u);
    float base = bits_float(read_word(address(norrath_creature_view_484cac)));
    spread = spread * bits_float(0x3dccccccu);
    base = base * bits_float(0x3e4cccccu);
    spread = spread + bits_float(0x3f733333u);
    base = base + bits_float(0x3f4cccccu);
    base = base * bits_float(0x43480000u);
    level = level * spread;
    level = level * base;
    char* const tag = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4853d0);
    if (norrath_actor_find_tag_string(object, tag, 0) != nullptr) {
        level = level * norrath_game_object_find_tag_float(object, tag, 0);
    }
    const int created = norrath_item_create_random(pointer(item), static_cast<int>(level), 0, -1, 1);
    if (created != 0) return static_cast<unsigned>(created);
    (void)norrath_item_create(reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4853d8), pointer(item), -1, 0);
    write_half(a + 84u, 40u);
    return 40u;
}
