#include "../ctors-lava-wraith-detail/detail.hpp"
// __15PushPhysicsPropG6Point3sPcN23f at 0x150280, 360 bytes.
// The short is not read. GameObject receives a point snapshot, a null map and
// a null argument list. The scale is stored at +256 and view 0x448a70 at +72.
// The three strings are stored at +268, +272 and +276; lumpFindResource is
// called with (+268, +272) for +260 and then (+268, +276) for +264. Flags gain
// 0x10. +76 is cleared; +28 gets scale * 1.2f (literal-pool word 0x3f999999)
// and +32 gets scale + scale. bodyInit receives +80, 1.0f, 1.0f and three
// copies of scale + scale. The position's +24 float is raised by the scale
// while the position is copied to +96, then lowered again, the run list is
// called, the position is copied to +232, and +244/+248/+252 are cleared.
// The machine V0 at return is the third position word, not the run list result.
extern "C" unsigned norrath_construct_PushPhysicsProp(void* object, const void* point, short, char* lump, char* first,
                                                       char* second, float scale) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, nullptr);
    const Address a = address(object);
    write_word(a + 256u, float_bits(scale));
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_448a70));
    write_word(a + 276u, address_bits(second));
    write_word(a + 268u, address_bits(lump));
    write_word(a + 272u, address_bits(first));
    write_word(a + 260u, address_bits(norrath_lump_find_resource(lump, first)));
    void* const second_resource = norrath_lump_find_resource(static_cast<char*>(load_pointer(a + 268u)),
                                                              static_cast<char*>(load_pointer(a + 276u)));
    const float stored_scale = bits_float(read_word(a + 256u));
    const float doubled = stored_scale + stored_scale;
    const float scaled = stored_scale * bits_float(0x3f999999u);
    write_word(a + 264u, address_bits(second_resource));
    write_word(a + 12u, read_word(a + 12u) | 0x10u);
    write_word(a + 76u, 0u);
    write_word(a + 32u, float_bits(doubled));
    write_word(a + 28u, float_bits(scaled));
    norrath_body_init(bytes(a + 80u), 1.0f, 1.0f, doubled, doubled, doubled);
    write_word(a + 24u, float_bits(bits_float(read_word(a + 24u)) + bits_float(read_word(a + 256u))));
    write_point(a + 96u, snapshot_point(bytes(a + 16u)));
    write_word(a + 24u, float_bits(bits_float(read_word(a + 24u)) - bits_float(read_word(a + 256u))));
    (void)norrath_actor_add_run_list(object);
    const PointBytes position = snapshot_point(bytes(a + 16u));
    write_point(a + 232u, position);
    write_word(a + 252u, 0u);
    write_word(a + 244u, 0u);
    write_word(a + 248u, 0u);
    return position.z;
}
