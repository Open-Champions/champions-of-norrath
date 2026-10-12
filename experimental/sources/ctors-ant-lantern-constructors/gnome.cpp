#include "detail.hpp"
// __5GnomeG6Point3sPPcPc at 0x2011f8, 472 bytes.
// Observed: Creature receives the point snapshot, the sign-extended short, the
// table at 0x42ba00 as its map, the single 10.0 and the argument list; the last
// argument is the name string. Byte 40 is 15, bit 0x01000000 is ORed into
// object+12 and 0x00040004 into the HIGH word at object+164. 15.0 goes to +28,
// byte 172 is 9, half 792 is 0xffff, the global single at 0x480b10 goes to +168,
// _vt$5Gnome (0x44b9f0) to +72, the word at 0x4844a4 to +248, and +968, +176,
// +192, +236, +972 and half 572 are cleared. setParameters receives the single at
// 0x480b14, the single at 0x480b10, three 1.0 values and the integer 1. Word 964
// is eRand % 3, then replaced by 0, 1 or 2 when the name equals (strticmp) the
// text at 0x46f310, 0x46f320 or 0x46f330 in that order. initType__5Gnome runs;
// the 64-bit modelGetActiveChunks(model->+12, word at +244) is stored at +552;
// the animation state at +288 is initialised and receives the header word at
// model+28 with flag 1; the object joins the slow run list; V0 is
// finishConstruction's.
extern "C" unsigned norrath_construct_Gnome(void* object, const void* point, short type, char** arguments, char* name) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, norrath_ctors_ant_lantern_view_42ba00, 10.0f, arguments);
    const Address a = address(object);
    bytes(a)[40] = 15u;
    write_word(a + 12u, read_word(a + 12u) | 0x01000000u);
    write_word(a + 164u, read_word(a + 164u) | 0x00040004u);
    write_word(a + 28u, 0x41700000u);
    bytes(a)[172] = 9u;
    write_half(a + 792u, 0xffffu);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480b10)));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44b9f0));
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 968u, 0u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 972u, 0u);
    const auto single = [](const unsigned char* view) {
        const unsigned bits = read_word(address(view));
        float value;
        __builtin_memcpy(&value, &bits, 4);
        return value;
    };
    (void)norrath_actor_set_parameters(object, single(norrath_ctors_ant_lantern_view_480b14),
                                       single(norrath_ctors_ant_lantern_view_480b10), 1.0f, 1.0f, 1.0f, 1);
    write_word(a + 964u, norrath_actor_random(object) % 3u);
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46f310)) == 0) {
        write_word(a + 964u, 0u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46f320)) == 0) {
        write_word(a + 964u, 1u);
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46f330)) == 0) {
        write_word(a + 964u, 2u);
    }
    initType__5Gnome(object);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 28u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
