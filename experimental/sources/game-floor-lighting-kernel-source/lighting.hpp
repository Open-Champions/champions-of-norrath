#pragma once
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4 && sizeof(float) == 4,
              "requires the target word and floating-point model");
extern "C" {
extern unsigned char norrath_floor_light_scene_view[];
extern unsigned char norrath_floor_light_grid_view[];
extern unsigned char norrath_floor_light_interface_mode_word[];
extern unsigned char norrath_factory_world_pointer_word[];
unsigned ee_cvt_w_s_word(float value);
void norrath_find_lights_at_point(const unsigned char* point, unsigned char* output, int capacity);
float norrath_world_query_light_map(unsigned char* world, const unsigned char* point);
unsigned norrath_light_floor_sprite(const unsigned char* point);
}
namespace norrath_floor_lighting_detail {
using Word = unsigned;
inline Word address(const void* value) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(value)); }
inline unsigned char* pointer(Word value) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value)); }
inline Word read_byte(Word value) { return *reinterpret_cast<volatile const unsigned char*>(static_cast<__UINTPTR_TYPE__>(value)); }
inline Word read_word(Word value) {
    const Word a = read_byte(value); const Word b = read_byte(value + 1u);
    const Word c = read_byte(value + 2u); const Word d = read_byte(value + 3u);
    return a | (b << 8u) | (c << 16u) | (d << 24u);
}
inline void write_word(Word where, Word value) {
    volatile unsigned char* out = pointer(where);
    out[0] = static_cast<unsigned char>(value); out[1] = static_cast<unsigned char>(value >> 8u);
    out[2] = static_cast<unsigned char>(value >> 16u); out[3] = static_cast<unsigned char>(value >> 24u);
}
inline int signed_word(Word value) { return value < 0x80000000u ? static_cast<int>(value) : -1 - static_cast<int>(~value); }
inline int signed_byte(Word value) { return value < 128u ? static_cast<int>(value) : static_cast<int>(value) - 256; }
inline int signed_half(Word where) {
    const Word low = read_byte(where); const Word high = read_byte(where + 1u);
    const Word value = low | (high << 8u);
    return value < 32768u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
inline float from_word(Word value) { return __builtin_bit_cast(float, value); }
inline float read_float(Word value) { return from_word(read_word(value)); }
inline int cell(Word value, Word offset) { return signed_word(value + offset) >> 8; }
// Single architectural operations preserve EE floating-point behavior and avoid
// replacing its reciprocal square root or min/max with a host library model.
inline float fp_add(float a, float b) { float out; __asm__ volatile ("add.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
inline float fp_sub(float a, float b) { float out; __asm__ volatile ("sub.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
inline float fp_mul(float a, float b) { float out; __asm__ volatile ("mul.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
inline float fp_max(float a, float b) { float out; __asm__ volatile ("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
inline float fp_min(float a, float b) { float out; __asm__ volatile ("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
inline float reciprocal_length(float squared) { float out; const float one = 1.0f; __asm__ volatile ("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(one), "f"(squared)); return out; }
inline void capture_point(const unsigned char* input, unsigned char* output) {
    const Word base = address(input);
    const Word x = read_word(base); const Word y = read_word(base + 4u); const Word z = read_word(base + 8u);
    write_word(address(output), x); write_word(address(output) + 4u, y); write_word(address(output) + 8u, z);
}
}
