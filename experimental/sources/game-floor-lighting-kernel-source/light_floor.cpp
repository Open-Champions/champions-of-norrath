#include "lighting.hpp"
extern "C" unsigned norrath_light_floor_sprite(const unsigned char* point) {
    using namespace norrath_floor_lighting_detail;
    alignas(8) unsigned char captured[12];
    alignas(8) unsigned char selected[128];
    capture_point(point, captured);
    float red = 0.0f, green = 0.0f, blue = 0.0f;
    norrath_find_lights_at_point(captured, selected, 32);
    const Word list = address(selected);
    if (read_word(list) != 0u) {
        Word offset = 0u;
        for (;;) {
            const Word light = read_word(list + offset);
            offset += 4u;
            if (signed_byte(read_byte(light + 9u)) != 0) {
                const float dx = fp_sub(read_float(light + 48u), read_float(address(captured)));
                const float dy = fp_sub(read_float(light + 52u), read_float(address(captured) + 4u));
                const float dz = fp_sub(read_float(light + 56u), read_float(address(captured) + 8u));
                const float yy = fp_mul(dy, dy); const float xx = fp_mul(dx, dx);
                const float zz = fp_mul(dz, dz);
                const float radius = read_float(light + 28u);
                const float intensity = read_float(light + 32u);
                const float xy = fp_add(xx, yy);
                const int current_flag = signed_byte(read_byte(light + 9u));
                const float radius_squared = fp_mul(radius, radius);
                const float distance_squared = fp_add(xy, zz);
                const float remainder = fp_sub(radius_squared, distance_squared);
                const float inverse = reciprocal_length(distance_squared);
                float strength = fp_mul(remainder, intensity);
                if (current_flag == 0) strength = fp_mul(strength, 0.25f);
                const float direction = fp_mul(dz, inverse);
                const int light_blue = signed_half(light + 14u);
                const int light_red = signed_half(light + 10u);
                const int light_green = signed_half(light + 12u);
                const float blue_value = static_cast<float>(light_blue);
                const float red_value = static_cast<float>(light_red);
                const float green_value = static_cast<float>(light_green);
                strength = fp_mul(strength, direction);
                strength = fp_max(strength, 0.0f);
                strength = fp_min(strength, 1.0f);
                const float blue_part = fp_mul(blue_value, strength);
                const float red_part = fp_mul(red_value, strength);
                const float green_part = fp_mul(green_value, strength);
                blue = fp_add(blue, blue_part);
                red = fp_add(red, red_part);
                green = fp_add(green, green_part);
            }
            if (read_word(list + offset) == 0u) break;
        }
    }
    const Word scene = address(norrath_floor_light_scene_view);
    const Word world = read_word(address(norrath_factory_world_pointer_word));
    float ambient = read_float(scene + 8u);
    if (world != 0u) ambient = fp_mul(ambient, norrath_world_query_light_map(pointer(world), captured));
    const Word ambient_green = read_byte(scene + 45u);
    const Word ambient_red = read_byte(scene + 44u);
    const Word ambient_blue = read_byte(scene + 46u);
    const Word extra_blue = read_byte(scene + 50u);
    const Word extra_green = read_byte(scene + 49u);
    ambient = fp_max(ambient, 0.0f);
    const float scaled_green = fp_mul(static_cast<float>(ambient_green), ambient);
    const Word extra_red = read_byte(scene + 48u);
    const float scaled_red = fp_mul(static_cast<float>(ambient_red), ambient);
    const float scaled_blue = fp_mul(static_cast<float>(ambient_blue), ambient);
    green = fp_add(green, scaled_green);
    red = fp_add(red, scaled_red);
    blue = fp_add(blue, scaled_blue);
    green = fp_add(green, static_cast<float>(extra_green));
    const float clamped_green = fp_min(fp_max(green, 0.0f), 255.0f);
    red = fp_add(red, static_cast<float>(extra_red));
    const Word green_word = ee_cvt_w_s_word(clamped_green);
    blue = fp_add(blue, static_cast<float>(extra_blue));
    const float clamped_red = fp_min(fp_max(red, 0.0f), 255.0f);
    const float clamped_blue = fp_min(fp_max(blue, 0.0f), 255.0f);
    const Word red_word = ee_cvt_w_s_word(clamped_red);
    const Word blue_word = ee_cvt_w_s_word(clamped_blue);
    return red_word | (green_word << 8u) | (blue_word << 16u);
}
