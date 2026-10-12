#include "detail.hpp"
// __7LanternG6Point3 at 0x230890, 344 bytes.
// Observed: GameObject receives this, a snapshot of the point, and two null
// arguments. _vt$7Lantern (0x44a8a0) goes to object+72. The 12 point bytes are
// copied to object+124. The light embedded at object+76 gets zero at +9 and +8,
// the single 240.0 at +28 and +36, the word at global 0x48198c at +32 and +40,
// 1.0 at +44, halves 200/150/50 at +10..+14 and again at +18..+22, and zero at
// +16 and +24, then lightAdd is called with that light. Afterwards bits
// 0x10001 of object+12 are set, the 12 bytes at object+16 are copied to
// object+136, bytes 148 and 149 are cleared, byte 85 is 1, byte 40 is 0xf0, halves 51/39/18 go to
// 94/96/98, and 100 is cleared. V0 is objectAddToRunList's.
extern "C" unsigned norrath_construct_Lantern(void* object, const void* point) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    const Address light = a + 76u;
    const unsigned tint = read_word(address(norrath_ctors_ant_lantern_view_48198c));
    bytes(light)[9] = 0u;
    write_word(a + 124u, local.x);
    write_word(a + 128u, local.y);
    write_word(a + 132u, local.z);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44a8a0));
    write_word(light + 36u, 0x43700000u);
    write_word(light + 40u, tint);
    write_word(light + 44u, 0x3f800000u);
    write_word(light + 28u, 0x43700000u);
    write_word(light + 32u, tint);
    write_half(light + 18u, 200u);
    write_half(light + 20u, 150u);
    write_half(light + 22u, 50u);
    write_half(light + 10u, 200u);
    write_half(light + 12u, 150u);
    write_half(light + 14u, 50u);
    write_half(light + 16u, 0u);
    write_half(light + 24u, 0u);
    bytes(light)[8] = 0u;
    norrath_light_add(pointer(light));
    write_word(a + 12u, read_word(a + 12u) | 0x10001u);
    bytes(a)[85] = 1u;
    bytes(a)[40] = 0xf0u;
    write_half(a + 94u, 51u);
    write_half(a + 96u, 39u);
    write_half(a + 98u, 18u);
    write_word(a + 136u, read_word(a + 16u));
    write_word(a + 140u, read_word(a + 20u));
    write_word(a + 144u, read_word(a + 24u));
    bytes(a)[148] = 0u;
    bytes(a)[149] = 0u;
    write_half(a + 100u, 0u);
    return norrath_actor_add_run_list(object);
}
