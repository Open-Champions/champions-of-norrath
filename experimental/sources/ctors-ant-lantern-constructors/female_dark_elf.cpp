#include "detail.hpp"
// __13FemaleDarkElfG6Point3sPPcPc at 0x284f20, 1160 bytes. The last argument is a
// name string.
// Observed: Creature receives the snapshot, the short, the table at 0x438b28 as
// its map, the single 10.0 and the argument list. _vt$13FemaleDarkElf (0x450530)
// goes to +72; halves at 1092, 1084, 1076 and 1068 are 0xffff, bytes 1064 and 1065
// are 0xff and the 16 bytes at 1048..1063 are zero. Byte 40 is 25, the single at
// 0x482414 goes to +168, 15.0 to +28, the word at 0x4844a4 to +248, byte 172 is 9
// and +176, +192, +236 and half 572 are cleared. The low bit of an eRand roll goes
// to byte 960; the name compared (strticmp) with the text at 0x4770d0 sets it to 0,
// else with the text at 0x4770e8 sets it to 1. Words 208, 964, 1104 and 244 are
// cleared. When byte 960 (signed) is 1, +244 = 1 << (roll & 3). Otherwise +244 = 1 <<
// ((roll % 14) + 4), +964 = (roll & 2) + 29 and +1104 = (roll % 3) * 2 + 33, and a
// roll scaled by 2^-16 below the single at 0x482418 stores (roll % 3) + 1 at +208.
// The eight-byte modelGetActiveChunks(model->+12, word at +244) goes to +552, bit 8
// goes into the high word at +164, initType__13FemaleDarkElfb(object, 0) runs, the
// animation state at +288 receives the header word at model+212 with flag 1 and
// the object joins the slow run list. Ten setAnimEventFlags calls follow (18, 19,
// 20, 21 with the single at 0x48241c; 4, 6, 8, 10 with a2 = 16 and 1.0; 13 and 14
// with the single at 0x482420) and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_FemaleDarkElf(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_438b28, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_450530));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1092u - index * 8u, 0xffffu);
    bytes(a)[1065] = 0xffu;
    bytes(a)[1064] = 0xffu;
    for (unsigned offset = 1048u; offset < 1064u; offset += 2u) write_half(a + offset, 0u);
    bytes(a)[40] = 25u;
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_482414)));
    write_word(a + 28u, 0x41700000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    bytes(a)[172] = 9u;
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    bytes(a)[960] = static_cast<unsigned char>(norrath_actor_random(object) & 1u);
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4770d0)) == 0) {
        bytes(a)[960] = 0u;
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4770e8)) == 0) {
        bytes(a)[960] = 1u;
    }
    write_word(a + 208u, 0u);
    write_word(a + 964u, 0u);
    write_word(a + 1104u, 0u);
    write_word(a + 244u, 0u);
    if (static_cast<signed char>(bytes(a)[960]) == 1) {
        write_word(a + 244u, 1u << (norrath_actor_random(object) & 3u));
    } else {
        const unsigned first = norrath_actor_random(object);
        write_word(a + 244u, 1u << (first % 14u + 4u));
        const unsigned second = norrath_actor_random(object);
        write_word(a + 964u, (second & 2u) + 29u);
        const unsigned third = norrath_actor_random(object);
        write_word(a + 1104u, (third % 3u) * 2u + 33u);
        const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
        if (roll < read_single(norrath_ctors_ant_lantern_view_482418))
            write_word(a + 208u, norrath_actor_random(object) % 3u + 1u);
    }
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    write_word(a + 164u, read_word(a + 164u) | 0x8u);
    const float lead = read_single(norrath_ctors_ant_lantern_view_48241c);
    initType__13FemaleDarkElfb(object, 0);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 212u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float tail = read_single(norrath_ctors_ant_lantern_view_482420);
    setAnimEventFlags__8Creatureififififi(object, 18, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 19, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 20, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 21, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 10, 1.0f, 16, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 13, tail, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 14, tail, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
