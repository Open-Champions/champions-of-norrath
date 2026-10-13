// Observed: SetFog__8VIRasteriffRC10VIColor32F at 0x011141e8, 216 bytes. The
// arguments are the raster, an int, two floats (first in $f12, second in $f13)
// and the address of a 16-byte colour; the result is zero. With nearZ the first
// float and farZ the second:
//   raster + 17548 = the int; raster + 17552 = nearZ; raster + 17556 = farZ;
//   the 16 bytes at the colour address are copied to raster + 17560.
//   Scratchpad words (at 0x700001f8 and 0x700001fc) are set to 0.0 and 255.0 and
//   read back; then, with nearZ and farZ re-read from the raster:
//     word 0x700001f0 = (zero - 255.0) / (farZ - nearZ)
//     word 0x700001f4 = (255.0 * farZ - zero * nearZ) / (farZ - nearZ)
//   (zero and 255.0 are the values read back from the scratchpad). Then
//   SetFogRegisters__8VIRaster(raster) is called and 1 is stored at
//   raster + 17888.
// Inferred: the two scratchpad results are a linear scale and offset that map
// depth between the near and far values onto 0..255 (fog coefficient), consumed
// by the VU code. Unknown: what the int selects, the colour's component order,
// and the names of the raster members.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile float* norrath_host_scratchpad(unsigned address);
#define NORRATH_SCRATCH(address) norrath_host_scratchpad(address)
#else
#define NORRATH_SCRATCH(address) reinterpret_cast<volatile float*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void SetFogRegisters__8VIRaster(void* raster);

int SetFog__8VIRasteriffRC10VIColor32F(void* raster, int mode, float nearZ, float farZ, const void* color) {
    unsigned char* bytes = static_cast<unsigned char*>(raster);
    *reinterpret_cast<float*>(bytes + 17552) = nearZ;
    *reinterpret_cast<float*>(bytes + 17556) = farZ;
    *reinterpret_cast<int*>(bytes + 17548) = mode;
    volatile float* spZero = NORRATH_SCRATCH(0x700001f8);
    volatile float* spTop = NORRATH_SCRATCH(0x700001fc);
    *reinterpret_cast<volatile int*>(spZero) = 0;
    *spTop = 255.0f;
    const unsigned* from = static_cast<const unsigned*>(color);
    unsigned* to = reinterpret_cast<unsigned*>(bytes + 17560);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
    float top = *spTop;
    float zero = *spZero;
    float nearRead = *reinterpret_cast<float*>(bytes + 17552);
    float farRead = *reinterpret_cast<float*>(bytes + 17556);
    float range = farRead - nearRead;
    *NORRATH_SCRATCH(0x700001f0) = (zero - top) / range;
    nearRead = *reinterpret_cast<float*>(bytes + 17552);
    farRead = *reinterpret_cast<float*>(bytes + 17556);
    float scaledZero = zero * nearRead;
    float scaledTop = top * farRead;
    float rangeAgain = farRead - nearRead;
    *NORRATH_SCRATCH(0x700001f4) = (scaledTop - scaledZero) / rangeAgain;
    SetFogRegisters__8VIRaster(raster);
    *reinterpret_cast<int*>(bytes + 17888) = 1;
    return 0;
}
}
