#include "player_grid.hpp"
extern "C" NorrathPoint3* norrath_object_find_empty_spot(NorrathPoint3* destination, NorrathPoint3 origin, int distance, int clearance, char* result_flag) {
    using namespace norrath_player_grid_detail;
    // Native non-save span0..719. Relevant point/list/box slots retain their
    // real relative overlaps, including unused box-component stores.
    alignas(16) unsigned char workspace[720];
    const Address frame = address(workspace), initial = frame, candidate = frame + 16u;
    const Address offset = frame + 32u, nearby = frame + 128u;
    const Address minimum = frame + 640u, maximum = frame + 672u;
    copy_point(initial, address(&origin)); copy_point(candidate, initial);
    const float distance_float = static_cast<float>(distance);
    const float clearance_float = static_cast<float>(clearance);
    const float vertical = read_float(address(norrath_grid_vertical_extent_view));
    write_word(frame + 708u, address(result_flag));
    unsigned attempts = 0u;
    for (;;) {
        unsigned seed = read_word(address(norrath_constructor_random_state_view));
        float dx, dy, dz;
        for (;;) {
            const unsigned sx = random_step(seed), sy = random_step(sx), sz = random_step(sy);
            dx = random_component(sx); dy = random_component(sy); dz = random_component(sz);
            const float x_square = dx * dx, y_square = dy * dy, z_square = dz * dz;
            const float xy_square = x_square + y_square;
            const float length_square = xy_square + z_square;
            seed = sz;
            if (!(1.0f < length_square)) break;
        }
        put_float(frame + 64u, dx); put_float(frame + 68u, dy); put_float(frame + 72u, dz);
        copy_point(frame + 48u, frame + 64u);
        write_word(offset + 8u, 0u);
        const Address world_before_test = read_word(address(norrath_factory_world_pointer_word));
        const float offset_x = read_float(frame + 48u) * distance_float;
        write_word(address(norrath_constructor_random_state_view), seed);
        const float offset_y = read_float(frame + 52u) * distance_float;
        put_float(offset, offset_x); put_float(offset + 4u, offset_y);
        put_float(frame + 80u, offset_x + read_float(initial));
        put_float(frame + 84u, offset_y + read_float(initial + 4u));
        write_word(frame + 88u, read_word(initial + 8u));
        const float forbidden = norrath_world_test_forbidden(bytes(world_before_test), bytes(frame + 80u), clearance_float);
        bool accepted = false;
        if (forbidden == 0.0f) {
            put_float(frame + 96u, read_float(offset) + read_float(initial));
            put_float(frame + 100u, read_float(offset + 4u) + read_float(initial + 4u));
            put_float(frame + 104u, read_float(offset + 8u) + read_float(initial + 8u));
            const Address world_before_height = read_word(address(norrath_factory_world_pointer_word));
            const float height = norrath_world_find_height(bytes(world_before_height), bytes(frame + 96u), clearance_float);
            const float original_z = read_float(initial + 8u);
            const float rise = height - original_z;
            if (rise < 6.0f) {
                put_float(frame + 112u, read_float(initial) + read_float(offset));
                put_float(frame + 116u, read_float(initial + 4u) + read_float(offset + 4u));
                put_float(frame + 120u, original_z + read_float(offset + 8u));
                copy_point(candidate, frame + 112u);
                put_float(candidate + 8u, height);
                put_float(minimum + 16u, clearance_float); put_float(minimum + 20u, clearance_float);
                put_float(minimum + 24u, vertical); put_float(maximum + 16u, clearance_float);
                put_float(maximum + 20u, clearance_float); put_float(maximum + 24u, vertical);
                put_float(minimum, read_float(candidate) - clearance_float);
                put_float(minimum + 4u, read_float(candidate + 4u) - clearance_float);
                put_float(minimum + 8u, height - vertical);
                put_float(maximum, read_float(candidate) + clearance_float);
                put_float(maximum + 4u, read_float(candidate + 4u) + clearance_float);
                put_float(maximum + 8u, height + vertical);
                (void)norrath_object_find_in_box(point_value(minimum), point_value(maximum),
                    reinterpret_cast<void**>(static_cast<__UINTPTR_TYPE__>(nearby)), 128);
                Address cursor = nearby;
                const unsigned clearance_square = unsigned(clearance) * unsigned(clearance);
                const float candidate_y = read_float(candidate + 4u);
                while (read_word(cursor) != 0u) {
                    const Address object = read_word(cursor); cursor += 4u;
                    if ((read_word(object + 12u) & 1u) == 0u) {
                        const float separation_y = read_float(object + 20u) - candidate_y;
                        const float clearance_squared_float = static_cast<float>(signed_word(clearance_square));
                        const float separation_x = read_float(object + 16u) - read_float(candidate);
                        const float radius = read_float(object + 28u);
                        const float radius_squared = radius * radius;
                        const float y_squared = separation_y * separation_y;
                        const float x_squared = separation_x * separation_x;
                        const float limit = clearance_squared_float + radius_squared;
                        const float separation_squared = x_squared + y_squared;
                        if (separation_squared <= limit) { cursor -= 4u; break; }
                    }
                }
                accepted = read_word(cursor) == 0u;
            }
        }
        if (accepted) break;
        attempts += 1u;
        if (!(signed_word(attempts) < 50)) break;
    }
    const Address flag = read_word(frame + 708u);
    if (flag != 0u) *bytes(flag) = static_cast<unsigned char>(attempts != 50u);
    copy_point(address(destination), candidate);
    return destination;
}
