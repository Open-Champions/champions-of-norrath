#include "detail.hpp"
// __8FroglockG6Point3sPPcPc at 0x1ff820, 880 bytes. The last argument is a name
// string.
// Observed: Creature receives the snapshot, the short, the table at 0x42b958 as
// its map, the single 10.0 and the argument list. _vt$8Froglock (0x44bcc0) goes
// to +72; halves at 1092, 1084, 1076 and 1068 are 0xffff, bytes 1064 and 1065 are
// 0xff and the 16 bytes at 1048..1063 are zero. Byte 40 is 35, bit 2 goes into the
// high word at +164, the single at 0x48109c goes to +168, 18.0 to +28, the word at
// 0x4844a4 to +248, the single at 0x4810a0 to +100, byte 172 is 9, +176, +192,
// +236 and half 572 are cleared and +960 is 17. The name is compared with strticmp
// against the text at 0x46f120: on a match +960 stays 17 and a roll scaled by 2^-16
// below the single at 0x4810a4 stores (roll & 1) + 1 at +208. Otherwise the name
// is compared with the text at 0x46f130: on a match +960 becomes 16 and a roll
// below the single at 0x4810a8 stores (roll & 3) + 1 at +208. Then
// initAttackVariant__8Froglock runs, setParameters receives the single at
// 0x4810ac and four 1.0 values and 1, word 244 is cleared, the eight-byte
// modelGetActiveChunks(model->+12, 0) goes to +552, the animation state at +288
// receives the header word at model+60 with flag 1, the object joins the slow run
// list, five setAnimEventFlags calls follow (12, 3, 13, then 4 and 5 with a2 = 16
// and 1.0) and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_Froglock(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42b958, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bcc0));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1092u - index * 8u, 0xffffu);
    bytes(a)[1065] = 0xffu;
    bytes(a)[1064] = 0xffu;
    for (unsigned offset = 1048u; offset < 1064u; offset += 2u) write_half(a + offset, 0u);
    bytes(a)[40] = 35u;
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_48109c)));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 28u, 0x41900000u);
    write_word(a + 100u, read_word(address(norrath_ctors_ant_lantern_view_4810a0)));
    bytes(a)[172] = 9u;
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 960u, 17u);
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46f120)) == 0) {
        write_word(a + 960u, 17u);
        const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
        if (roll < read_single(norrath_ctors_ant_lantern_view_4810a4))
            write_word(a + 208u, (norrath_actor_random(object) & 1u) + 1u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46f130)) == 0) {
        write_word(a + 960u, 16u);
        const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
        if (roll < read_single(norrath_ctors_ant_lantern_view_4810a8))
            write_word(a + 208u, (norrath_actor_random(object) & 3u) + 1u);
    }
    initAttackVariant__8Froglock(object);
    (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_4810ac), 1.0f, 1.0f, 1.0f, 1.0f, 1);
    write_word(a + 244u, 0u);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const float lead = read_single(norrath_ctors_ant_lantern_view_4810b0);
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(address(model) + 12u)), 0);
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 60u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 12, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 13, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
