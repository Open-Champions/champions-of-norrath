// Observed: SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight at 0x01114080,
// 168 bytes.
// With the raster as this, an index i, a matrix and a light record (32 bytes): the slot is
// the scratchpad address 0x700001c0 + i * 32. The four floats at light + 0 are copied to slot
// + 0, then the 16 bytes at light + 16 are copied to slot + 16 (as two unaligned 8-byte
// moves). Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, light, slot) is then called, which
// overwrites the first three floats of the slot with the transformed light position. Finally
// one is stored at this + 17888 (the same changed-state word EnablePointLight sets) and the
// result is zero.
// Inferred: scratchpad slots are per-light records of a world position (with a fourth
// float kept unchanged) followed by 16 bytes of colour/attenuation data. The state word
// marks lighting as changed. Unknown: field names and the number of slots.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_scratchpad_byte(unsigned address);
#define NORRATH_SCRATCH_BYTES(address) norrath_host_scratchpad_byte(address)
#else
#define NORRATH_SCRATCH_BYTES(address) reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out);

int SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight(void* raster, int index, const void* matrix,
                                                                   const void* light) {
    unsigned char* slot = NORRATH_SCRATCH_BYTES(0x700001c0u + static_cast<unsigned>(index) * 32u);
    const float* from = static_cast<const float*>(light);
    float* to = reinterpret_cast<float*>(slot);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
    const unsigned char* colour = static_cast<const unsigned char*>(light) + 16;
    for (int i = 0; i < 16; ++i) slot[16 + i] = colour[i];
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, light, slot);
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
