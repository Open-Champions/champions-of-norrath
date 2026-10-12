#include "detail.hpp"
// __6BadgerG6Point3sPPci at 0x1eef40, 924 bytes.
// Observed: Creature receives the snapshot, the short, the 176-byte record at
// 0x42b1d0 + 176 * kind as its map, the single 10.0 and the argument list. Byte
// 40 is 40, bit 2 goes into the high word at +164, _vt$6Badger (0x44c350) to +72,
// 35.0 to +28, the word at 0x4844a4 to +248, byte 172 is 3, 1.0 to +168, +176,
// +192, +236 and half 572 are cleared and byte 964 is the low byte of kind.
// When that byte (as a signed value) is 1: 1.0 to +92, the singles at 0x480cc8 and
// 0x480ccc to +88 and +224, -10.0 to +228, 25.0 to +28, and setParameters gets
// 6.0, the single at 0x480cd0, 3.0, the single at 0x480cd4, 2.0 and 1. Otherwise
// byte 40 becomes 45, -15.0 goes to +228 and the level text at 0x34b6b1 is compared
// with the text at 0x46dbe0: on a match setParameters gets 10.0, the single at
// 0x480cd8, 3.0, 3.0, 4.5 and 1, else 10.0, the single at 0x480cdc, 2.5, the
// single at 0x480ce0, 2.0 and 1. Word 244 is cleared and the eight-byte
// modelGetActiveChunks(model->+12, 0) is stored at +552; the animation state at
// +288 is initialised and receives the header word at model+132 with flag 1; the
// object joins the slow run list. Seven setAnimEventFlags calls follow (4, 5, 6, 7,
// 10, 11 and 12) and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_Badger(void* object, const void* point, short type, char** arguments, int kind) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type,
                                     pointer(address(norrath_ctors_ant_lantern_view_42b1d0) + static_cast<unsigned>(kind) * 176u),
                                     10.0f, arguments);
    const Address a = address(object);
    bytes(a)[40] = 40u;
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44c350));
    write_word(a + 28u, 0x420c0000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    bytes(a)[172] = 3u;
    write_word(a + 176u, 0u);
    write_word(a + 168u, 0x3f800000u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    bytes(a)[964] = static_cast<unsigned char>(kind);
    if (static_cast<signed char>(static_cast<unsigned char>(kind)) == 1) {
        write_word(a + 92u, 0x3f800000u);
        write_word(a + 88u, read_word(address(norrath_ctors_ant_lantern_view_480cc8)));
        write_word(a + 224u, read_word(address(norrath_ctors_ant_lantern_view_480ccc)));
        write_word(a + 228u, 0xc1200000u);
        write_word(a + 28u, 0x41c80000u);
        (void)norrath_actor_set_parameters(object, 6.0f, read_single(norrath_ctors_ant_lantern_view_480cd0), 3.0f,
                                           read_single(norrath_ctors_ant_lantern_view_480cd4), 2.0f, 1);
    } else {
        bytes(a)[40] = 45u;
        write_word(a + 228u, 0xc1700000u);
        if (strcmp(reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_34b6b1),
                   reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46dbe0)) == 0) {
            (void)norrath_actor_set_parameters(object, 10.0f, read_single(norrath_ctors_ant_lantern_view_480cd8), 3.0f, 3.0f, 4.5f, 1);
        } else {
            (void)norrath_actor_set_parameters(object, 10.0f, read_single(norrath_ctors_ant_lantern_view_480cdc), 2.5f,
                                               read_single(norrath_ctors_ant_lantern_view_480ce0), 2.0f, 1);
        }
    }
    write_word(a + 244u, 0u);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(address(model) + 12u)), 0);
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const float first = read_single(norrath_ctors_ant_lantern_view_480ce4);
    const float second = read_single(norrath_ctors_ant_lantern_view_480ce8);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 132u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 4, first, 1, second, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, first, 1, second, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, first, 1, second, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 7, first, 1, second, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 10, 0.5f, 1, read_single(norrath_ctors_ant_lantern_view_480cec), 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, 1.0f, 5, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 12, 1.0f, 5, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
