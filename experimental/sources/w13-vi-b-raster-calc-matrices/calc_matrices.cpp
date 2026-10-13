// Observed: CalcMatrices__8VIRaster at 0x01114388, 96 bytes. The raster argument is not
// used; everything happens in scratchpad addresses 0x70000000..0x700001ff. In order it calls:
//   Mul__10VIMatrix44RC10VIMatrix44T1(0x70000080, 0x70000000, 0x70000040)
//   Mul__10VIMatrix44RC10VIMatrix44T1(0x70000140, 0x70000080, 0x700000c0)
//   InvertAffine__10VIMatrix44RC10VIMatrix44(0x70000100, 0x70000000)
//   SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(0x70000100, 0x700001a0, 0x70000180)
//   Normalize__7VIVect3(0x70000180)
// and, only if the scratchpad byte at 0x70000222 is nonzero,
//   Mul__C10VIMatrix44RC7VIVect3R7VIVect3(0x70000100, 0x700001c0, 0x70000190).
// It returns 0.
// Inferred: builds derived 4x4 matrices (a product, a second product, an affine inverse) and
// the transformed and normalized sun direction in the scratchpad block that the VU-side
// code reads; byte 0x70000222 enables the extra vector transform. The sun-direction input
// 0x700001a0 is the block SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight fills.
// Unknown: which matrices the blocks hold (projection, view, clip...), the member names, and
// what the vectors at 0x700001c0 and 0x70000190 are.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif
#define NORRATH_SCRATCH(address) const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(address))

extern "C" {
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void InvertAffine__10VIMatrix44RC10VIMatrix44(void* result, const void* source);
void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* source, void* destination);
void Normalize__7VIVect3(void* vector);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* source, void* destination);

int CalcMatrices__8VIRaster(void* raster) {
    (void)raster;
    Mul__10VIMatrix44RC10VIMatrix44T1(NORRATH_SCRATCH(0x70000080), NORRATH_SCRATCH(0x70000000), NORRATH_SCRATCH(0x70000040));
    Mul__10VIMatrix44RC10VIMatrix44T1(NORRATH_SCRATCH(0x70000140), NORRATH_SCRATCH(0x70000080), NORRATH_SCRATCH(0x700000c0));
    InvertAffine__10VIMatrix44RC10VIMatrix44(NORRATH_SCRATCH(0x70000100), NORRATH_SCRATCH(0x70000000));
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(NORRATH_SCRATCH(0x70000100), NORRATH_SCRATCH(0x700001a0), NORRATH_SCRATCH(0x70000180));
    Normalize__7VIVect3(NORRATH_SCRATCH(0x70000180));
    if (*NORRATH_SCRATCH_BYTE(0x70000222) != 0)
        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(NORRATH_SCRATCH(0x70000100), NORRATH_SCRATCH(0x700001c0), NORRATH_SCRATCH(0x70000190));
    return 0;
}
}
