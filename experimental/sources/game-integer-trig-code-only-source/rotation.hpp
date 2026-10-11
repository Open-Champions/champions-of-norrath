#pragma once
using NorrathMatrix34 = float[12];
static_assert(sizeof(float) == 4 && alignof(float) == 4);
static_assert(sizeof(int) == 4 && sizeof(unsigned) == 4);
static_assert(sizeof(NorrathMatrix34) == 48);
extern "C" {
extern float norrath_icos_scale, norrath_icos_cubic, norrath_icos_quintic, norrath_icos_septic;
extern float norrath_isin_scale, norrath_isin_cubic, norrath_isin_quintic, norrath_isin_septic;
float norrath_integer_cosine(int angle);
float norrath_integer_sine(int angle);
// Explicit int adapter normalizes low16 bits, including noncanonical high bits.
void norrath_rotate_matrix(NorrathMatrix34 matrix, int axis, int raw_angle);
}
