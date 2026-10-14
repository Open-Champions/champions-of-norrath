// Observed: __ml__FRC7VIVect3RC10VIMatrix33 at 0x0108abd0, 144 bytes, in the pinned SLUS-20565 image. Arguments: a result buffer (hidden return
// pointer of a three-float value), a vector v and a 3x3 matrix m of nine consecutive floats. result[i] = m[i]*v[0] + m[3+i]*v[1] + m[6+i]*v[2] for
// i = 0..2, each sum added left to right; the result pointer is returned. All nine matrix floats and three vector floats are read before the stores.
// Inferred: the vector is a row vector multiplied by a row-major matrix (equivalently the transposed matrix applied to a column vector).
// Unknown: whether the result buffer may alias the vector (the original reads every input first, so aliasing would be safe here too).
extern "C" {
float* __ml__FRC7VIVect3RC10VIMatrix33(float* result, const float* vector, const float* matrix) {
    const float x = vector[0], y = vector[1], z = vector[2];
    const float r0 = matrix[0] * x + matrix[3] * y + matrix[6] * z;
    const float r1 = matrix[1] * x + matrix[4] * y + matrix[7] * z;
    const float r2 = matrix[2] * x + matrix[5] * y + matrix[8] * z;
    result[0] = r0;
    result[1] = r1;
    result[2] = r2;
    return result;
}
}
