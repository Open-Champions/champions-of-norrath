#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4 &&
              sizeof(float) == 4, "target scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 byte-view address bridge");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
extern "C" {
extern unsigned char norrath_collision_frame_bytes[];
// Distinct mutable word at original 0x480024, not another routine's factor.
extern unsigned char norrath_find_height_orientation_scale_bytes[];
unsigned char* norrath_make_collision_list(unsigned char*, float, float, float,
                                          float, unsigned char*, int);
float floorf(float);
float ceilf(float);
unsigned ee_cvt_w_s_word(float);
float ee_mul_ordered(float, float);
float ee_collision_int_to_float(int);
// Observed A0/A1 pointer lanes and F12 radius, not an original class declaration.
float norrath_world_find_height(unsigned char* world,
                               const unsigned char* point, float radius);
}
