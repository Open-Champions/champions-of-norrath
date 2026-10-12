#include "detail.hpp"
// __5GhoulG6Point3siPPc at 0x1f62e8, 852 bytes.
// Observed: three eRand rolls r1, r2, r3 give the Creature scale
// (r1 & 7) + (r2 & 7) + (r3 & 7) + 18 as a single. The map is the table at
// 0x42b630 when the int argument is nonzero and the table at 0x42b548 when it is
// zero. Creature also receives the snapshot, the short and the argument list.
// Bit 2 goes into the high word at +164, _vt$5Ghoul (0x44bae0) to +72, the word
// at 0x4844a4 to +248, 170.0 to +176, byte 172 is 16, the single at 0x480e58 to
// +168, 18.0 to +28, the single at 0x480e5c to +104, 1.0 to +100 and +96, and +236
// and half 572 are cleared. setParameters receives the single at 0x480e60 and four
// 1.0 values and 1. The animation state at +288 is initialised, receives the
// header word at model+172 with flag 1 and gets a randomised time; the object
// joins the slow run list. Eight setAnimEventFlags calls follow (3, 4, 5, 5 with
// a3 = 2, then 17, 18, 19 and 20 with the same floating values and a2 = 1,
// a3 = 2, t0 = 4). The eight-byte modelGetActiveChunks(model->+12, word at +244)
// is stored at +552 and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_Ghoul(void* object, const void* point, short type, int variant, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned r1 = norrath_actor_random(object);
    const unsigned r2 = norrath_actor_random(object);
    const unsigned r3 = norrath_actor_random(object);
    const float scale = static_cast<float>(static_cast<int>((r1 & 7u) + ((r2 & 7u) + (r3 & 7u)) + 18u));
    (void)norrath_creature_construct(object, &local, type,
                                     variant != 0 ? norrath_ctors_ant_lantern_view_42b630 : norrath_ctors_ant_lantern_view_42b548,
                                     scale, arguments);
    const Address a = address(object);
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bae0));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 176u, 0x432a0000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480e58)));
    write_word(a + 28u, 0x41900000u);
    write_word(a + 104u, read_word(address(norrath_ctors_ant_lantern_view_480e5c)));
    write_word(a + 100u, 0x3f800000u);
    write_word(a + 96u, 0x3f800000u);
    bytes(a)[172] = 16u;
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_480e60), 1.0f, 1.0f, 1.0f, 1.0f, 1);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const float lead = read_single(norrath_ctors_ant_lantern_view_480e64);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 172u)), 1);
    norrath_actor_animation_randomize_time(state);
    (void)norrath_actor_add_slow_run_list(object);
    const float second = read_single(norrath_ctors_ant_lantern_view_480e68);
    const float third = read_single(norrath_ctors_ant_lantern_view_480e6c);
    const float fourth = read_single(norrath_ctors_ant_lantern_view_480e70);
    setAnimEventFlags__8Creatureififififi(object, 3, lead, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, lead, 2, second, 2, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 17, third, 1, second, 2, fourth, 4, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 18, third, 1, second, 2, fourth, 4, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 19, third, 1, second, 2, fourth, 4, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 20, third, 1, second, 2, fourth, 4, 0.0f, 0);
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    return norrath_actor_finish_construction(object);
}
