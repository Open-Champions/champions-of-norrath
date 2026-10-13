// Observed: Restore2D__8VIRaster at 0x01114500, 96 bytes. It copies the 64-byte matrix at
// raster + 17584 to scratchpad 0x70000000 and the one at raster + 17648 to 0x70000040 with
// sceVu0CopyMatrix, copies the two bytes at raster + 17712 and 17713 to scratchpad bytes
// 0x70000220 and 0x70000221, stores 1 at raster + 17888 and returns 0.
// Inferred: this undoes Store2D__8VIRaster, which moves the same four items the other way
// and clears the scratchpad bytes; the two matrices are the scratchpad-resident transforms
// saved while 2D drawing is active, and the two bytes are the light-enable switches
// (0x70000221 is the byte EnableSunLight writes).
// Unknown: the member names and what the matrices hold.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void sceVu0CopyMatrix(void* destination, const void* source);

int Restore2D__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    sceVu0CopyMatrix(const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x70000000)), r + 17584);
    sceVu0CopyMatrix(const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(0x70000040)), r + 17648);
    *NORRATH_SCRATCH_BYTE(0x70000220) = r[17712];
    *NORRATH_SCRATCH_BYTE(0x70000221) = r[17713];
    *reinterpret_cast<int*>(r + 17888) = 1;
    return 0;
}
}
