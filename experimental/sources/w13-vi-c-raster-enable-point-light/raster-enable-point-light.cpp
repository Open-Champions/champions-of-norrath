// Observed: EnablePointLight__8VIRasterii at 0x01114130, 28 bytes. With the raster
// as this and ints i, v: the low byte of v is stored at the scratchpad address
// 0x70000222 + i, one is stored at this + 17888, and the result is zero.
// Inferred: the scratchpad bytes are per-light enable flags and the word at
// 17888 marks light state as changed (SetFog sets the same word). Unknown: the
// number of lights and the member names.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned char* norrath_host_scratchpad_byte(unsigned address);
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratchpad_byte(address)
#else
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int EnablePointLight__8VIRasterii(void* raster, int index, int enabled) {
    *NORRATH_SCRATCH_BYTE(0x70000222u + index) = static_cast<unsigned char>(enabled);
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
