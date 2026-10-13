// Observed: __ml__FRC7VIVect3RC10VIMatrix44 at 0x010d0bc8 is 136 bytes in the pinned SLUS-20565 image. Arguments: a result buffer
// (the hidden return pointer for a four-float value), a three-float vector and the matrix. A local four-float value is built from
// the vector's x, y, z and the float 1.0 as its fourth lane (aligned local, 16 bytes). sceVu0ApplyMatrix(out, matrix, value) then
// runs with a second local four-float buffer as destination, and its four floats are copied to the result buffer, which is
// returned.
// Inferred: the by-value return is a four-float vector (all four lanes are copied) and the vector is treated as a point (w = 1).
// Unknown: the library routine's behaviour (an import); whether the original used an intermediate copy. Nothing here ran on the EE.
extern "C" {
void sceVu0ApplyMatrix(float* destination, const float* matrix, const float* vector);

float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* vector, const float* matrix) {
    alignas(16) float point[4];
    alignas(16) float transformed[4];
    point[0] = vector[0];
    point[1] = vector[1];
    point[2] = vector[2];
    point[3] = 1.0f;
    sceVu0ApplyMatrix(transformed, matrix, point);
    result[0] = transformed[0];
    result[1] = transformed[1];
    result[2] = transformed[2];
    result[3] = transformed[3];
    return result;
}
}
