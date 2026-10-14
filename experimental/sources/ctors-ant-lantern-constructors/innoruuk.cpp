#include "detail.hpp"
// __8InnoruukG6Point3sPPc at 0x1f8c08, 1348 bytes.
// Observed: Creature receives the snapshot, the short, the table at 0x42b870 as
// its map, the single 10.0 and the argument list. _vt$8Innoruuk (0x44e550) goes
// to +72 and +988 is cleared. 1.0 divided by 0.0 (a runtime single divide, the
// value is not folded) and the single at 0x480f4c fill the light at object+1016:
// byte 9 and byte 8 are zero, halves 18 and 10 are 115, 20 and 12 are 15, 24 and
// 16 are -32, 14 and 22 are 0, +28 and +36 are 0.0, +32 and +40 take the quotient
// and +44 the single at 0x480f4c. The snapshot, as Creature left it, is copied to
// +1064 and +1076 is cleared. lightAdd runs with that light. Halves at 1204, 1196,
// 1188 and 1180 are 0xffff, bytes 1176 and 1177 are 0xff and the 16 bytes at
// 1160..1175 are zero. Bytes 40 and 41 are 50, 0x10 goes into the high word at +164
// and 0x30040400 into the low word at +160; word 1216 is -1, the singles at
// 0x480f50, 0x480f54 and 0x480f58 go to +88 (also +96 and +100), +104 and +168, 30.0
// to +28, 2.0 to +224, the word at 0x4844a4 to +248, +176, +192, +236, +256, +964,
// +968, +972, +976, +980, +1004 and half 572 are cleared and +960 is 1. An integer
// tag at 0x46edb0 selects the setParameters call and +984: present gives
// (100.0, 100.0, 4.0, [0x480f5c], 3.0, 1) and 1; absent gives (60.0, [0x480f60],
// 4.0, [0x480f64], 10.5, 1) and 0. The level text at 0x34b6b1 equal to the text at
// 0x485270 clears +960. The animation state at +288 is initialised and receives the
// header word at model+124 with flag 1. Twelve setAnimEventFlags calls follow
// (10, 11, 9, 3, 6, 7, 8, 16, 5, 17, 12, 4), the object joins the slow run list and
// V0 is finishConstruction's.
extern "C" unsigned norrath_construct_Innoruuk(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42b870, 10.0f, arguments);
    const Address a = address(object);
    const PointBytes light_point = local;
    volatile float zero = 0.0f;
    const float quotient = 1.0f / zero;
    unsigned quotient_bits;
    __builtin_memcpy(&quotient_bits, &quotient, 4);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44e550));
    write_word(a + 988u, 0u);
    const Address light = a + 1016u;
    bytes(light)[9] = 0u;
    write_word(a + 1064u, light_point.x);
    write_word(a + 1068u, light_point.y);
    write_word(a + 1072u, light_point.z);
    write_half(light + 18u, 115u);
    write_half(light + 10u, 115u);
    write_word(light + 44u, read_word(address(norrath_ctors_ant_lantern_view_480f4c)));
    write_word(light + 40u, quotient_bits);
    write_half(light + 20u, 15u);
    write_half(light + 24u, 0xffe0u);
    write_word(light + 28u, 0u);
    write_word(light + 36u, 0u);
    write_word(light + 32u, quotient_bits);
    write_half(light + 12u, 15u);
    write_half(light + 14u, 0u);
    write_half(light + 16u, 0xffe0u);
    write_half(light + 22u, 0u);
    bytes(light)[8] = 0u;
    norrath_light_add(pointer(light));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1204u - index * 8u, 0xffffu);
    bytes(a)[1177] = 0xffu;
    bytes(a)[1176] = 0xffu;
    for (unsigned offset = 1160u; offset < 1176u; offset += 2u) write_half(a + offset, 0u);
    bytes(a)[41] = 50u;
    bytes(a)[40] = 50u;
    write_word(a + 164u, read_word(a + 164u) | 0x10u);
    write_word(a + 160u, read_word(a + 160u) | 0x30040400u);
    write_word(a + 1216u, 0xffffffffu);
    write_word(a + 88u, read_word(address(norrath_ctors_ant_lantern_view_480f50)));
    write_word(a + 104u, read_word(address(norrath_ctors_ant_lantern_view_480f54)));
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480f58)));
    write_word(a + 28u, 0x41f00000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 224u, 0x40000000u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 100u, read_word(address(norrath_ctors_ant_lantern_view_480f50)));
    write_word(a + 96u, read_word(address(norrath_ctors_ant_lantern_view_480f50)));
    write_word(a + 92u, read_word(address(norrath_ctors_ant_lantern_view_480f54)));
    write_word(a + 960u, 1u);
    write_word(a + 964u, 0u);
    write_word(a + 968u, 0u);
    write_word(a + 972u, 0u);
    write_word(a + 976u, 0u);
    write_word(a + 980u, 0u);
    write_word(a + 1004u, 0u);
    write_word(a + 1076u, 0u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46edb0), 0) != 0) {
        (void)norrath_actor_set_parameters(object, 100.0f, 100.0f, 4.0f, read_single(norrath_ctors_ant_lantern_view_480f5c), 3.0f, 1);
        write_word(a + 984u, 1u);
    } else {
        (void)norrath_actor_set_parameters(object, 60.0f, read_single(norrath_ctors_ant_lantern_view_480f60), 4.0f,
                                           read_single(norrath_ctors_ant_lantern_view_480f64), 10.5f, 1);
        write_word(a + 984u, 0u);
    }
    if (strcmp(reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_34b6b1),
               reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485270)) == 0)
        write_word(a + 960u, 0u);
    void* const state = pointer(a + 288u);
    const float first = read_single(norrath_ctors_ant_lantern_view_480f68);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const float second = read_single(norrath_ctors_ant_lantern_view_480f6c);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 124u)), 1);
    const float third = read_single(norrath_ctors_ant_lantern_view_480f70);
    const float fourth = read_single(norrath_ctors_ant_lantern_view_480f74);
    setAnimEventFlags__8Creatureififififi(object, 10, first, 1, second, 1, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, read_single(norrath_ctors_ant_lantern_view_480f78), 1, third, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 9, read_single(norrath_ctors_ant_lantern_view_480f7c), 1, third, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 3, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, fourth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 7, fourth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, fourth, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 16, 1.0f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, read_single(norrath_ctors_ant_lantern_view_480f80), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 17, read_single(norrath_ctors_ant_lantern_view_480f84), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 12, second, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, first, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
