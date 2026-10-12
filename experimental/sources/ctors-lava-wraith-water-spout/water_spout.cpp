#include "../ctors-lava-wraith-detail/detail.hpp"
// __10WaterSpoutG6Point3i at 0x1e5700, 232 bytes.
// GameObject receives a point snapshot, a null map and a null argument list.
// View 0x44a390 goes to +72, byte +40 becomes 0xf0 and the low half of the int
// goes to +56. The integer-angle cosine of the sign-extended int is widened with
// fptodp, multiplied as a soft double by the doubleword constant 0x4032666666666666
// (the double at 0x46d0c0, 9.2), narrowed with dptofp and stored at +76. The
// sine of the half reloaded from +56 gets the same treatment and goes to +80.
// Flags are set to 9 (assigned, not ORed), the run list is called, and bytes
// +88/+89 are cleared. Low V0 is the run-list result.
extern "C" unsigned norrath_construct_WaterSpout(void* object, const void* point, int type) {
    using namespace norrath_ctors_lava_wraith;
    const unsigned long long scale = 0x4032666666666666ull;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44a390));
    write_byte(a + 40u, 0xf0u);
    write_half(a + 56u, static_cast<unsigned>(type));
    const float cosine = norrath_integer_cosine(low_signed(type));
    const unsigned long long cosine_scaled = norrath_softdouble_multiply_2fb680(norrath_entity_fptodp(cosine), scale);
    write_word(a + 76u, float_bits(norrath_softdouble_to_float_2fbc20(cosine_scaled)));
    const float sine = norrath_integer_sine(read_signed_half(a + 56u));
    const unsigned long long sine_scaled = norrath_softdouble_multiply_2fb680(norrath_entity_fptodp(sine), scale);
    const float sine_result = norrath_softdouble_to_float_2fbc20(sine_scaled);
    write_word(a + 12u, 9u);
    write_word(a + 80u, float_bits(sine_result));
    const unsigned result = norrath_actor_add_run_list(object);
    write_byte(a + 88u, 0u);
    write_byte(a + 89u, 0u);
    return result;
}
