#include "detail.hpp"
// __10BlackWidowG6Point3sPPcPc at 0x209c20, 896 bytes. The last argument is a
// name string.
// Observed: two eRand rolls give the Creature scale (r1 & 7) + (r2 & 7) + 32 as
// a single. Creature receives the snapshot, the short, the table at 0x42c188 as
// its map and the argument list. _vt$10BlackWidow (0x44c260) goes to +72, a Web
// (__3Webii) is constructed at object+976 with 35 and 180, half 56 is the short, bits
// 0x1000 go into the low word at +160 and 0x80 into the high word at +164, 140.0
// to +176, the single at 0x4813bc to +168, the word at 0x4844a4 to +248, byte 172
// is 18, 15.0 to +28, 0.5 to +224, the single at 0x4813c0 to +96 and +236, +972,
// +960 and half 572 are cleared. setParameters receives 1.0, the single at
// 0x4813c4, three 1.0 values and 1. The animation state at +288 is initialised.
// When the name equals (strticmp) the text at 0x46ffe0 the header at model+148 is
// added with flag 4, bit 1 is ORed into +12, 0x01200800 into the low word at +160
// and half 572 becomes 16; otherwise the header at model+116 is added with flag 1,
// the time is randomised and half 572 is cleared. The eight-byte
// modelGetActiveChunks(model->+12, 0) goes to +552, the object joins the slow run
// list, eight setAnimEventFlags calls follow (16, 4, 6, 8, 9, 10, 11, 18) and V0
// is finishConstruction's.
extern "C" unsigned norrath_construct_BlackWidow(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned r1 = norrath_actor_random(object);
    const unsigned r2 = norrath_actor_random(object);
    const float scale = static_cast<float>(static_cast<int>((r1 & 7u) + (r2 & 7u) + 32u));
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42c188, scale, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44c260));
    __3Webii(pointer(a + 976u), 35, 180);
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 160u, read_word(a + 160u) | 0x1000u);
    write_word(a + 164u, read_word(a + 164u) | 0x80u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 176u, 0x430c0000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_4813bc)));
    bytes(a)[172] = 18u;
    write_word(a + 28u, 0x41700000u);
    write_word(a + 224u, 0x3f000000u);
    write_word(a + 96u, read_word(address(norrath_ctors_ant_lantern_view_4813c0)));
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 972u, 0u);
    write_word(a + 960u, 0u);
    (void)norrath_actor_set_parameters(object, 1.0f, read_single(norrath_ctors_ant_lantern_view_4813c4), 1.0f, 1.0f, 1.0f, 1);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46ffe0)) == 0) {
        norrath_actor_animation_add(state, pointer(read_word(address(model) + 148u)), 4);
        write_word(a + 12u, read_word(a + 12u) | 0x1u);
        write_word(a + 160u, read_word(a + 160u) | 0x01200800u);
        write_half(a + 572u, 16u);
    } else {
        norrath_actor_animation_add(state, pointer(read_word(address(model) + 116u)), 1);
        norrath_actor_animation_randomize_time(state);
        write_half(a + 572u, 0u);
    }
    const float lead = read_single(norrath_ctors_ant_lantern_view_4813c8);
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(address(model) + 12u)), 0);
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 16, read_single(norrath_ctors_ant_lantern_view_4813cc), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, 0.5f, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, read_single(norrath_ctors_ant_lantern_view_4813d0), 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, lead, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 9, lead, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 10, lead, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, lead, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 18, read_single(norrath_ctors_ant_lantern_view_4813d4), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
