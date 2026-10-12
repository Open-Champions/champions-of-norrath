#include "detail.hpp"
// __10CloudGiantG6Point3sPPcPc at 0x1e6b30, 1404 bytes. The last argument is a
// name string.
// Observed: Creature receives the snapshot, the short, the table at 0x42ae60 as
// its map, the single 10.0 and the argument list. _vt$10CloudGiant (0x44dbe8) goes
// to +72. Two blocks of four 0xffff halves, two marker bytes and a 16-byte zero run
// follow: halves at 1116..1092 with markers at 1088/1089 and zeros at 1072..1087,
// then halves at 1252..1228 with markers at 1224/1225 and zeros at 1208..1223.
// Bytes 40 and 41 are 75 and 50, 0x40000 goes into the low word at +160 and 2 into
// the high word at +164, the word at 0x4844a4 to +248, the single at 0x480b20 to
// +168, 35.0 to +28, the single at 0x480b24 to +104, word 1308 is -1 and +176,
// +192, +236, +256, half 572, +208, +1304 and +244 are cleared; +984 is 1. The name
// is compared (strticmp) with the text at 0x46d300. On a match: +208 and +976
// become 1, +980 is cleared, and the first of the four 8-byte entries at 1228 whose
// head is 0xffff or 24 gets head 24, halves 10 and 10 at +2 and +4 and 0 at +6;
// 0x20000000 is ORed into the low word at +160, setParameters receives 60.0, 2.0,
// 4.0, the single at 0x480b28, 10.5 and 1, and +788 becomes 2000.0. Otherwise a
// roll gives +244 = 1 << (roll % 3) and setParameters receives 6.0, 2.0, the single
// at 0x480b2c, 1.0, 1.0 and 1. The eight-byte modelGetActiveChunks(model->+12,
// +244) is stored at +552; a roll scaled by 2^-16 below the single at 0x480b30
// stores 1 at +208; 2.0 goes to +224, the animation state at +288 receives the
// header word at model+132, the object joins the slow run list, eleven
// setAnimEventFlags calls follow (18, 19, 4, 20, 12, 13, 7, 8, 6, 10, 11) and V0 is
// finishConstruction's.
extern "C" unsigned norrath_construct_CloudGiant(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42ae60, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44dbe8));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1116u - index * 8u, 0xffffu);
    bytes(a)[1089] = 0xffu;
    bytes(a)[1088] = 0xffu;
    for (unsigned offset = 1072u; offset < 1088u; offset += 2u) write_half(a + offset, 0u);
    const Address second = a + 1192u;
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1252u - index * 8u, 0xffffu);
    bytes(second)[33] = 0xffu;
    bytes(second)[32] = 0xffu;
    for (unsigned offset = 16u; offset < 32u; offset += 2u) write_half(second + offset, 0u);
    bytes(a)[40] = 75u;
    bytes(a)[41] = 50u;
    write_word(a + 160u, read_word(a + 160u) | 0x40000u);
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480b20)));
    write_word(a + 28u, 0x420c0000u);
    write_word(a + 1308u, 0xffffffffu);
    write_word(a + 104u, read_word(address(norrath_ctors_ant_lantern_view_480b24)));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    write_word(a + 984u, 1u);
    write_word(a + 208u, 0u);
    write_word(a + 1304u, 0u);
    write_word(a + 244u, 0u);
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46d300)) == 0) {
        write_word(a + 208u, 1u);
        write_word(a + 976u, 1u);
        write_word(a + 980u, 0u);
        for (unsigned slot = 0u; slot < 4u; ++slot) {
            const Address entry = second + 36u + slot * 8u;
            const unsigned head = bytes(entry)[0] | (bytes(entry)[1] << 8);
            if (head == 0xffffu || head == 24u) {
                write_half(entry, 24u);
                write_half(entry + 6u, 0u);
                write_half(entry + 2u, 10u);
                write_half(entry + 4u, 10u);
                break;
            }
        }
        write_word(a + 160u, read_word(a + 160u) | 0x20000000u);
        (void)norrath_actor_set_parameters(object, 60.0f, 2.0f, 4.0f, read_single(norrath_ctors_ant_lantern_view_480b28), 10.5f, 1);
        write_word(a + 788u, 0x44fa0000u);
    } else {
        const unsigned roll = norrath_actor_random(object);
        write_word(a + 244u, 1u << (roll % 3u));
        (void)norrath_actor_set_parameters(object, 6.0f, 2.0f, read_single(norrath_ctors_ant_lantern_view_480b2c), 1.0f, 1.0f, 1);
    }
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    const float chance = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
    if (chance < read_single(norrath_ctors_ant_lantern_view_480b30)) write_word(a + 208u, 1u);
    write_word(a + 224u, 0x40000000u);
    void* const state = pointer(a + 288u);
    const float held = read_single(norrath_ctors_ant_lantern_view_480b34);
    norrath_actor_animation_initialize(state);
    const float soft = read_single(norrath_ctors_ant_lantern_view_480b38);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 132u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 18, read_single(norrath_ctors_ant_lantern_view_480b3c), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 19, read_single(norrath_ctors_ant_lantern_view_480b40), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, read_single(norrath_ctors_ant_lantern_view_480b44), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 20, read_single(norrath_ctors_ant_lantern_view_480b48), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 12, held, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 13, held, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 7, read_single(norrath_ctors_ant_lantern_view_480b4c), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, 1.0f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 10, 2.0f, 1, soft, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, 2.0f, 1, soft, 2, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
