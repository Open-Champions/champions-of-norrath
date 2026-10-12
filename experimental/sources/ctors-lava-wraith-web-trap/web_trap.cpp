#include "../ctors-lava-wraith-detail/detail.hpp"
// __7WebTrapG6Point3sPPc at 0x20e490, 708 bytes.
// GameObject receives a point snapshot, a null map and the argument list. View
// 0x449c20 goes to +72 and +76 and +136 are cleared. A second snapshot has 24.0f
// added to its third float. The type, sign-extended, is an integer angle:
// sine goes to a direction's x, cosine to its y and 0.0f to its z, each scaled
// by 500.0f. worldHitscan (world word at 0x4844a0, the raised snapshot, that
// direction, 1200.0f, 0) returns a fraction t. For 0 < t <= 1 the hit offset is
// snapshot + direction * t; it is added to the three floats at +84, flags gain
// 1, +152 becomes 1, +80 becomes -1, +148 is cleared, bytes +40/+41 become 0xff,
// byte +132 becomes 50 and +156 becomes 0.0f. The middle of the object position
// and the new +84 vector, (position + vector) * 0.5f, is stored at +108 and
// again at +120. lumpFindResource of the texts at 0x485330 and 0x470358 goes to
// +140, +144 is cleared and the run list is called; low V0 is its result. Any
// other t queues the object for deletion and V0 is that result.
extern "C" unsigned norrath_construct_WebTrap(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    PointBytes from = snapshot_point(&local);
    const float raised = bits_float(from.z) + 24.0f;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_449c20));
    write_word(a + 76u, 0u);
    write_word(a + 136u, 0u);
    from.z = float_bits(raised);
    const float sine = norrath_integer_sine(static_cast<int>(type));
    const float cosine = norrath_integer_cosine(static_cast<int>(type));
    PointBytes direction{float_bits(sine), float_bits(cosine), float_bits(0.0f)};
    const float reach = 500.0f;
    direction.x = float_bits(bits_float(direction.x) * reach);
    direction.y = float_bits(bits_float(direction.y) * reach);
    direction.z = float_bits(bits_float(direction.z) * reach);
    const float fraction = norrath_world_hitscan(world_pointer(), &from, &direction, 1200.0f, 0);
    if (!(0.0f < fraction && fraction <= 1.0f)) {
        return norrath_object_add_to_delete_list(object);
    }
    const float step_x = bits_float(direction.x) * fraction;
    const float step_y = bits_float(direction.y) * fraction;
    const float step_z = bits_float(direction.z) * fraction;
    const float hit_x = bits_float(from.x) + step_x;
    const float hit_y = bits_float(from.y) + step_y;
    const float hit_z = bits_float(from.z) + step_z;
    const float offset_x = bits_float(read_word(a + 84u));
    const float offset_y = bits_float(read_word(a + 88u));
    const float offset_z = bits_float(read_word(a + 92u));
    write_word(a + 88u, float_bits(offset_y + hit_y));
    write_word(a + 12u, read_word(a + 12u) | 0x1u);
    write_word(a + 152u, 1u);
    write_word(a + 92u, float_bits(offset_z + hit_z));
    write_word(a + 84u, float_bits(offset_x + hit_x));
    write_word(a + 80u, 0xffffffffu);
    write_word(a + 148u, 0u);
    write_byte(a + 41u, 0xffu);
    write_byte(a + 40u, 0xffu);
    write_byte(a + 132u, 50u);
    write_word(a + 156u, float_bits(0.0f));
    const float sum_x = bits_float(read_word(a + 16u)) + bits_float(read_word(a + 84u));
    const float sum_y = bits_float(read_word(a + 20u)) + bits_float(read_word(a + 88u));
    const float sum_z = bits_float(read_word(a + 24u)) + bits_float(read_word(a + 92u));
    const float half = 0.5f;
    const PointBytes middle{float_bits(sum_x * half), float_bits(sum_y * half), float_bits(sum_z * half)};
    write_point(a + 108u, middle);
    write_point(a + 120u, snapshot_point(bytes(a + 108u)));
    void* const resource = norrath_lump_find_resource(reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_485330),
                                                      reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_470358));
    write_word(a + 144u, 0u);
    store_pointer(a + 140u, resource);
    return norrath_actor_add_run_list(object);
}
