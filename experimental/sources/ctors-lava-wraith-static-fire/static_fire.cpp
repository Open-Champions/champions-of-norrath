#include "../ctors-lava-wraith-detail/detail.hpp"
// __10StaticFireG6Point3sPPc at 0x28dd00, 620 bytes.
// The short is not read. GameObject receives a point snapshot, a null map and
// the argument list. A light position is the object position with 0.0f added to
// x and 36.0f added to z; it is stored at +176. View 0x449310 goes to +72 and a
// light record at +128 is filled in place: 120.0f at +164 and +156, the literal
// float 0x3891a2b3 at +168 and +160, zero at +172, halves 255 at +146 and +138,
// 128 at +148 and +140, -16 at +152 and +144, zero at +142 and +150, and zero
// bytes at +137 and +136, before lightAdd receives +128. Then, in order: a
// present string tag at 0x486118 stores its int tag as a byte at +111, else
// zero; the same for 0x4778c8 at +112; init runs; the tag at 0x4778d8 stores a
// byte at +108 when present; 0x486120 stores its int tag converted to float at
// +28 and 0x486128 at +84 when present; 0x486130 stores a byte at +110, or 1
// when absent. Low V0 is the last stored byte's source (the int tag or 1).
extern "C" unsigned norrath_construct_StaticFire(void* object, const void* point, short, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const Address light = a + 128u;
    const float lift = 36.0f;
    const float x = bits_float(read_word(a + 16u)) + 0.0f;
    const float z = bits_float(read_word(a + 24u)) + lift;
    const PointBytes glow{float_bits(x), read_word(a + 20u), float_bits(z)};
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_449310));
    write_byte(light + 9u, 0u);
    write_point(a + 176u, glow);
    write_word(light + 36u, float_bits(120.0f));
    write_word(light + 40u, 0x3891a2b3u);
    write_word(light + 44u, float_bits(0.0f));
    write_half(light + 18u, 255u);
    write_half(light + 20u, 128u);
    write_half(light + 24u, 0xfff0u);
    write_word(light + 28u, float_bits(120.0f));
    write_word(light + 32u, 0x3891a2b3u);
    write_half(light + 10u, 255u);
    write_half(light + 12u, 128u);
    write_half(light + 14u, 0u);
    write_half(light + 16u, 0xfff0u);
    write_half(light + 22u, 0u);
    write_byte(light + 8u, 0u);
    norrath_light_add(pointer(light));
    char* const big = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_486118);
    if (norrath_actor_find_tag_string(object, big, 0) != nullptr) {
        write_byte(a + 111u, static_cast<unsigned>(norrath_game_object_find_tag_int(object, big, 0)));
    } else {
        write_byte(a + 111u, 0u);
    }
    char* const small = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4778c8);
    if (norrath_actor_find_tag_string(object, small, 0) != nullptr) {
        write_byte(a + 112u, static_cast<unsigned>(norrath_game_object_find_tag_int(object, small, 0)));
    } else {
        write_byte(a + 112u, 0u);
    }
    norrath_static_fire_init(object);
    char* const wind = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4778d8);
    if (norrath_actor_find_tag_string(object, wind, 0) != nullptr) {
        write_byte(a + 108u, static_cast<unsigned>(norrath_game_object_find_tag_int(object, wind, 0)));
    }
    char* const radius = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_486120);
    if (norrath_actor_find_tag_string(object, radius, 0) != nullptr) {
        write_word(a + 28u, float_bits(static_cast<float>(norrath_game_object_find_tag_int(object, radius, 0))));
    }
    char* const density = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_486128);
    if (norrath_actor_find_tag_string(object, density, 0) != nullptr) {
        write_word(a + 84u, float_bits(static_cast<float>(norrath_game_object_find_tag_int(object, density, 0))));
    }
    char* const fire = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_486130);
    if (norrath_actor_find_tag_string(object, fire, 0) != nullptr) {
        const int value = norrath_game_object_find_tag_int(object, fire, 0);
        write_byte(a + 110u, static_cast<unsigned>(value));
        return static_cast<unsigned>(value);
    }
    write_byte(a + 110u, 1u);
    return 1u;
}
