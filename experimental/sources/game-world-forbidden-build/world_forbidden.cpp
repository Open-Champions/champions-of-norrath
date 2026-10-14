#include "world_forbidden.hpp"
namespace {
using Word = unsigned;
Word address(const unsigned char* pointer) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned char* bytes(Word value) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
}
Word read_half(Word value) {
    const unsigned char* pointer = bytes(value);
    return Word(pointer[0]) | (Word(pointer[1]) << 8);
}
Word read_word(Word value) {
    const unsigned char* pointer = bytes(value);
    return Word(pointer[0]) | (Word(pointer[1]) << 8) |
           (Word(pointer[2]) << 16) | (Word(pointer[3]) << 24);
}
void write_word(Word destination, Word value) {
    for (unsigned index = 0; index != 4; ++index)
        bytes(destination)[index] = static_cast<unsigned char>(value >> (8 * index));
}
float float_from_bytes(const unsigned char* pointer) {
    float result;
    unsigned char* representation = reinterpret_cast<unsigned char*>(&result);
    for (unsigned index = 0; index != 4; ++index)
        representation[index] = pointer[index];
    return result;
}
float read_float(Word value) { return float_from_bytes(bytes(value)); }
int signed_half(Word value) {
    return value < 32768u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
int signed_word(Word value) {
    return value < 0x80000000u ? static_cast<int>(value)
                             : -1 - static_cast<int>(~value);
}
}
extern "C" float norrath_world_test_forbidden(unsigned char* world,
                                               const unsigned char* incoming_point,
                                               float radius) {
    unsigned char point[12];
    for (unsigned index = 0; index != 12; ++index) point[index] = incoming_point[index];
    // Actual native capacity64 supplies63 pointers plus the genuine terminator.
    // This stack workspace is not initialized by the caller source.
    alignas(4) unsigned char collision_list[64 * 4];
    const Word frame_address = address(norrath_collision_frame_bytes);
    const Word frame = read_word(frame_address);
    const float point_x = float_from_bytes(point);
    const float point_y = float_from_bytes(point + 4);
    const float high_x = point_x + radius;
    write_word(frame_address, frame + 1u);
    const float high_y = point_y + radius;
    const float low_x = point_x - radius;
    const float low_y = point_y - radius;
    const float quarter_radius = ee_mul_ordered(radius, 0.25f);
    (void)norrath_make_collision_list(world, low_x, low_y, high_x, high_y,
                                     collision_list, 64);
    const float radius_squared = ee_mul_ordered(quarter_radius, quarter_radius);
    const Word reach = ee_cvt_w_s_word(ceilf(quarter_radius));
    Word list_offset = 0u;
    Word part = read_word(address(collision_list));
    if (part != 0u) {
        const float orientation_scale =
            read_float(address(norrath_forbidden_orientation_scale_bytes));
        do {
            const Word map = read_word(part + 12u);
            const Word translation_z = read_word(part + 28u);
            const Word maximum = Word(signed_half(read_half(map + 18u))) + translation_z;
            if (-1000000 < signed_word(maximum)) {
                const float translation_x =
                    ee_collision_int_to_float(signed_word(read_word(part + 20u)));
                const float translation_y =
                    ee_collision_int_to_float(signed_word(read_word(part + 24u)));
                const Word orientation_a_word = read_half(part + 32u);
                const Word orientation_b_word = read_half(part + 34u);
                const float difference_x = point_x - translation_x;
                const Word flags = read_half(part + 16u);
                const float difference_y = point_y - translation_y;
                const float orientation_a =
                    ee_collision_int_to_float(signed_half(orientation_a_word));
                const float orientation_b =
                    ee_collision_int_to_float(signed_half(orientation_b_word));
                const float a = ee_mul_ordered(orientation_a, orientation_scale);
                const float b = ee_mul_ordered(orientation_b, orientation_scale);
                float local_x, local_y;
                if (flags & 1u) {
                    const float y_a = ee_mul_ordered(difference_y, a);
                    const float x_a = ee_mul_ordered(difference_x, a);
                    const float x_b = ee_mul_ordered(difference_x, b);
                    const float y_b = ee_mul_ordered(difference_y, b);
                    local_y = x_b - y_a;
                    local_x = x_a + y_b;
                } else {
                    const float negative_x = -difference_x;
                    const float y_a = ee_mul_ordered(difference_y, a);
                    const float x_a = ee_mul_ordered(difference_x, a);
                    const float y_b = ee_mul_ordered(difference_y, b);
                    const float negative_x_b = ee_mul_ordered(negative_x, b);
                    local_x = x_a + y_b;
                    local_y = negative_x_b + y_a;
                }
                const float grid_x = ee_mul_ordered(local_x, 0.25f);
                const float grid_y = ee_mul_ordered(local_y, 0.25f);
                const Word center_x = ee_cvt_w_s_word(floorf(grid_x));
                const float floor_y = floorf(grid_y);
                const Word origin_x = read_word(map);
                const Word center_y = ee_cvt_w_s_word(floor_y);
                const Word origin_y = read_word(map + 4u);
                const Word start_x = center_x - reach;
                const Word end_x = center_x + reach + 1u;
                const Word start_y = center_y - reach;
                const Word end_y = center_y + reach + 1u;
                // Original reads width even for an empty signed row range.
                const Word width = read_word(map + 8u);
                for (Word row = start_y; signed_word(row) <= signed_word(end_y); ++row) {
                    const Word relative_row = row - origin_y;
                    if (signed_word(relative_row) < 0) continue;
                    if (signed_word(relative_row) >= signed_word(read_word(map + 12u))) continue;
                    if (signed_word(end_x) < signed_word(start_x)) continue;
                    Word sample = map + 20u +
                        2u * (relative_row * width - origin_x + start_x);
                    for (Word column = start_x; signed_word(column) <= signed_word(end_x);
                         ++column, sample += 2u) {
                        const Word relative_column = column - origin_x;
                        if (signed_word(relative_column) < 0 ||
                            signed_word(relative_column) >= signed_word(width)) continue;
                        // Actual LH sign extension does not change the tested low bit.
                        if ((read_half(sample) & 1u) == 0u) continue;
                        const float sample_x = ee_collision_int_to_float(signed_word(column));
                        const float sample_y = ee_collision_int_to_float(signed_word(row));
                        const float distance_x = sample_x - grid_x;
                        const float distance_y = sample_y - grid_y;
                        const float x_squared = ee_mul_ordered(distance_x, distance_x);
                        const float y_squared = ee_mul_ordered(distance_y, distance_y);
                        const float distance_squared = x_squared + y_squared;
                        if (distance_squared < radius_squared) return 1.0f;
                    }
                }
            }
            list_offset += 4u;
            part = read_word(address(collision_list) + list_offset);
        } while (part != 0u);
    }
    return 0.0f;
}
