// Observed: Magnitude__C7VIVect3 at 0x0104f2b8, 52 bytes, in the pinned SLUS-20565 image. It takes the address of three consecutive 32-bit floats
// and returns, as a single float, the square root of x*x + y*y + z*z. The sum is formed as (x*x + y*y) + z*z, using the hardware single-precision square root.
// Inferred: the Euclidean length of the vector. Unknown: behaviour for NaN or infinite lanes beyond what the hardware instruction does.
extern "C" {
float Magnitude__C7VIVect3(const float* self) {
    const float x = self[0], y = self[1], z = self[2];
    return __builtin_sqrtf(x * x + y * y + z * z);
}
}
