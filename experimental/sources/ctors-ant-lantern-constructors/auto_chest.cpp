#include "detail.hpp"
// __9AutoChestPcG6Point3sPPc at 0x21eee8, 704 bytes.
// Observed: AutoObject (__10AutoObjectPcG6Point3sPPc) receives this, the name
// string, a point snapshot, the sign-extended short and the argument list. Bits
// 0x00100018 are ORed into object+12, _vt$9AutoChest (0x44ece8) goes to +72, word
// 232 and byte 240 are cleared. If the tag at 0x4854c8 exists, its string is
// compared (case-insensitively) with the texts at 0x4854d0, 0x4854d8 and 0x4854e0;
// a match sets byte 240 to 0, 1 or 2 respectively. Word 236 is cleared and becomes
// 1 when the name argument equals the text at 0x4854e8. Byte 228 is
// (eRand < 2000), forced to 1 by an integer tag at 0x4854f0. A float tag at
// 0x4854f8 (when its string tag exists) scales the roll as in Chest, with
// levelFlags bits 0x10000 (double) and 0x8000 (times 3.0). Half 220 is
// (int)((eRand % 12 + 12) * scale * gameTreasureRamp), cleared when a further
// eRand is below 10000, and mapped to a class 0 / 1 (<40) / 2 (<80) / 4. When the
// name equals the text at 0x46fd38 byte 228 is cleared; otherwise a nonzero byte
// 228 clears half 220 and makes the class 8. The name at +76 is copied into a
// local buffer, the suffix at 0x485500 is appended, lumpFindResource(object+109,
// buffer) goes to +244 and the 64-bit modelGetActiveChunks(word at +144, class) is
// stored at +200. V0 after the last call is not established; this returns 0.
extern "C" unsigned norrath_construct_AutoChest(void* object, char* name, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoObject(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 12u, read_word(a + 12u) | 0x00100018u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44ece8));
    write_word(a + 232u, 0u);
    bytes(a)[240] = 0u;
    char* const mode_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854c8);
    if (norrath_actor_find_tag_string(object, mode_tag, 0) != nullptr) {
        const char* mode = norrath_actor_find_tag_string(object, mode_tag, 0);
        if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4854d0)) == 0) {
            bytes(a)[240] = 0u;
        } else if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4854d8)) == 0) {
            bytes(a)[240] = 1u;
        } else if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4854e0)) == 0) {
            bytes(a)[240] = 2u;
        }
    }
    write_word(a + 236u, 0u);
    if (norrath_factory_strcasecmp(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4854e8)) == 0)
        write_word(a + 236u, 1u);
    bytes(a)[228] = norrath_actor_random(object) < 2000u ? 1u : 0u;
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854f0), 0) != 0)
        bytes(a)[228] = 1u;
    char* const scale_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854f8);
    float scale = 1.0f;
    if (norrath_actor_find_tag_string(object, scale_tag, 0) != nullptr)
        scale = norrath_player_find_tag_float(object, scale_tag, 0);
    const unsigned level_flags = read_word(address(norrath_ctors_ant_lantern_view_4844c0));
    if ((level_flags & 0x10000u) != 0u) scale = scale + scale;
    if ((level_flags & 0x8000u) != 0u) scale = scale * 3.0f;
    const unsigned roll = norrath_actor_random(object);
    const float ramp = norrath_player_treasure_ramp();
    const float amount = static_cast<float>(static_cast<int>(roll % 12u + 12u)) * scale * ramp;
    write_half(a + 220u, static_cast<unsigned>(static_cast<int>(amount)));
    if (norrath_actor_random(object) < 10000u) write_half(a + 220u, 0u);
    const int count = static_cast<short>(bytes(a)[220] | (bytes(a)[221] << 8));
    int chunk_class = 0;
    if (count != 0) chunk_class = count < 40 ? 1 : (count < 80 ? 2 : 4);
    if (norrath_factory_strcasecmp(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46fd38)) == 0) {
        bytes(a)[228] = 0u;
    } else if (static_cast<signed char>(bytes(a)[228]) != 0) {
        write_half(a + 220u, 0u);
        chunk_class = 8;
    }
    char buffer[32];
    strcpy(buffer, reinterpret_cast<const char*>(pointer(a + 76u)));
    strcat(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485500));
    write_word(a + 244u, address_bits(norrath_lump_find_resource(reinterpret_cast<char*>(pointer(a + 109u)), buffer)));
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(a + 144u)), chunk_class);
    write_word(a + 200u, static_cast<unsigned>(active));
    write_word(a + 204u, static_cast<unsigned>(active >> 32));
    return 0u;
}
