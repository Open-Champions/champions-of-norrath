#include "detail.hpp"
// __6CthuluG6Point3sPPc at 0x280a88, 796 bytes.
// Observed: Creature receives the snapshot, the short, the table at 0x438a38 as
// its map, the single 10.0 and the argument list. _vt$6Cthulu (0x450160) goes to
// +72, the word at 0x4844a4 to +248, 20.0 to +28, 0.5 to +224, 1.0 to +168, and
// +176, +192, +236, +256 and half 572 are cleared. A roll scaled by 2^-16 and the
// single at 0x482328 is widened with fptodp, added to the double at 0x476ac8 with
// dpadd and narrowed with dptofp; the result v is stored at +224, multiplied into
// the single already at +28 and times -15.0 stored at +228. Another eRand gives
// +960 = (word at +808) + (r % 600); +968 is cleared; +964 and +972 take the low
// bit of the next two rolls. Bit 0x2000 goes into the low word at +160 and 2 into
// the high word at +164. setParameters receives the singles at 0x482334 and
// 0x482338, three 1.0 values and 1. The animation state at +288 is initialised
// and receives the header word at model+76 with flag 1; the object joins the slow
// run list. Seven setAnimEventFlags calls follow (3, 4, 5, 6, 11, 18, then 14 with
// a2 = 4) and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_Cthulu(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_438a38, 10.0f, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_450160));
    write_word(a + 28u, 0x41a00000u);
    write_word(a + 224u, 0x3f000000u);
    write_word(a + 168u, 0x3f800000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 256u, 0u);
    const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
    const float scaled = roll * read_single(norrath_ctors_ant_lantern_view_482328);
    const unsigned long long constant =
        static_cast<unsigned long long>(read_word(address(norrath_ctors_ant_lantern_view_476ac8))) |
        (static_cast<unsigned long long>(read_word(address(norrath_ctors_ant_lantern_view_476ac8) + 4u)) << 32);
    const float value = dptofp(dpadd(norrath_entity_fptodp(scaled), constant));
    float stored;
    {
        const unsigned word = read_word(a + 28u);
        __builtin_memcpy(&stored, &word, 4);
    }
    const float times = stored * value;
    const float behind = value * -15.0f;
    unsigned bits_out;
    __builtin_memcpy(&bits_out, &value, 4);
    write_word(a + 224u, bits_out);
    __builtin_memcpy(&bits_out, &times, 4);
    write_word(a + 28u, bits_out);
    __builtin_memcpy(&bits_out, &behind, 4);
    write_word(a + 228u, bits_out);
    write_word(a + 960u, read_word(a + 808u) + norrath_actor_random(object) % 600u);
    write_word(a + 968u, 0u);
    write_word(a + 964u, norrath_actor_random(object) & 1u);
    const unsigned tail = norrath_actor_random(object) & 1u;
    write_word(a + 160u, read_word(a + 160u) | 0x2000u);
    write_word(a + 164u, read_word(a + 164u) | 0x2u);
    write_word(a + 972u, tail);
    (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_482334),
                                       read_single(norrath_ctors_ant_lantern_view_482338), 1.0f, 1.0f, 1.0f, 1);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 76u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    const float one = read_single(norrath_ctors_ant_lantern_view_48232c);
    const float two = read_single(norrath_ctors_ant_lantern_view_482330);
    setAnimEventFlags__8Creatureififififi(object, 3, one, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, two, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, two, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, read_single(norrath_ctors_ant_lantern_view_48233c), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 11, read_single(norrath_ctors_ant_lantern_view_482340), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 18, one, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 14, 1.0f, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
