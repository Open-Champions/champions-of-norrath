// Observed: SetAmbient__8VIRasterRC10VIColor32F at 0x01113e88, 56 bytes. It copies sixteen
// bytes from the color argument to the scratchpad address 0x700001e0 as two unaligned
// eight-byte moves, stores 1 at raster + 17888 (the same word SetFog, EnableSunLight and
// EnableAllDirLights set) and returns 0.
// Inferred: the sixteen bytes are four floats (the type name says 32-bit float color); the
// scratchpad block is the ambient color read by VU or DMA-side code, and the raster word is
// a "lighting state changed" flag.
// Unknown: the member names, the meaning of the four components, and whether the
// destination is read as floats.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

typedef unsigned long long VIUnaligned64 __attribute__((aligned(1)));

extern "C" {
int SetAmbient__8VIRasterRC10VIColor32F(void* raster, const void* color) {
    const VIUnaligned64* from = static_cast<const VIUnaligned64*>(color);
    VIUnaligned64* to = reinterpret_cast<VIUnaligned64*>(const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x700001e0)));
    VIUnaligned64 first = from[0];
    VIUnaligned64 second = from[1];
    to[0] = first;
    to[1] = second;
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
