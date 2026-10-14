#include "factory.hpp"
namespace {
using Address = unsigned;
Address address(const void* p) { return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
unsigned char* bytes(Address p) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(p)); }
void* pointer(Address p) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(p)); }
char* text(Address p) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(p)); }
unsigned read_word(Address p) {
    return unsigned(bytes(p)[0]) | (unsigned(bytes(p)[1]) << 8) |
           (unsigned(bytes(p)[2]) << 16) | (unsigned(bytes(p)[3]) << 24);
}
unsigned read_half(Address p) { return unsigned(bytes(p)[0]) | (unsigned(bytes(p)[1]) << 8); }
void write_word(Address p, unsigned v) {
    bytes(p)[0] = static_cast<unsigned char>(v);
    bytes(p)[1] = static_cast<unsigned char>(v >> 8);
    bytes(p)[2] = static_cast<unsigned char>(v >> 16);
    bytes(p)[3] = static_cast<unsigned char>(v >> 24);
}
void write_half(Address p, unsigned v) {
    bytes(p)[0] = static_cast<unsigned char>(v);
    bytes(p)[1] = static_cast<unsigned char>(v >> 8);
}
short signed_low_half(unsigned v) {
    const unsigned h = v & 0xffffu;
    return static_cast<short>(h < 0x8000u ? int(h) : int(h) - 65536);
}
int signed_byte(unsigned char c) { return c < 128u ? int(c) : int(c) - 256; }
void copy_point(NorrathPoint3& dst, const void* src) {
    // Snapshot object representation, without float evaluation or conversion.
    const Address a = address(src), d = address(&dst);
    const unsigned x = read_word(a), y = read_word(a + 4u), z = read_word(a + 8u);
    write_word(d, x); write_word(d + 4u, y); write_word(d + 8u, z);
}
float read_float(Address p) { float v; write_word(address(&v), read_word(p)); return v; }
unsigned float_word(float v) { return read_word(address(&v)); }
bool equal_name(char* name, unsigned char* key) {
    return norrath_factory_strcasecmp(name, reinterpret_cast<char*>(key)) == 0;
}
bool prefix_name(char* name, unsigned char* key) {
    Address n = address(name), k = address(key);
    while (*bytes(k) != 0u) {
        const int left = signed_byte(*bytes(n)), right = signed_byte(*bytes(k));
        const unsigned lf = norrath_factory_ctype_minimum_view[unsigned(left + 128)];
        const unsigned rf = norrath_factory_ctype_minimum_view[unsigned(right + 128)];
        const int a = (lf & 1u) != 0u ? left + 32 : left;
        const int b = (rf & 1u) != 0u ? right + 32 : right;
        if (a != b) return false;
        n += 1u; k += 1u;
    }
    return true;
}
unsigned ee_truncate_word(unsigned bits) {
    // EE manual v6.0 CVT.W.S: toward zero; exponent >0x9d clamps by sign.
    // No host int cast, host infinity/NaN interpretation or FCSR rounding mode.
    const unsigned exponent = (bits >> 23) & 255u;
    const bool negative = (bits & 0x80000000u) != 0u;
    if (exponent > 157u) return negative ? 0x80000000u : 0x7fffffffu;
    if (exponent < 127u) return 0u;
    const unsigned significand = (bits & 0x7fffffu) | 0x800000u;
    const unsigned magnitude = exponent >= 150u ? significand << (exponent - 150u)
                                               : significand >> (150u - exponent);
    return negative ? 0u - magnitude : magnitude;
}
unsigned half_then_truncate(Address p) {
    // This separate target single operation is the original MUL.S by0.5.
    // Initial numerical proof is finite normals/signed zero with supported half.
    const float value = read_float(p);
    const float scaled = value * 0.5f;
    return ee_truncate_word(float_word(scaled));
}
unsigned char* base_object(int size, const NorrathPoint3& saved) {
    unsigned char* object = norrath_block_alloc(size);
    if (object != nullptr) {
        NorrathPoint3 copy;
        copy_point(copy, &saved);
        (void)norrath_game_object_construct(object, &copy, nullptr, nullptr);
    }
    return object;
}
}
// Every constructor answer is ignored. The returned object is the allocated
// pointer saved before the genuine call, including each allocation-null path.
#define CREATE(SIZE, CTOR, ...) do { \
    unsigned char* object = norrath_block_alloc(SIZE); \
    if (object != nullptr) (void)CTOR(object, __VA_ARGS__); \
    return object; \
} while (false)
extern "C" unsigned char* norrath_game_create_by_name(char* name, NorrathPoint3 incoming,
                                                     short type, char** arguments) {
    NorrathPoint3 saved;
    copy_point(saved, &incoming);
    const void* point = &saved;
    if (norrath_factory_strncmp(name, reinterpret_cast<char*>(norrath_factory_text_483c38), 6u) == 0) {
        norrath_monster_spawn(arguments, point, type, false);
        return nullptr;
    }
    // Original selection 0x14c49c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462010)) {
        unsigned char* object = base_object(76, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned x = half_then_truncate(a + 16u);
        const unsigned y = half_then_truncate(a + 20u);
        const unsigned z = half_then_truncate(a + 24u);
        write_word(a + 72u, address(norrath_factory_ambient_vtable));
        write_word(a + 12u, read_word(a + 12u) | 3u);
        const Address light = address(norrath_factory_marker_data_view);
        write_word(light, x); write_word(light + 4u, y); write_word(light + 8u, z);
        return object;
    }
    // Original selection 0x14c554: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462028)) {
        unsigned char* object = base_object(76, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned x = half_then_truncate(a + 16u);
        const unsigned y = half_then_truncate(a + 20u);
        const unsigned z = half_then_truncate(a + 24u);
        write_word(a + 72u, address(norrath_factory_directional_vtable));
        write_word(a + 12u, read_word(a + 12u) | 3u);
        const Address light = address(norrath_factory_marker_data_view);
        write_word(light + 12u, x); write_word(light + 16u, y); write_word(light + 20u, z);
        return object;
    }
    // Original selection 0x14c60c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462040)) {
        unsigned char* object = base_object(76, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object), light = address(norrath_factory_marker_data_view);
        write_word(light + 24u, read_word(a + 16u));
        const unsigned flags = read_word(a + 12u) | 3u;
        write_word(a + 72u, address(norrath_factory_directional_d_vtable));
        const unsigned y = read_word(a + 20u);
        write_word(a + 12u, flags);
        write_word(light + 28u, y);
        write_word(light + 32u, read_word(a + 24u));
        return object;
    }
    // Original selection 0x14c6b4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462058)) {
        const Address p = address(point);
        const unsigned z = ee_truncate_word(read_word(p + 8u));
        const unsigned y = ee_truncate_word(read_word(p + 4u));
        const unsigned x = ee_truncate_word(read_word(p));
        write_word(address(norrath_factory_clear_color_word), (z << 16) | (y << 8) | x | 0x80000000u);
        return nullptr;
    }
    // Original selection 0x14c70c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462000)) {
        norrath_custom_variables_process(arguments);
        return nullptr;
    }
    // Original selection 0x14c734: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462068)) {
        norrath_route_add_node(reinterpret_cast<const unsigned char*>(point));
        return nullptr;
    }
    // Original selection 0x14c75c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4623c0)) {
        CREATE(992, norrath_construct_Generator, point, type, arguments);
    }
    // Original selection 0x14c770: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4623d0)) {
        CREATE(416, norrath_construct_AntTrap, name, point, type, arguments);
    }
    // Original selection 0x14c7b4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4623e0)) {
        CREATE(1096, norrath_construct_AutoPropLP, name, point, type, arguments);
    }
    // Original selection 0x14c7c8: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4623f0)) {
        CREATE(1096, norrath_construct_AutoPropLP, name, point, type, arguments);
    }
    // Original selection 0x14c7dc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462400)) {
        CREATE(1096, norrath_construct_AutoPropLP, name, point, type, arguments);
    }
    // Original selection 0x14c7f0: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c40)) {
        CREATE(1088, norrath_construct_Critter, name, point, type, arguments);
    }
    // Original selection 0x14c804: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c48)) {
        CREATE(80, norrath_construct_Trap, point, type, arguments);
    }
    // Original selection 0x14c844: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c50)) {
        CREATE(1096, norrath_construct_EggSack, name, point, type, arguments);
    }
    // Original selection 0x14c88c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462410)) {
        CREATE(416, norrath_construct_GothTrap, name, point, type, arguments);
    }
    // Original selection 0x14c8d4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462420)) {
        CREATE(416, norrath_construct_HateHole, name, point, type, arguments);
    }
    // Original selection 0x14c91c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462430)) {
        CREATE(416, norrath_construct_HateBridge, name, point, type, arguments);
    }
    // Original selection 0x14c964: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462440)) {
        CREATE(408, norrath_construct_HackDoor, name, point, type, arguments);
    }
    // Original selection 0x14c9ac: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c58)) {
        CREATE(76, norrath_construct_Player1, point, type, arguments);
    }
    // Original selection 0x14c9ec: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462450)) {
        CREATE(992, norrath_construct_AntQueen, point, type, arguments);
    }
    // Original selection 0x14ca2c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462460)) {
        CREATE(992, norrath_construct_Ant, point, type, arguments, name, static_cast<signed char>(1));
    }
    // Original selection 0x14ca44: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462470)) {
        CREATE(992, norrath_construct_Ant, point, type, arguments, name, static_cast<signed char>(1));
    }
    // Original selection 0x14ca90: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483c60)) {
        CREATE(992, norrath_construct_Ant, point, type, arguments, name, 0);
    }
    // Original selection 0x14cb44: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462480)) {
        CREATE(976, norrath_construct_Arenabeast, point, type, arguments);
    }
    // Original selection 0x14cb84: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462490)) {
        CREATE(968, norrath_construct_Badger, point, type, arguments, 1);
    }
    // Original selection 0x14cbd0: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483c68)) {
        CREATE(968, norrath_construct_Badger, point, type, arguments, 0);
    }
    // Original selection 0x14cc88: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4624a0)) {
        CREATE(1040, norrath_construct_BlackWidow, point, type, arguments, name);
    }
    // Original selection 0x14cd3c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c70)) {
        CREATE(992, norrath_construct_Cat, point, type, arguments);
    }
    // Original selection 0x14cd7c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_461ff0)) {
        CREATE(968, norrath_construct_CdEyeball, point, type, arguments);
    }
    // Original selection 0x14cdc0: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4624b0)) {
        CREATE(1312, norrath_construct_CloudGiant, point, type, arguments, name);
    }
    // Original selection 0x14ce74: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c78)) {
        CREATE(976, norrath_construct_Cthulu, point, type, arguments);
    }
    // Original selection 0x14ceb4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4624c0)) {
        CREATE(968, norrath_construct_Cyclops, point, type, arguments, 1);
    }
    // Original selection 0x14cefc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c80)) {
        CREATE(968, norrath_construct_Cyclops, point, type, arguments, 0);
    }
    // Original selection 0x14cf44: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c88)) {
        CREATE(1104, norrath_construct_Demon, point, type, arguments);
    }
    // Original selection 0x14cf88: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4624d0)) {
        CREATE(1112, norrath_construct_FemaleDarkElf, point, type, arguments, name);
    }
    // Original selection 0x14d03c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c90)) {
        CREATE(1064, norrath_construct_FireBeetle, point, type, arguments, 2);
    }
    // Original selection 0x14d084: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483c98)) {
        CREATE(1064, norrath_construct_FireBeetle, point, type, arguments, 1);
    }
    // Original selection 0x14d0d0: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4624e0)) {
        CREATE(1064, norrath_construct_FireBeetle, point, type, arguments, 0);
    }
    // Original selection 0x14d184: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483ca0)) {
        CREATE(1128, norrath_construct_FireFly, point, type, arguments);
    }
    // Original selection 0x14d1c8: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4624f0)) {
        CREATE(1112, norrath_construct_Froglock, point, type, arguments, name);
    }
    // Original selection 0x14d280: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483ca8)) {
        CREATE(976, norrath_construct_Ghoul, point, type, 0, arguments);
    }
    // Original selection 0x14d338: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_462500)) {
        CREATE(976, norrath_construct_Ghoul, point, type, 1, arguments);
    }
    // Original selection 0x14d3ec: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462510)) {
        CREATE(960, norrath_construct_GnomeNavigator, point, type, arguments);
    }
    // Original selection 0x14d430: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483cb0)) {
        CREATE(976, norrath_construct_Gnome, point, type, arguments, name);
    }
    // Original selection 0x14d4e8: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483cb8)) {
        CREATE(1144, norrath_construct_Goblin, point, type, arguments, name);
    }
    // Original selection 0x14d59c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462520)) {
        CREATE(1224, norrath_construct_Innoruuk, point, type, arguments);
    }
    // Original selection 0x14d5dc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462530)) {
        CREATE(1056, norrath_construct_LavaMonster, point, type, arguments, name);
    }
    // Original selection 0x14d5f4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462540)) {
        CREATE(1056, norrath_construct_LavaMonster, point, type, arguments, name);
    }
    // Original selection 0x14d640: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_462550)) {
        CREATE(1120, norrath_construct_MaleDarkElf, point, type, arguments, name);
    }
    // Original selection 0x14d6f4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483cc0)) {
        CREATE(960, norrath_construct_Mermaid, point, type, arguments);
    }
    // Original selection 0x14d734: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483cc8)) {
        CREATE(992, norrath_construct_Minion, point, type, arguments);
    }
    // Original selection 0x14d774: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462560)) {
        CREATE(1168, norrath_construct_MummyKing, point, type, arguments);
    }
    // Original selection 0x14d7b8: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483cd0)) {
        CREATE(1128, norrath_construct_Mummy, point, type, arguments, name);
    }
    // Original selection 0x14d86c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483cd8)) {
        CREATE(960, norrath_construct_Natasla, point, type, arguments);
    }
    // Original selection 0x14d8b0: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483ce0)) {
        CREATE(1144, norrath_construct_Orc, point, type, arguments, name, static_cast<signed char>(1));
    }
    // Original selection 0x14d940: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483ce8)) {
        const signed char variant = (read_word(address(norrath_factory_world_flags_word)) & 0x30000u) != 0u ? 0 : 1;
        CREATE(1144, norrath_construct_Orc, point, type, arguments, name, variant);
    }
    // Original selection 0x14da44: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483cf0)) {
        CREATE(968, norrath_construct_Rondo, point, type, arguments);
    }
    // Original selection 0x14da84: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462570)) {
        CREATE(968, norrath_construct_Scorpion, point, type, arguments);
    }
    // Original selection 0x14dac4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462580)) {
        CREATE(968, norrath_construct_SeaMonster, point, type, arguments);
    }
    // Original selection 0x14db04: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483cf8)) {
        CREATE(1128, norrath_construct_Skeleton, point, type, arguments, name, 1);
    }
    // Original selection 0x14db58: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_462590)) {
        // Both nested prefix tests restart at the ORIGINAL name pointer.
        const int variant = prefix_name(name, norrath_factory_text_483d00) ? 2
                          : prefix_name(name, norrath_factory_text_483d08) ? 3 : 0;
        CREATE(1128, norrath_construct_Skeleton, point, type, arguments, name, variant);
    }
    // Original selection 0x14dd7c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625a0)) {
        CREATE(1112, norrath_construct_Skelight, point, type, arguments, static_cast<signed char>(1), 1);
    }
    // Original selection 0x14ddc4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625b0)) {
        CREATE(960, norrath_construct_SmallSpider, point, type, arguments);
    }
    // Original selection 0x14de04: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d10)) {
        CREATE(984, norrath_construct_Soul, point, type, arguments, nullptr);
    }
    // Original selection 0x14de4c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625c0)) {
        CREATE(1064, norrath_construct_SpiderQueen, point, type, arguments);
    }
    // Original selection 0x14de8c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625d0)) {
        CREATE(1112, norrath_construct_SuperOrc, point, type, arguments);
    }
    // Original selection 0x14decc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d18)) {
        CREATE(1112, norrath_construct_Ulthork, point, type, arguments);
    }
    // Original selection 0x14df0c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625e0)) {
        CREATE(1120, norrath_construct_UndeadKnight, point, type, arguments);
    }
    // Original selection 0x14df4c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4625f0)) {
        CREATE(2704, norrath_construct_UndeadKnightClone, point, type, arguments);
    }
    // Original selection 0x14df8c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d20)) {
        CREATE(960, norrath_construct_Vampire, point, type, arguments);
    }
    // Original selection 0x14dfcc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462600)) {
        CREATE(1168, norrath_construct_VampireLord, point, type, arguments);
    }
    // Original selection 0x14e010: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483d28)) {
        CREATE(992, norrath_construct_WoodElfSoldier, point, type, arguments, name);
    }
    // Original selection 0x14e0c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d30)) {
        CREATE(976, norrath_construct_Wraith, point, type, arguments);
    }
    // Original selection 0x14e104: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d38)) {
        CREATE(160, norrath_construct_WebTrap, point, type, arguments);
    }
    // Original selection 0x14e144: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b40)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 0);
    }
    // Original selection 0x14e184: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b50)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 5);
    }
    // Original selection 0x14e1c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462610)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 1);
    }
    // Original selection 0x14e204: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b48)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 2);
    }
    // Original selection 0x14e244: eq; preserves order.
    if (equal_name(name, norrath_factory_text_461fc0)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 3);
    }
    // Original selection 0x14e284: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d40)) {
        CREATE(140, norrath_construct_Torch, point, arguments, 4);
    }
    // Original selection 0x14e2c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d48)) {
        CREATE(148, norrath_construct_Candle, point, 0);
    }
    // Original selection 0x14e304: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d50)) {
        CREATE(148, norrath_construct_Candle, point, 1);
    }
    // Original selection 0x14e344: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d58)) {
        CREATE(80, norrath_construct_Candle2, point);
    }
    // Original selection 0x14e37c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b38)) {
        CREATE(152, norrath_construct_Lantern, point);
    }
    // Original selection 0x14e3b4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462620)) {
        CREATE(92, norrath_construct_WaterSpout, point, int(type));
    }
    // Original selection 0x14e3f4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462630)) {
        CREATE(80, norrath_construct_MissileTrap, point, int(type), arguments);
    }
    // Original selection 0x14e434: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462640)) {
        CREATE(88, norrath_construct_Shooter, point, 0, arguments);
    }
    // Original selection 0x14e44c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462650)) {
        CREATE(88, norrath_construct_Shooter, point, 1, arguments);
    }
    // Original selection 0x14e48c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d60)) {
        CREATE(88, norrath_construct_Shooter, point, 0, arguments);
    }
    // Original selection 0x14e4cc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462660)) {
        CREATE(368, norrath_construct_UserParamProp, point, type, arguments);
    }
    // Original selection 0x14e50c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462670)) {
        unsigned char* object = base_object(96, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned flags = read_word(a + 12u) | 0x10u;
        write_word(a + 72u, address(norrath_factory_cosmetic_vtable));
        write_word(a + 32u, 0x41300000u); write_word(a + 28u, 0x41900000u);
        bytes(a)[40] = 18u; bytes(a)[41] = 11u;
        write_word(a + 84u, address(norrath_factory_text_4626a8));
        write_word(a + 76u, address(norrath_factory_text_462680));
        write_word(a + 80u, address(norrath_factory_text_462690));
        write_half(a + 56u, unsigned(type));
        write_word(a + 12u, flags);
        void* first = norrath_lump_find_resource(reinterpret_cast<char*>(norrath_factory_text_462680),
                                                reinterpret_cast<char*>(norrath_factory_text_462690));
        write_word(a + 88u, address(first));
        void* second = norrath_lump_find_resource(text(read_word(a + 76u)), text(read_word(a + 84u)));
        write_word(a + 92u, address(second));
        return object;
    }
    // Original selection 0x14e5f8: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483d68)) {
        CREATE(136, norrath_construct_Chest, point, type, arguments, 0);
    }
    // Original selection 0x14e6ac: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d70)) {
        CREATE(136, norrath_construct_Chest, point, type, arguments, 1);
    }
    // Original selection 0x14e6f4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d78)) {
        CREATE(136, norrath_construct_Chest, point, type, arguments, 2);
    }
    // Original selection 0x14e73c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4626c0)) {
        CREATE(136, norrath_construct_Chest, point, type, arguments, 3);
    }
    // Original selection 0x14e788: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483d80)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_4626d0), reinterpret_cast<char*>(norrath_factory_text_4626e0), reinterpret_cast<char*>(norrath_factory_text_4626f0), reinterpret_cast<char*>(norrath_factory_text_462700), reinterpret_cast<char*>(norrath_factory_text_4626f0), 16, 42, 257, arguments);
    }
    // Original selection 0x14e85c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462718)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462728), reinterpret_cast<char*>(norrath_factory_text_462738), reinterpret_cast<char*>(norrath_factory_text_462748), reinterpret_cast<char*>(norrath_factory_text_462758), reinterpret_cast<char*>(norrath_factory_text_462770), 16, 42, 1281, arguments);
    }
    // Original selection 0x14e8c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d88)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462788), reinterpret_cast<char*>(norrath_factory_text_462798), reinterpret_cast<char*>(norrath_factory_text_4627a8), reinterpret_cast<char*>(norrath_factory_text_4627b8), reinterpret_cast<char*>(norrath_factory_text_4627d0), 16, 42, 769, arguments);
    }
    // Original selection 0x14e92c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4627e8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_4627f8), reinterpret_cast<char*>(norrath_factory_text_462808), reinterpret_cast<char*>(norrath_factory_text_462818), reinterpret_cast<char*>(norrath_factory_text_462828), reinterpret_cast<char*>(norrath_factory_text_462840), 16, 42, 1281, arguments);
    }
    // Original selection 0x14e994: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462858)) {
        CREATE(136, norrath_construct_Chest, point, type, arguments, 4);
    }
    // Original selection 0x14e9dc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462868)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462878), reinterpret_cast<char*>(norrath_factory_text_462888), reinterpret_cast<char*>(norrath_factory_text_462898), reinterpret_cast<char*>(norrath_factory_text_4628a8), reinterpret_cast<char*>(norrath_factory_text_4628c0), 20, 20, 1153, arguments);
    }
    // Original selection 0x14ea3c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_4628d8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_4628e8), reinterpret_cast<char*>(norrath_factory_text_4628f8), reinterpret_cast<char*>(norrath_factory_text_462908), reinterpret_cast<char*>(norrath_factory_text_462918), reinterpret_cast<char*>(norrath_factory_text_462930), 20, 20, 1153, arguments);
    }
    // Original selection 0x14ea9c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462948)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462958), reinterpret_cast<char*>(norrath_factory_text_462968), reinterpret_cast<char*>(norrath_factory_text_462978), reinterpret_cast<char*>(norrath_factory_text_462988), reinterpret_cast<char*>(norrath_factory_text_4629a0), 16, 48, 257, arguments);
    }
    // Original selection 0x14eb08: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_4629b8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_4629c8), reinterpret_cast<char*>(norrath_factory_text_4629d8), reinterpret_cast<char*>(norrath_factory_text_4629e8), reinterpret_cast<char*>(norrath_factory_text_4629f8), reinterpret_cast<char*>(norrath_factory_text_462a10), 11, 29, 5, arguments);
    }
    // Original selection 0x14ebf4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462a28)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462a38), reinterpret_cast<char*>(norrath_factory_text_462a48), reinterpret_cast<char*>(norrath_factory_text_462a60), reinterpret_cast<char*>(norrath_factory_text_4629f8), reinterpret_cast<char*>(norrath_factory_text_462a10), 11, 29, 5, arguments);
    }
    // Original selection 0x14ec54: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483d90)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_483d98), reinterpret_cast<char*>(norrath_factory_text_462a78), reinterpret_cast<char*>(norrath_factory_text_462a88), reinterpret_cast<char*>(norrath_factory_text_462a98), reinterpret_cast<char*>(norrath_factory_text_462a88), 10, 33, 65, arguments);
    }
    // Original selection 0x14ecbc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483da0)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_483d98), reinterpret_cast<char*>(norrath_factory_text_462a78), reinterpret_cast<char*>(norrath_factory_text_462a88), reinterpret_cast<char*>(norrath_factory_text_462a98), reinterpret_cast<char*>(norrath_factory_text_462a88), 10, 33, 65, arguments);
    }
    // Original selection 0x14ed24: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483da8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_483d98), reinterpret_cast<char*>(norrath_factory_text_462ab0), reinterpret_cast<char*>(norrath_factory_text_462ac0), reinterpret_cast<char*>(norrath_factory_text_462ad0), reinterpret_cast<char*>(norrath_factory_text_462ac0), 10, 19, 65, arguments);
    }
    // Original selection 0x14ed8c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483db0)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_483d98), reinterpret_cast<char*>(norrath_factory_text_462ab0), reinterpret_cast<char*>(norrath_factory_text_462ac0), reinterpret_cast<char*>(norrath_factory_text_462ad0), reinterpret_cast<char*>(norrath_factory_text_462ac0), 10, 19, 65, arguments);
    }
    // Original selection 0x14edf4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483db8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_483d98), reinterpret_cast<char*>(norrath_factory_text_462ae8), reinterpret_cast<char*>(norrath_factory_text_462af8), reinterpret_cast<char*>(norrath_factory_text_462b08), reinterpret_cast<char*>(norrath_factory_text_462af8), 10, 19, 65, arguments);
    }
    // Original selection 0x14ee5c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483dc0)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462b20), reinterpret_cast<char*>(norrath_factory_text_462b30), reinterpret_cast<char*>(norrath_factory_text_462b40), reinterpret_cast<char*>(norrath_factory_text_462b50), reinterpret_cast<char*>(norrath_factory_text_462b40), 16, 20, 129, arguments);
    }
    // Original selection 0x14eec4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462b68)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462958), reinterpret_cast<char*>(norrath_factory_text_462b78), reinterpret_cast<char*>(norrath_factory_text_462b88), reinterpret_cast<char*>(norrath_factory_text_462b98), reinterpret_cast<char*>(norrath_factory_text_462bb0), 16, 20, 129, arguments);
    }
    // Original selection 0x14ef2c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462bc8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462958), reinterpret_cast<char*>(norrath_factory_text_462bd8), reinterpret_cast<char*>(norrath_factory_text_462be8), reinterpret_cast<char*>(norrath_factory_text_462bf8), reinterpret_cast<char*>(norrath_factory_text_462c10), 16, 20, 129, arguments);
    }
    // Original selection 0x14ef94: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462c28)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462c38), reinterpret_cast<char*>(norrath_factory_text_462c48), reinterpret_cast<char*>(norrath_factory_text_462c58), reinterpret_cast<char*>(norrath_factory_text_462c68), reinterpret_cast<char*>(norrath_factory_text_462c80), 17, 40, 73, arguments);
    }
    // Original selection 0x14effc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462c98)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462c38), reinterpret_cast<char*>(norrath_factory_text_462ca8), reinterpret_cast<char*>(norrath_factory_text_462cb8), reinterpret_cast<char*>(norrath_factory_text_462cc8), reinterpret_cast<char*>(norrath_factory_text_462ce0), 17, 37, 73, arguments);
    }
    // Original selection 0x14f07c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462cf8)) {
        CREATE(132, norrath_construct_ContainerWithResources, point, type, reinterpret_cast<char*>(norrath_factory_text_462d08), reinterpret_cast<char*>(norrath_factory_text_462d18), reinterpret_cast<char*>(norrath_factory_text_462d28), reinterpret_cast<char*>(norrath_factory_text_462d38), reinterpret_cast<char*>(norrath_factory_text_462d50), 20, 20, 1, arguments);
    }
    // Original selection 0x14f0fc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483dc8)) {
        CREATE(104, norrath_construct_Ice, point, type, reinterpret_cast<char*>(norrath_factory_text_462d68), reinterpret_cast<char*>(norrath_factory_text_462d78), reinterpret_cast<char*>(norrath_factory_text_462d88), reinterpret_cast<char*>(norrath_factory_text_462d98), reinterpret_cast<char*>(norrath_factory_text_462db0));
    }
    // Original selection 0x14f164: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483dd0)) {
        CREATE(112, norrath_construct_Trigger, point, arguments);
    }
    // Original selection 0x14f1a4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462dc8)) {
        CREATE(80, norrath_construct_TriggerParams, point, arguments);
    }
    // Original selection 0x14f1e4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462dd8)) {
        unsigned char* object = norrath_block_alloc(76);
        if (object == nullptr) return nullptr;
        const Address world = read_word(address(norrath_factory_world_pointer_word));
        const int selected_type = read_word(world + 112u) != 0u ? int(type) : 0;
        (void)norrath_construct_PushTrigger(object, point, selected_type, arguments);
        return object;
    }
    // Original selection 0x14f234: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483dd8)) {
        unsigned char* object = base_object(328, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned flags = read_word(a + 12u) | 0x1809u;
        write_word(a + 72u, address(norrath_factory_prop_vtable));
        write_word(a + 12u, flags);
        norrath_object_update_in_grid(object);
        write_half(a + 56u, unsigned(type));
        write_half(a + 320u, read_half(a + 320u) | 1u);
        write_word(a + 28u, 0x40800000u);
        NorrathPoint3 current;
        copy_point(current, pointer(a + 16u));
        const short yaw = signed_low_half(read_half(a + 56u) + 0x4000u);
        norrath_matrix_make_translation_yaw(pointer(a + 272u), &current, yaw);
        return object;
    }
    // Original selection 0x14f304: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462de8)) {
        CREATE(400, norrath_construct_ItemProp, point, type, arguments);
    }
    // Original selection 0x14f344: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462df8)) {
        CREATE(92, norrath_construct_DoorSwing, point, arguments);
    }
    // Original selection 0x14f384: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e08)) {
        CREATE(92, norrath_construct_DoorSecret, point, arguments);
    }
    // Original selection 0x14f3c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e18)) {
        CREATE(92, norrath_construct_RollDoor, point, type, arguments);
    }
    // Original selection 0x14f404: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483de0)) {
        CREATE(80, norrath_construct_Switch, point, arguments);
    }
    // Original selection 0x14f444: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e28)) {
        CREATE(88, norrath_construct_FloorSwitch, point, arguments);
    }
    // Original selection 0x14f484: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e38)) {
        CREATE(208, norrath_construct_Savepoint, point, type, arguments);
    }
    // Original selection 0x14f4c4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e48)) {
        CREATE(356, norrath_construct_JumpGate, point, type, arguments);
    }
    // Original selection 0x14f504: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483de8)) {
        CREATE(356, norrath_construct_JumpGate, point, type, arguments);
    }
    // Original selection 0x14f544: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462e58)) {
        CREATE(76, norrath_construct_PokeReflector, point, arguments);
    }
    // Original selection 0x14f584: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483df0)) {
        unsigned char* object = base_object(152, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object), animation = a + 100u;
        write_word(a + 72u, address(norrath_factory_animation_vtable));
        bytes(animation)[5] = 1u;
        const unsigned flags = read_word(a + 12u) | 9u;
        write_word(a + 32u, 0x42100000u); write_word(a + 28u, 0x41900000u);
        bytes(a)[40] = 18u; bytes(a)[41] = 36u;
        write_word(a + 84u, address(norrath_factory_text_462e88));
        write_word(a + 88u, address(norrath_factory_text_462e98));
        write_word(a + 76u, address(norrath_factory_text_462e68));
        write_word(a + 80u, address(norrath_factory_text_462e78));
        write_word(a + 12u, flags); write_half(a + 56u, unsigned(type) - 0x4000u);
        write_word(a + 144u, 0xffffffffu); write_word(a + 148u, 0u);
        void* first = norrath_lump_find_resource(reinterpret_cast<char*>(norrath_factory_text_462e68),
                                                reinterpret_cast<char*>(norrath_factory_text_462e78));
        char* second_lump = text(read_word(a + 76u));
        char* second_resource = text(read_word(a + 84u));
        write_word(a + 92u, address(first));
        void* second = norrath_lump_find_resource(second_lump, second_resource);
        const unsigned animation_flags = read_word(a + 148u);
        write_word(a + 96u, address(second));
        char* third_lump = text(read_word(a + 76u));
        char* third_resource = text(read_word(a + 88u));
        const int mode = (animation_flags & 2u) != 0u ? 4 : 1;
        void* third = norrath_lump_find_resource(third_lump, third_resource);
        norrath_actor_animation_add(pointer(animation), third, mode);
        if ((read_word(a + 148u) & 1u) != 0u) norrath_animation_set_to_end(pointer(animation));
        norrath_object_add_to_slow_run_list(object);
        return object;
    }
    // Original selection 0x14f6ec: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483df8)) {
        CREATE(84, norrath_construct_Lever, point, int(type), arguments);
    }
    // Original selection 0x14f72c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462ea8)) {
        CREATE(152, norrath_construct_WeaponRack, point, int(type), arguments);
    }
    // Original selection 0x14f76c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462eb8)) {
        CREATE(280, norrath_construct_PushPhysicsProp, point, type, reinterpret_cast<char*>(norrath_factory_text_462ec8), reinterpret_cast<char*>(norrath_factory_text_462ed8), reinterpret_cast<char*>(norrath_factory_text_462ef8), 24.0f);
    }
    // Original selection 0x14f7cc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462f18)) {
        CREATE(280, norrath_construct_PushPhysicsProp, point, type, reinterpret_cast<char*>(norrath_factory_text_462ec8), reinterpret_cast<char*>(norrath_factory_text_462f28), reinterpret_cast<char*>(norrath_factory_text_462f48), 15.0f);
    }
    // Original selection 0x14f82c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462f68)) {
        CREATE(280, norrath_construct_PushPhysicsPropSimple, point);
    }
    // Original selection 0x14f864: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483e00)) {
        CREATE(80, norrath_construct_Counter, point, arguments);
    }
    // Original selection 0x14f8a4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483e08)) {
        CREATE(84, norrath_construct_Timer, point, arguments);
    }
    // Original selection 0x14f8e4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483e10)) {
        CREATE(140, norrath_construct_Lamp, point);
    }
    // Original selection 0x14f920: prefix; preserves order.
    if (prefix_name(name, norrath_factory_text_483e18)) {
        CREATE(192, norrath_construct_Gold, point, arguments);
    }
    // Original selection 0x14f9cc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462f78)) {
        CREATE(132, norrath_construct_Container, point, type, arguments);
    }
    // Original selection 0x14fa0c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462f88)) {
        unsigned char* object = base_object(96, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned flags = read_word(a + 12u) | 8u;
        write_word(a + 72u, address(norrath_factory_cosmetic_vtable));
        write_half(a + 56u, unsigned(type)); write_word(a + 12u, flags);
        return object;
    }
    // Original selection 0x14fa94: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462f98)) {
        unsigned char* object = base_object(152, saved);
        if (object == nullptr) return nullptr;
        const Address a = address(object);
        const unsigned flags = read_word(a + 12u) | 8u;
        write_word(a + 72u, address(norrath_factory_animation_vtable));
        bytes(a)[105] = 1u; write_word(a + 12u, flags); write_half(a + 56u, unsigned(type));
        return object;
    }
    // Original selection 0x14fb24: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483e20)) {
        CREATE(1144, norrath_construct_NPC, point, type, arguments);
    }
    // Original selection 0x14fb64: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462fb0)) {
        CREATE(960, norrath_construct_Teleporter, point, type, arguments);
    }
    // Original selection 0x14fba4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b68)) {
        CREATE(76, norrath_construct_Blocker, point, type, arguments);
    }
    // Original selection 0x14fbe4: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483b60)) {
        norrath_game_line_add(point, type, arguments);
        if (norrath_object_find_tag_string(arguments, reinterpret_cast<char*>(norrath_factory_text_483ba8), 0) == nullptr)
            return nullptr;
        char* fresh = norrath_object_find_tag_string(arguments, reinterpret_cast<char*>(norrath_factory_text_483ba8), 0);
        if (!equal_name(fresh, norrath_factory_text_483e28)) return nullptr;
        CREATE(1896, norrath_construct_WaveMaker, point, type, arguments);
    }
    // Original selection 0x14fc7c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462fc0)) {
        CREATE(2368, norrath_construct_LavaTractor, point, type, arguments);
    }
    // Original selection 0x14fcbc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_483e30)) {
        CREATE(2360, norrath_construct_Boat, point, type, arguments);
    }
    // Original selection 0x14fcfc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462fd0)) {
        CREATE(2360, norrath_construct_SkulBoat, point, type, arguments);
    }
    // Original selection 0x14fd3c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_461fd0)) {
        CREATE(88, norrath_construct_FireBomb, point, type, arguments);
    }
    // Original selection 0x14fd7c: eq; preserves order.
    if (equal_name(name, norrath_factory_text_461fe0)) {
        CREATE(1192, norrath_construct_StaticFire, point, type, arguments);
    }
    // Original selection 0x14fdbc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462fe0)) {
        CREATE(252, norrath_construct_Catapult, point, type, arguments);
    }
    // Original selection 0x14fdfc: eq; preserves order.
    if (equal_name(name, norrath_factory_text_462ff0)) {
        CREATE(440, norrath_construct_HateGate, name, point, type, arguments);
    }
    // Original selection 0x14fe44: eq; preserves order.
    if (equal_name(name, norrath_factory_text_463000)) {
        CREATE(440, norrath_construct_HateInto, name, point, type, arguments);
    }
    // Actual ordered mutable fallback tags, 0x14fe88..0x1500d7.
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_463010), 0) != 0) {
        if (norrath_has_light_or_particle(arguments) != 0)
            CREATE(336, norrath_construct_AutoContainerLP, name, point, type, arguments);
        CREATE(328, norrath_construct_AutoContainer, name, point, type, arguments);
    }
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_463020), 0) != 0) {
        if (norrath_has_light_or_particle(arguments) != 0)
            CREATE(256, norrath_construct_AutoChestLP, name, point, type, arguments);
        CREATE(248, norrath_construct_AutoChest, name, point, type, arguments);
    }
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_463030), 0) != 0) {
        if (norrath_has_light_or_particle(arguments) != 0)
            CREATE(1096, norrath_construct_AutoPropLP, name, point, type, arguments);
        CREATE(408, norrath_construct_AutoProp, name, point, type, arguments);
    }
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_463040), 0) != 0)
        CREATE(408, norrath_construct_AutoProp, name, point, type, arguments);
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_463050), 0) != 0)
        CREATE(408, norrath_construct_AutoProp, name, point, type, arguments);
    if (norrath_object_find_tag_int(arguments, reinterpret_cast<char*>(norrath_factory_text_483e38), 0) != 0)
        CREATE(1088, norrath_construct_Critter, name, point, type, arguments);
    if (norrath_object_find_tag_string(arguments, reinterpret_cast<char*>(norrath_factory_text_463060), 0) != nullptr)
        CREATE(992, norrath_construct_Generator, point, type, arguments);
    return nullptr;
}
#undef CREATE
