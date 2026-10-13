// Observed: SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight at 0x01113ed8, 148
// bytes. It copies the three floats at the light pointer to scratchpad 0x700001a0 and
// sixteen bytes from light + 12 to scratchpad 0x700001ac (two unaligned eight-byte moves),
// then calls SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, light, 0x700001a0) and
// Normalize__7VIVect3(0x700001a0), stores 1 at raster + 17888 and returns 0.
// Inferred: the light's first three floats are a direction; it is rotated by the matrix
// (the sub-matrix product writes over the scratchpad copy) and normalized in place, and
// the following sixteen bytes (four floats, a color) sit next to it for VU or DMA-side code.
// Unknown: the VIRasterDirLight member names, the exact layout beyond these 28 bytes, and
// the roles of the two scratchpad blocks.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

typedef unsigned long long VIUnaligned64 __attribute__((aligned(1)));

extern "C" {
void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* source, void* destination);
void Normalize__7VIVect3(void* vector);

int SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight(void* raster, const void* matrix, const void* light) {
    const float* direction = static_cast<const float*>(light);
    float* directionOut = reinterpret_cast<float*>(const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x700001a0)));
    directionOut[0] = direction[0];
    directionOut[1] = direction[1];
    directionOut[2] = direction[2];
    const VIUnaligned64* from = reinterpret_cast<const VIUnaligned64*>(static_cast<const unsigned char*>(light) + 12);
    VIUnaligned64* to = reinterpret_cast<VIUnaligned64*>(const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x700001ac)));
    VIUnaligned64 first = from[0];
    VIUnaligned64 second = from[1];
    to[0] = first;
    to[1] = second;
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, light, directionOut);
    Normalize__7VIVect3(directionOut);
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
