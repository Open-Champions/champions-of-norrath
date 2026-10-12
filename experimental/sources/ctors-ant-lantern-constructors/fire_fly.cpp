#include "detail.hpp"
// __7FireFlyG6Point3sPPc at 0x2118e8, 484 bytes.
// Observed: FireBeetle (__10FireBeetleG6Point3sPPci) receives a point snapshot,
// the sign-extended short, the argument list and integer 0. _vt$7FireFly
// (0x44bdb0) goes to object+72. The light embedded at object+1064 gets zero at
// +9 and +8, the single 120.0 at +28 and +36, the word at global 0x481540 at +32
// and +40, 1.0 at +44, halves 200/150/50 at +10..+14 and +18..+22, zero at +16
// and +24, and the snapshot is copied to object+1112; lightAdd receives that
// light. setParameters then gets the global word at 0x481544, 1.0, 2.0, 1.0,
// 1.0 and the integer 1. Next: 160.0 to +176, 20.0 to +168, 12.0 to +28, 0.5 to
// +224, the global at 0x4844a4 to +248, bits 0x101000 into the low word of +160,
// half 1 at +572, byte 18 at +172, 360 at +256 and 0 at +236. The animation
// state at object+288 is initialised, receives the header word at
// (*(object+36))+68 with flag 1 and gets a randomised time; the object joins the
// slow run list; halves 51/39/18/0 go to 1082..1088; V0 is
// finishConstruction's.
extern "C" unsigned norrath_construct_FireFly(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_FireBeetle(object, &local, type, arguments, 0);
    const Address a = address(object);
    const Address light = a + 1064u;
    const unsigned tint = read_word(address(norrath_ctors_ant_lantern_view_481540));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bdb0));
    bytes(light)[9] = 0u;
    write_word(a + 1112u, local.x);
    write_word(a + 1116u, local.y);
    write_word(a + 1120u, local.z);
    write_half(light + 22u, 50u);
    write_half(light + 14u, 50u);
    write_word(light + 36u, 0x42f00000u);
    write_word(light + 40u, tint);
    write_half(light + 18u, 200u);
    write_half(light + 20u, 150u);
    write_word(light + 28u, 0x42f00000u);
    write_word(light + 32u, tint);
    write_word(light + 44u, 0x3f800000u);
    write_half(light + 10u, 200u);
    write_half(light + 12u, 150u);
    write_half(light + 16u, 0u);
    write_half(light + 24u, 0u);
    bytes(light)[8] = 0u;
    norrath_light_add(pointer(light));
    const float first = [] {
        const unsigned bits = read_word(address(norrath_ctors_ant_lantern_view_481544));
        float value;
        __builtin_memcpy(&value, &bits, 4);
        return value;
    }();
    (void)norrath_actor_set_parameters(object, first, 1.0f, 2.0f, 1.0f, 1.0f, 1);
    write_word(a + 176u, 0x43200000u);
    write_word(a + 168u, 0x41a00000u);
    write_word(a + 28u, 0x41400000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 224u, 0x3f000000u);
    write_word(a + 160u, read_word(a + 160u) | 0x101000u);
    write_half(a + 572u, 1u);
    bytes(a)[172] = 18u;
    write_word(a + 256u, 360u);
    write_word(a + 236u, 0u);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 68u)), 1);
    norrath_actor_animation_randomize_time(state);
    (void)norrath_actor_add_slow_run_list(object);
    write_half(a + 1082u, 51u);
    write_half(a + 1084u, 39u);
    write_half(a + 1086u, 18u);
    write_half(a + 1088u, 0u);
    return norrath_actor_finish_construction(object);
}
