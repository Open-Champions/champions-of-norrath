// Observed: EnableSunLight__8VIRasteri at 0x01113f70, 24 bytes. It stores the low byte of its
// int argument into the scratchpad byte at 0x70000221 and stores 1 at raster + 17888
// (the same word SetFog and EnableAllDirLights set), then returns 0.
// Inferred: the scratchpad byte is an on/off switch read by VU or DMA-side code;
// the raster word is a "lighting state changed" flag.
// Unknown: the member and byte names, and the effect of values other than 0 and 1.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int EnableSunLight__8VIRasteri(void* raster, int enable) {
    *NORRATH_SCRATCH_BYTE(0x70000221) = static_cast<unsigned char>(enable);
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
