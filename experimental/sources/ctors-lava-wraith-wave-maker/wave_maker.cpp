#include "../ctors-lava-wraith-detail/detail.hpp"
// __9WaveMakerG6Point3sPPc at 0x151080, 1496 bytes.
// The reciprocal length uses the EE rsqrt.s (fs / sqrt(ft)), which the original
// executes; on a host it is 1.0f / sqrt.
static inline float norrath_wave_maker_inverse_length(float squared) {
#if defined(__mips__) && !defined(NORRATH_CTORS_LAVA_WRAITH_HOST)
    float result;
    __asm__("rsqrt.s %0,%1,%2" : "=f"(result) : "f"(1.0f), "f"(squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(squared);
#endif
}
// GameObject receives a point snapshot, a null map and the argument list. View
// 0x44c7a0 goes to +72, the run list is called and flag bit 0 is set. The line
// named by the text at 0x483e28 is fetched with gameLineGet and gameLineGetPtr
// and its pointer goes to +1892; its first point (word at line+20) seeds a
// position whose height comes from worldFindHeight (radius 32.0f), and
// worldCheckForWater receives that position, 32.0f and +1888. For each of the
// line's records (count at line+4, array at line+20, 64 bytes each) the cross
// product of the record's direction (+36..+44) with the up vector (0, 0, 1) is
// normalised with rsqrt.s and stored in the 36-byte slot at +76 + 36 * index as
// that unit vector, the negated direction and the up vector. The 504-byte
// buffer at 0x601fd0 is cleared; the segment count is the line count minus one
// when the word at line+8 is zero. Each segment is then sampled every 17.0f
// along its length (float at record+48): getWaveletMat(index, distance /
// length) and matTransform of (0, -1, 0) give a step vector scaled by 24.0f; a
// probe starting at the segment position walks that step (at most 255 times,
// counted in the byte array at 0x601030) until the water level at +1888 is
// within 4.0f above the terrain, setting the sample's bit in the buffer when
// the terrain is more than 12.0f above the water. The sample count is scaled
// by the literal 0.1f (0x3dcccccc), rounded half away from zero and stored at
// +1884; +1878 gets the half 1192 and +1880 is cleared. Low V0 is the constant
// 1192, not a callee result.
extern "C" unsigned norrath_construct_WaveMaker(void* object, const void* point, short, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_game_object_construct(object, &local, nullptr, arguments);
    const Address a = address(object);
    const float up[3] = {0.0f, 0.0f, 1.0f};
    const float down[3] = {0.0f, -1.0f, 0.0f};
    unsigned sample_count = 0u;
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44c7a0));
    (void)norrath_actor_add_run_list(object);
    write_word(a + 12u, read_word(a + 12u) | 1u);
    const int line_handle = norrath_game_line_get(reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_483e28));
    void* const line_pointer = norrath_game_line_get_ptr(line_handle);
    store_pointer(a + 1892u, line_pointer);
    Address line = address(line_pointer);
    float seed[3];
    {
        const Address first = address(load_pointer(line + 20u));
        seed[0] = bits_float(read_word(first));
        seed[1] = bits_float(read_word(first + 4u));
        seed[2] = bits_float(read_word(first + 8u));
    }
    seed[2] = norrath_world_find_height(world_pointer(), seed, 32.0f);
    (void)norrath_world_check_for_water(world_pointer(), seed, 32.0f, reinterpret_cast<float*>(bytes(a + 1888u)));
    line = address(load_pointer(a + 1892u));
    const int count = static_cast<int>(read_word(line + 4u));
    if (count > 0) {
        int index = 0;
        Address slot = a + 76u;
        do {
            const Address record = address(load_pointer(line + 20u)) + (static_cast<unsigned>(index) << 6);
            const float dx = bits_float(read_word(record + 36u));
            const float dy = bits_float(read_word(record + 40u));
            const float dz = bits_float(read_word(record + 44u));
            ++index;
            float cross[3];
            cross[0] = dy * up[2] - dz * up[1];
            cross[1] = dz * up[0] - dx * up[2];
            cross[2] = dx * up[1] - dy * up[0];
            float squared = cross[0] * cross[0];
            squared = squared + cross[1] * cross[1];
            squared = squared + cross[2] * cross[2];
            const float inverse = norrath_wave_maker_inverse_length(squared);
            const float nx = cross[0] * inverse;
            const float ny = cross[1] * inverse;
            const float nz = cross[2] * inverse;
            write_word(slot, float_bits(nx));
            write_word(slot + 4u, float_bits(ny));
            write_word(slot + 8u, float_bits(nz));
            write_word(slot + 12u, float_bits(-dx));
            write_word(slot + 16u, float_bits(-dy));
            write_word(slot + 20u, float_bits(-dz));
            write_word(slot + 24u, float_bits(up[0]));
            write_word(slot + 28u, float_bits(up[1]));
            write_word(slot + 32u, float_bits(up[2]));
            slot += 36u;
        } while (index < static_cast<int>(read_word(line + 4u)));
    }
    (void)memset(bytes(address(norrath_ctors_lava_wraith_view_601fd0)), 0, 504u);
    const int segments = static_cast<int>(read_word(line + 4u)) - (read_word(line + 8u) == 0u ? 1 : 0);
    if (segments > 0) {
        int index = 0;
        do {
            const Address record = address(load_pointer(address(load_pointer(a + 1892u)) + 20u)) + (static_cast<unsigned>(index) << 6);
            float position[3] = {bits_float(read_word(record)), bits_float(read_word(record + 4u)), bits_float(read_word(record + 8u))};
            float step[3];
            step[0] = bits_float(read_word(record + 12u)) * 17.0f;
            step[1] = bits_float(read_word(record + 16u)) * 17.0f;
            step[2] = bits_float(read_word(record + 20u)) * 17.0f;
            float travelled = 0.0f;
            if (0.0f <= bits_float(read_word(record + 48u))) {
                do {
                    float matrix[9];
                    float offset[3];
                    norrath_wave_maker_get_wavelet_mat(object, index, travelled / bits_float(read_word(record + 48u)), matrix);
                    norrath_mat_transform(offset, matrix, const_cast<float*>(down));
                    offset[0] = offset[0] * 24.0f;
                    offset[1] = offset[1] * 24.0f;
                    offset[2] = offset[2] * 24.0f;
                    unsigned char* const flag = bytes(address(norrath_ctors_lava_wraith_view_601030) + sample_count);
                    *flag = 0u;
                    const Address mask_word = address(norrath_ctors_lava_wraith_view_601fd0) + ((static_cast<int>(sample_count) >> 5) << 2);
                    const unsigned bit = 1u << (sample_count & 0x1fu);
                    float probe[3] = {position[0], position[1], position[2]};
                    for (;;) {
                        const float height = norrath_world_find_height(world_pointer(), probe, 8.0f);
                        const float water = bits_float(read_word(a + 1888u));
                        if (water - height < 4.0f) {
                            if (12.0f < height - water) write_word(mask_word, read_word(mask_word) | bit);
                            break;
                        }
                        probe[0] = probe[0] + offset[0];
                        const unsigned taken = static_cast<unsigned>(*flag) + 1u;
                        *flag = static_cast<unsigned char>(taken);
                        probe[1] = probe[1] + offset[1];
                        probe[2] = probe[2] + offset[2];
                        if ((taken & 0xffu) >= 255u) break;
                    }
                    if (*flag == 255u) *flag = 0u;
                    position[0] = position[0] + step[0];
                    travelled = travelled + 17.0f;
                    ++sample_count;
                    position[1] = position[1] + step[1];
                    position[2] = position[2] + step[2];
                } while (travelled <= bits_float(read_word(record + 48u)));
            }
            ++index;
        } while (index < segments);
    }
    const float scaled = static_cast<float>(static_cast<int>(sample_count)) * bits_float(0x3dccccccu);
    int rounded;
    if (0.0f <= scaled) rounded = static_cast<int>(scaled + 0.5f);
    else rounded = static_cast<int>(scaled - 0.5f);
    write_word(a + 1884u, static_cast<unsigned>(rounded));
    write_half(a + 1878u, 1192u);
    write_word(a + 1880u, 0u);
    return 1192u;
}
