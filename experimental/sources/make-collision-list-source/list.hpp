#pragma once
static_assert(__CHAR_BIT__==8 && sizeof(unsigned)==4 && sizeof(int)==4 && sizeof(float)==4,"target word model");
static_assert(sizeof(void*)==4 && sizeof(__UINTPTR_TYPE__)==4,"target32 address bridge");
extern "C" {
extern unsigned char norrath_collision_frame_bytes[];
extern unsigned char norrath_collision_grid_scale_bytes[];
float floorf(float);
unsigned ee_cvt_w_s_word(float);
// Explicit pointer-lane adapter. Returns observed terminator address; original declaration unknown.
unsigned char* norrath_make_collision_list(unsigned char* world,
 float low_x,float low_y,float high_x,float high_y,unsigned char* output,int capacity);
}
