// Observed: Store2D__8VIRaster at 0x01114490, 112 bytes. It copies the 64-byte matrices at
// scratchpad 0x70000000 and 0x70000040 to raster + 17584 and raster + 17648 with
// sceVu0CopyMatrix, copies scratchpad bytes 0x70000220 and 0x70000221 to raster + 17712 and
// 17713, clears both scratchpad bytes, stores 1 at raster + 17888 and returns 0.
// Inferred: the saving half of a Store2D/Restore2D pair around 2D drawing; clearing the two
// bytes turns the lighting switches off meanwhile (0x70000221 is the byte EnableSunLight
// writes).
// Unknown: the member names and what the matrices hold.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void sceVu0CopyMatrix(void* destination, const void* source);

int Store2D__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    sceVu0CopyMatrix(r + 17584, const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x70000000)));
    sceVu0CopyMatrix(r + 17648, const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x70000040)));
    r[17712] = *NORRATH_SCRATCH_BYTE(0x70000220);
    r[17713] = *NORRATH_SCRATCH_BYTE(0x70000221);
    *NORRATH_SCRATCH_BYTE(0x70000220) = 0;
    *NORRATH_SCRATCH_BYTE(0x70000221) = 0;
    *reinterpret_cast<int*>(r + 17888) = 1;
    return 0;
}
}
