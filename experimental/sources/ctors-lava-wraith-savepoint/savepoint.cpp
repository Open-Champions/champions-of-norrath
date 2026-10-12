#include "../ctors-lava-wraith-detail/detail.hpp"
// __9SavepointG6Point3sPPc at 0x214a48, 428 bytes.
// GameObject receives a point snapshot, the resource map at 0x42c570 and the
// argument list. View 0x449550 goes to +72, byte +81 becomes 1 and +145 zero.
// The point snapshot is copied to +184. A light record at +136 is filled in
// place: the float quotient 1.0f / 0.0f (a runtime divide on the target) at
// +176, zeros at +164, +172 and +180, 1.0f at +168 (as a float word), halves
// 300 at +158 and +150, -16 at +160 and +152, 140 at +146, +148, +154 and
// +156, and byte +144 zero, then lightAdd receives +136. Byte +40 becomes 36,
// flags gain 0x00100008, +132 is cleared, bytes +197/+198 are cleared and
// lightDeactivate receives +136. addIcon receives the point snapshot, kind 0,
// the text at 0x4853b8 and zeros. The animation state at +76 is initialised,
// the 64-bit modelGetActiveChunks (word at model+12, 0) goes to +120, the type
// goes to +56 and the run list is called. Byte +196 becomes 1 last. Low V0 is
// the run-list result.
extern "C" unsigned norrath_construct_Savepoint(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, norrath_ctors_lava_wraith_view_42c570, arguments);
    const Address a = address(object);
    const Address light = a + 136u;
    volatile float zero = 0.0f;
    const float quotient = 1.0f / zero;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_449550));
    write_byte(a + 81u, 1u);
    write_byte(light + 9u, 0u);
    write_point(a + 184u, local);
    write_word(light + 44u, float_bits(0.0f));
    write_word(light + 40u, float_bits(quotient));
    write_word(light + 28u, float_bits(0.0f));
    write_word(light + 36u, float_bits(0.0f));
    write_word(light + 32u, float_bits(1.0f));
    write_half(light + 22u, 300u);
    write_half(light + 24u, 0xfff0u);
    write_half(light + 14u, 300u);
    write_half(light + 16u, 0xfff0u);
    write_half(light + 20u, 140u);
    write_half(light + 10u, 140u);
    write_half(light + 12u, 140u);
    write_half(light + 18u, 140u);
    write_byte(light + 8u, 0u);
    norrath_light_add(pointer(light));
    write_byte(a + 40u, 36u);
    const unsigned flags = read_word(a + 12u) | 0x00100008u;
    write_word(a + 132u, 0u);
    write_word(a + 12u, flags);
    write_byte(a + 197u, 0u);
    write_byte(a + 198u, 0u);
    norrath_light_deactivate(pointer(light));
    (void)norrath_map_add_icon(&local, 0, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4853b8), 0, nullptr, nullptr, nullptr, nullptr);
    norrath_actor_animation_initialize(bytes(a + 76u));
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(address(load_pointer(a + 36u)) + 12u), 0);
    write_half(a + 56u, static_cast<unsigned>(type));
    write_word(a + 120u, static_cast<unsigned>(chunks));
    write_word(a + 124u, static_cast<unsigned>(chunks >> 32));
    const unsigned result = norrath_actor_add_run_list(object);
    write_byte(a + 196u, 1u);
    return result;
}
