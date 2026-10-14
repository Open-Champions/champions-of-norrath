#include "detail.hpp"
// __8CatapultG6Point3sPPc at 0x28b858, 944 bytes.
// Observed: GameObject receives the snapshot, the table at 0x438eb8 as its map and
// the argument list. _vt$8Catapult (0x4506b0) goes to +72, byte 97 is 4, words
// 248, 28, 12 and 88 are cleared, byte 40 is 72, half 56 is the short and bytes
// 85, 84 and 78 are cleared. A string tag at 0x4777c8, when present, makes the
// integer tag at the same address, converted to a single and multiplied by the
// single at 0x4825bc, the value at +240; otherwise the single at 0x4825c0 is stored.
// A string tag at 0x4860e0 gives +244 = 60 * its integer tag, else 0. The string
// tag at 0x4860e8 is stored at +80. A string tag at 0x4860f0 gives byte 86 = its
// integer tag, else 0. The object joins the run list, the animation state at +92
// is initialised and byte 87 is cleared. An integer tag at 0x4860f8 clears +244;
// otherwise lumpFindResource(0x477798, 0x4777d8) is added to the state with flag 1.
// A nonzero byte 86 ends the constructor. Otherwise, with c, s = icos/isin of the
// short and c2, s2 = icos/isin of the short + 16384, blocks are built for
// i = -3..4 and j = -1..1, skipping j = 0 when i is in -2..3. Each block is a
// 76-byte allocation constructed as a Blocker at object+16 (snapshot, short, null
// arguments), its x and y are moved by (c * 22.5 * i + c2 * 22.5 * j) and
// (s * 22.5 * i + s2 * 22.5 * j), it ORs 0x4000 into its +12, takes 30.0 at +28
// (38.0 when i is negative) and is linked into the list at object+248. V0 is not
// established; this returns 0.
extern "C" unsigned norrath_construct_Catapult(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, norrath_ctors_ant_lantern_view_438eb8, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_4506b0));
    bytes(a)[97] = 4u;
    write_word(a + 248u, 0u);
    bytes(a)[40] = 72u;
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 28u, 0u);
    write_word(a + 12u, 0u);
    write_word(a + 88u, 0u);
    bytes(a)[85] = 0u;
    bytes(a)[84] = 0u;
    bytes(a)[78] = 0u;
    char* const speed_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4777c8);
    float speed;
    if (norrath_actor_find_tag_string(object, speed_tag, 0) != nullptr) {
        speed = static_cast<float>(norrath_game_object_find_tag_int(object, speed_tag, 0)) *
                read_single(norrath_ctors_ant_lantern_view_4825bc);
    } else {
        speed = read_single(norrath_ctors_ant_lantern_view_4825c0);
    }
    unsigned speed_bits;
    __builtin_memcpy(&speed_bits, &speed, 4);
    write_word(a + 240u, speed_bits);
    char* const delay_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860e0);
    if (norrath_actor_find_tag_string(object, delay_tag, 0) != nullptr)
        write_word(a + 244u, static_cast<unsigned>(norrath_game_object_find_tag_int(object, delay_tag, 0) * 60));
    else
        write_word(a + 244u, 0u);
    write_word(a + 80u, address_bits(norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860e8), 0)));
    char* const flag_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860f0);
    if (norrath_actor_find_tag_string(object, flag_tag, 0) != nullptr)
        bytes(a)[86] = static_cast<unsigned char>(norrath_game_object_find_tag_int(object, flag_tag, 0));
    else
        bytes(a)[86] = 0u;
    (void)norrath_actor_add_run_list(object);
    void* const state = pointer(a + 92u);
    norrath_actor_animation_initialize(state);
    bytes(a)[87] = 0u;
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860f8), 0) != 0) {
        write_word(a + 244u, 0u);
    } else {
        void* const header = norrath_lump_find_resource(reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_477798),
                                                      reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4777d8));
        norrath_actor_animation_add(state, header, 1);
    }
    if (static_cast<signed char>(bytes(a)[86]) != 0) return 0u;
    const int angle = static_cast<short>(bytes(a)[56] | (bytes(a)[57] << 8));
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    const float c2 = icos__Fi(angle + 16384);
    const float s2 = isin__Fi(angle + 16384);
    for (int i = -3; i < 5; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (static_cast<unsigned>(i + 2) < 6u && j == 0) continue;
            unsigned char* block = norrath_block_alloc(76);
            if (block == nullptr) continue;
            const PointBytes origin = snapshot_point(pointer(a + 16u));
            (void)norrath_construct_Blocker(block, &origin, static_cast<short>(angle), nullptr);
            const Address b = address(block);
            const float scaled_j = static_cast<float>(j) * 22.5f;
            const float scaled_i = static_cast<float>(i) * 22.5f;
            write_word(b + 12u, read_word(b + 12u) | 0x4000u);
            const float x = read_single(block + 16) + (c * scaled_i + c2 * scaled_j);
            const float y = read_single(block + 20) + (s * scaled_i + s2 * scaled_j);
            unsigned bits_out;
            __builtin_memcpy(&bits_out, &x, 4);
            write_word(b + 16u, bits_out);
            __builtin_memcpy(&bits_out, &y, 4);
            write_word(b + 20u, bits_out);
            const float height = i >= 0 ? 30.0f : 30.0f + 8.0f;
            __builtin_memcpy(&bits_out, &height, 4);
            write_word(b + 28u, bits_out);
            (void)norrath_list_oneway_link(pointer(a + 248u), block, block);
        }
    }
    return 0u;
}
