#include "detail.hpp"
// __5ChestG6Point3sPPci at 0x218790, 596 bytes.
// Observed: GameObject receives the point snapshot, the pointer-table entry at
// 0x42c8a0 indexed by the int argument (as the second argument), and the argument
// list. _vt$5Chest (0x44cb98) goes to object+72, byte 97 is 1, half 56 is the
// short and byte 89 is the int's low byte. eRand gives byte 88 = (value < 2000).
// If the text at 0x34b6b1 equals the text at 0x485398 or the text at 0x485420,
// byte 88 becomes 0. A tag string at 0x485438 present on the object turns its
// float tag into a scale (default 1.0). An integer tag at 0x485440 makes byte 88
// 1. Bit 0x10000 of the word at 0x4844c0 doubles the scale and bit 0x8000
// multiplies it by 3.0. Half 76 is (int)((float)(eRand % 12 + 12) * scale * ramp)
// with ramp from gameTreasureRamp, and zero when the text at 0x34b6b1 equals the
// text at 0x485420. Bits 0x00100018 are ORed into object+12, 25.0 is stored at +32
// and 22.0 at +28. Half 76 picks a class value: 0 stays 0, below 40 is 1, 40..79
// is 2, 80 and above is 4; byte 88 nonzero clears half 76 and makes it 8. The low
// word of modelGetActiveChunks(model->+12, class) goes to +84, where model is the
// word at +36, and the animation state at object+92 is initialised. V0 after the
// last call is not established; this returns 0.
extern "C" unsigned norrath_construct_Chest(void* object, const void* point, short type, char** arguments, int kind) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned table_entry = read_word(address(norrath_ctors_ant_lantern_view_42c8a0) + static_cast<unsigned>(kind) * 4u);
    (void)norrath_game_object_construct(object, &local, pointer(table_entry), arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44cb98));
    bytes(a)[97] = 1u;
    write_half(a + 56u, static_cast<unsigned>(type));
    bytes(a)[89] = static_cast<unsigned char>(kind);
    bytes(a)[88] = norrath_actor_random(object) < 2000u ? 1u : 0u;
    const char* level = reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_34b6b1);
    if (strcmp(level, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485398)) == 0) {
        bytes(a)[88] = 0u;
    } else if (strcmp(level, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485420)) == 0) {
        bytes(a)[88] = 0u;
    }
    char* scale_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485438);
    float scale = 1.0f;
    if (norrath_actor_find_tag_string(object, scale_tag, 0) != nullptr)
        scale = norrath_player_find_tag_float(object, scale_tag, 0);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485440), 0) != 0)
        bytes(a)[88] = 1u;
    const unsigned level_flags = read_word(address(norrath_ctors_ant_lantern_view_4844c0));
    if ((level_flags & 0x10000u) != 0u) scale = scale + scale;
    if ((level_flags & 0x8000u) != 0u) scale = scale * 3.0f;
    const unsigned roll = norrath_actor_random(object);
    const float ramp = norrath_player_treasure_ramp();
    const float amount = static_cast<float>(static_cast<int>(roll % 12u + 12u)) * scale * ramp;
    write_half(a + 76u, static_cast<unsigned>(static_cast<int>(amount)));
    if (strcmp(level, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485420)) == 0) write_half(a + 76u, 0u);
    write_word(a + 12u, read_word(a + 12u) | 0x00100018u);
    write_word(a + 32u, 0x41c80000u);
    write_word(a + 28u, 0x41b00000u);
    const int count = static_cast<short>(bytes(a)[76] | (bytes(a)[77] << 8));
    int chunk_class = 0;
    if (count != 0) chunk_class = count < 40 ? 1 : (count < 80 ? 2 : 4);
    if (static_cast<signed char>(bytes(a)[88]) != 0) {
        write_half(a + 76u, 0u);
        chunk_class = 8;
    }
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(address(model) + 12u)), chunk_class);
    write_word(a + 84u, static_cast<unsigned>(active));
    norrath_actor_animation_initialize(pointer(a + 92u));
    return 0u;
}
