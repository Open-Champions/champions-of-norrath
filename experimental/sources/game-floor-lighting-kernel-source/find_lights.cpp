#include "lighting.hpp"
#include "ee_compare.hpp"
#include "grid_stride.hpp"
extern "C" void norrath_find_lights_at_point(const unsigned char* point, unsigned char* output, int capacity) {
    using namespace norrath_floor_lighting_detail;
    alignas(8) unsigned char captured[12];
    capture_point(point, captured);
    const Word y_word = ee_cvt_w_s_word(read_float(address(captured) + 4u));
    const Word x_word = ee_cvt_w_s_word(read_float(address(captured)));
    int y_end = cell(y_word, 11820u); if (y_end >= 90) y_end = 89;
    int y_start = cell(y_word, 11220u); if (y_start < 0) y_start = 0;
    int x_end = cell(x_word, 11820u); if (x_end >= 90) x_end = 89;
    int x_start = cell(x_word, 11220u); if (x_start < 0) x_start = 0;
    Word found = 0u;
    for (int y = y_start; y <= y_end; ++y) {
        for (int x = x_start; x <= x_end; ++x) {
            const Word row = static_cast<Word>(y);
            const Word row3 = norrath_floor_grid_stride::add(norrath_floor_grid_stride::shift_left<1>(row), row);
            const Word row45 = norrath_floor_grid_stride::subtract(norrath_floor_grid_stride::shift_left<4>(row3), row3);
            const Word row90 = norrath_floor_grid_stride::shift_left<1>(row45);
            const Word index = norrath_floor_grid_stride::add(row90, static_cast<Word>(x));
            Word light = read_word(address(norrath_floor_light_grid_view) + index * 4u);
            Word destination = address(output) + found * 4u;
            while (light != 0u) {
                const float px = read_float(address(captured));
                const float radius = read_float(light + 28u);
                const float lx = read_float(light + 48u);
                bool include = ee_c_lt_s_word(fp_add(lx, radius), px) == 0u;
                if (include) include = ee_c_lt_s_word(px, fp_sub(lx, radius)) == 0u;
                if (include) {
                    const float ly = read_float(light + 52u);
                    const float py = read_float(address(captured) + 4u);
                    include = ee_c_lt_s_word(fp_add(ly, radius), py) == 0u;
                    if (include) include = ee_c_lt_s_word(py, fp_sub(ly, radius)) == 0u;
                }
                if (include) {
                    const float lz = read_float(light + 56u);
                    const float pz = read_float(address(captured) + 8u);
                    include = ee_c_lt_s_word(fp_add(lz, radius), pz) == 0u;
                    if (include) include = ee_c_lt_s_word(pz, fp_sub(lz, radius)) == 0u;
                }
                if (include && signed_word(found) < signed_word(static_cast<Word>(capacity) - 1u)) {
                    const int mode = signed_byte(read_byte(light + 8u));
                    if (mode == signed_word(read_word(address(norrath_floor_light_interface_mode_word)))) {
                        write_word(destination, light);
                        destination += 4u;
                        ++found;
                    }
                }
                light = read_word(light);
            }
        }
    }
    write_word(address(output) + found * 4u, 0u);
}
