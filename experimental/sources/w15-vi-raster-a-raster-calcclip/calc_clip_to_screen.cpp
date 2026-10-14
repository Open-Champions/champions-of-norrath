// Observed: CalcClipToScreen__8VIRaster at 0x01111c68 is 252 bytes in the pinned SLUS-20565 image. It calls
// Identity__10VIMatrix44 on the matrix at the fixed scratchpad address 0x700000c0, then overwrites five of its elements (as float
// offsets from that address) using ints at +17716, +17720, +17724, +17728, +17748, +17752 of the raster object and floats at +17544,
// +17772 and +17776 (half = 0.5f, 2048.0f is the float 0x45000000):
//   +0xc0 = (float)int[17724] * half
//   +0xd4 = -(float)int[17728] * half
//   +0xe8 = (float[17772] - float[17776]) * half
//   +0xf0 = ((2048 - (float)int[17748] * half) + (float)int[17724] * half) + (float)int[17716]
//   +0xf4 = ((2048 - (float)int[17752] * half) + (float)int[17728] * half) + (float)int[17720]
//   +0xf8 = ((float[17776] - float[17772]) * half + float[17772]) + float[17544]
// and returns 0.
// Inferred: a clip-space to screen-space transform: x/y scale by half the viewport extent, an offset of 2048 (the GS guard-band centre)
// minus half the viewport origin plus a screen offset, and z mapped from a near/far pair plus a bias. The field meanings are guesses from
// that shape; the +17716..+17752 ints are plausibly viewport left, top, width and height.
// Unknown: those field meanings, whether the 2048 centre is the GS primitive offset, and whether the scratchpad matrix is read later.
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void Identity__10VIMatrix44(float* matrix);
int CalcClipToScreen__8VIRaster(unsigned char* self) {
    float* m = reinterpret_cast<float*>(NORRATH_WORD_TO_POINTER(0x700000c0u));
    Identity__10VIMatrix44(m);
    const float half = 0.5f;
    const float centre = 2048.0f;
    float a = static_cast<float>(*reinterpret_cast<int*>(self + 17724)) * half;
    float b = static_cast<float>(*reinterpret_cast<int*>(self + 17748)) * half;
    float c = static_cast<float>(*reinterpret_cast<int*>(self + 17752)) * half;
    float d = static_cast<float>(*reinterpret_cast<int*>(self + 17728));
    float e = d * half;
    float f = static_cast<float>(*reinterpret_cast<int*>(self + 17716));
    float g = static_cast<float>(*reinterpret_cast<int*>(self + 17720));
    m[0] = a;
    float x = (centre - b) + a + f;
    float y = (centre - c) + e + g;
    m[5] = -d * half;
    m[12] = x;
    m[13] = y;
    const float zNear = *reinterpret_cast<float*>(self + 17772);
    const float zFar = *reinterpret_cast<float*>(self + 17776);
    m[10] = (zNear - zFar) * half;
    m[14] = (zFar - zNear) * half + zNear + *reinterpret_cast<float*>(self + 17544);
    return 0;
}
}
