#include "detail.hpp"
// __8JumpGateG6Point3sPPc at 0x282128, 1016 bytes.
// Observed: GameObject receives the snapshot, a null map and the argument list.
// _vt$8JumpGate (0x4494c0) goes to +72 and byte 221 is 1. The light at
// object+256 gets zero at +9 and +8, 140 in halves 10, 12, 18 and 20, 300 in halves
// 14 and 22, -16 in halves 16 and 24, 240.0 at +28 and +36, the single at 0x48239c
// at +32 and +40, 0.0 at +44, and the point (x + 0.0, y, z + 84.0) is copied to
// object+304; lightAdd runs. Byte 40 is 72 and bits 0x10008 are ORed into +12. If
// the integer tag at 0x476b70 is zero, map::addIcon is called with the snapshot, 0,
// the text at 0x486038, 0 and four null pointers; either way half 56 is the short.
// Bytes 77, 78 and 208 are cleared and byte 79 is the integer tag at 0x486040.
// lightDeactivate runs and the object joins the run list. The nine bytes at +80 are
// copied from the text at 0x476b80 when byte 79 (signed) is nonzero and from the
// text at 0x476b90 otherwise; a string tag at 0x486048 replaces them with strcpy.
// Three formats (0x486050, 0x486058, 0x486060) are printed with that name into
// object+144, a stack buffer and another stack buffer; lumpFindResource(object+144,
// first buffer) goes to +348 and lumpFindResource(object+144, second buffer) to
// +352. A nonzero byte 79 activates the light and stores 60.0 at +212. If the word
// at 0x486030 is nonzero and the word at 0x486da4 is zero, the word at 0x484538
// gives a count: for each index the position at object+16 is passed to
// objectFindEmptySpot(36, 24, null), the result is stored in the 12-byte table at
// 0x429410, and the word at 0x483e5c is advanced as a linear congruential
// generator (state * 0x19660d + 0x3c6ef35f); its high half is stored in the half
// table at 0x484f38. After the loop, or with a count of zero or less, the byte at
// 0x484ca8 becomes 3. V0 is not established; this returns 0.
extern "C" unsigned norrath_construct_JumpGate(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const Address light = a + 256u;
    float x, z;
    __builtin_memcpy(&x, &local.x, 4);
    __builtin_memcpy(&z, &local.z, 4);
    const float zero = 0.0f;
    const float lifted_x = x + zero;
    const float lifted_z = z + 84.0f;
    unsigned lifted_x_bits, lifted_z_bits;
    __builtin_memcpy(&lifted_x_bits, &lifted_x, 4);
    __builtin_memcpy(&lifted_z_bits, &lifted_z, 4);
    bytes(a)[221] = 1u;
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_4494c0));
    const unsigned tint = read_word(address(norrath_ctors_ant_lantern_view_48239c));
    bytes(light)[9] = 0u;
    write_word(a + 304u, lifted_x_bits);
    write_word(a + 308u, local.y);
    write_word(a + 312u, lifted_z_bits);
    write_half(light + 20u, 140u);
    write_half(light + 10u, 140u);
    write_half(light + 12u, 140u);
    write_half(light + 18u, 140u);
    write_word(light + 36u, 0x43700000u);
    write_word(light + 40u, tint);
    write_word(light + 44u, 0u);
    write_half(light + 22u, 300u);
    write_half(light + 24u, 0xfff0u);
    write_word(light + 28u, 0x43700000u);
    write_word(light + 32u, tint);
    write_half(light + 14u, 300u);
    write_half(light + 16u, 0xfff0u);
    bytes(light)[8] = 0u;
    norrath_light_add(pointer(light));
    bytes(a)[40] = 72u;
    write_word(a + 12u, read_word(a + 12u) | 0x10008u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_476b70), 0) == 0)
        addIcon__3mapP6Point3iPciT3N33(&local, 0, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486038), 0,
                                       nullptr, nullptr, nullptr, nullptr);
    write_half(a + 56u, static_cast<unsigned>(type));
    bytes(a)[77] = 0u;
    bytes(a)[78] = 0u;
    const int flag = norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486040), 0);
    bytes(a)[208] = 0u;
    bytes(a)[79] = static_cast<unsigned char>(flag);
    lightDeactivate__FP5Light(pointer(light));
    (void)norrath_actor_add_run_list(object);
    char* const name = reinterpret_cast<char*>(pointer(a + 80u));
    const unsigned char* source = static_cast<signed char>(bytes(a)[79]) != 0 ? norrath_ctors_ant_lantern_view_476b80
                                                                             : norrath_ctors_ant_lantern_view_476b90;
    for (unsigned index = 0u; index < 9u; ++index) bytes(a + 80u)[index] = source[index];
    char* const name_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486048);
    if (norrath_actor_find_tag_string(object, name_tag, 0) != nullptr)
        strcpy(name, norrath_actor_find_tag_string(object, name_tag, 0));
    char* const lump_name = reinterpret_cast<char*>(pointer(a + 144u));
    char first_buffer[128];
    char second_buffer[128];
    norrath_factory_sprintf(lump_name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_486050), name);
    norrath_factory_sprintf(first_buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_486058), name);
    norrath_factory_sprintf(second_buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_486060), name);
    write_word(a + 348u, address_bits(norrath_lump_find_resource(lump_name, first_buffer)));
    write_word(a + 352u, address_bits(norrath_lump_find_resource(lump_name, second_buffer)));
    if (static_cast<signed char>(bytes(a)[79]) != 0) {
        lightActivate__FP5Light(pointer(light));
        write_word(a + 212u, 0x42700000u);
    }
    if (read_word(address(norrath_ctors_ant_lantern_view_486030)) == 0u) return 0u;
    if (read_word(address(norrath_ctors_ant_lantern_view_486da4)) != 0u) return 0u;
    if (static_cast<int>(read_word(address(norrath_creature_view_484538))) > 0) {
        for (unsigned index = 0u;;) {
            const PointBytes position = snapshot_point(pointer(a + 16u));
            unsigned char spot[12];
            objectFindEmptySpot__FG6Point3iiPc(spot, &position, 36, 24, nullptr);
            ++index;
            const Address entry = address(norrath_player_view_429410) + (index - 1u) * 12u;
            for (unsigned byte = 0u; byte < 12u; ++byte) bytes(entry)[byte] = spot[byte];
            const unsigned state = read_word(address(norrath_ctors_ant_lantern_view_483e5c)) * 0x19660du + 0x3c6ef35fu;
            write_word(address(norrath_ctors_ant_lantern_view_483e5c), state);
            write_half(address(norrath_player_view_484f38) + (index - 1u) * 2u, state >> 16);
            if (!(static_cast<int>(index) < static_cast<int>(read_word(address(norrath_creature_view_484538))))) break;
        }
    }
    bytes(address(norrath_player_view_484ca8))[0] = 3u;
    return 0u;
}
