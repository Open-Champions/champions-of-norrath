// Observed: UploadMatrices__8VIRasterPP10_sceDmaTag at 0x01111f50, 804 bytes. The second
// argument is the address of the DMA write pointer (raster + 19492 in the callers). It writes
// one DMA tag (byte 3 = 0x10, word 1 = 0, 16-bit field 0 = 23) at the pointer, advances the
// pointer by 384 and fills the 23 following 16-byte entries from the scratchpad block
// 0x70000000..0x700002ff (S(n) = the float at 0x70000000 + n):
//   entry  0: words (0, 0, 0x13000000, 0x6c150000)
//   entries 1-3: sceVu0CopyMatrix of the 64-byte matrices at S(0x140), S(0x80), S(0xc0)
//   entry  4: S(384), -S(392), S(388), 0
//   entry  5: S(400), S(404), S(408), then 1.0f (or 1.0f / S(460) if the byte at 0x70000222 is set)
//   entry  6: raster word at +17864 (copied bitwise), S(556), 0, S(552)
//   entry  7: S(428..440) if the byte at 0x70000221 is set, else zeros
//   entry  8: S(464), S(468), S(472), 0 if the byte at 0x70000222 is set, else zeros
//   entry  9: S(480..492) if the byte at 0x70000220 is set, else zeros
//   entries 10-12: S(528..540), S(496..508), S(512..524)
//   entry 13 (last): 0, 0, 0 and (static word _8VIRaster$RInitOffset | 0x14000000)
// Finally it stores 0 at raster + 17888, clears the four scratchpad bytes at 0x70000224..227
// and returns 0.
// Inferred: uploads the per-frame matrices, camera vectors and light data (the three bytes at
// 0x70000220..222 are the switches EnableAmbient, EnableSunLight and the point-light enable
// set; the vector entries are fog/light/eye data for the VU microprogram) and clears the
// "state changed" flag (raster + 17888) other setters raise. The final entry mirrors the
// closing entry EndTriFan writes with RFanOffset.
// Unknown: the member names, which matrices the three copies carry, what the scratchpad
// floats mean individually, the static word's type (unsigned int assumed), and the switches'
// exact roles beyond being nonzero/zero flags.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#define NORRATH_SCRATCH_BYTE(address) norrath_host_scratch_byte(address)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#define NORRATH_SCRATCH_BYTE(address) reinterpret_cast<volatile unsigned char*>(static_cast<unsigned long>(address))
#endif
#define NORRATH_SCRATCH(address) const_cast<unsigned char*>(NORRATH_SCRATCH_BYTE(address))

extern "C" {
extern unsigned int VIRasterInitOffset __asm__("_8VIRaster$RInitOffset");
void sceVu0CopyMatrix(void* destination, const void* source);

int UploadMatrices__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned char** slot = static_cast<unsigned char**>(tagPointer);
    const float* S = reinterpret_cast<const float*>(NORRATH_SCRATCH(0x70000000));
    NORRATH_LOAD_POINTER(slot)[3] = 0x10;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = 0;
    unsigned char* p = NORRATH_LOAD_POINTER(slot);
    unsigned int* e = reinterpret_cast<unsigned int*>(p + 16);
    NORRATH_STORE_POINTER(slot, p + 384);
    *reinterpret_cast<unsigned short*>(p) = 23;
    e[3] = 0x6c150000;
    e[2] = 0x13000000;
    e[0] = 0;
    e[1] = 0;
    sceVu0CopyMatrix(p + 32, NORRATH_SCRATCH(0x70000140));
    sceVu0CopyMatrix(p + 96, NORRATH_SCRATCH(0x70000080));
    sceVu0CopyMatrix(p + 160, NORRATH_SCRATCH(0x700000c0));
    float* f = reinterpret_cast<float*>(p + 224);
    f[0] = S[96];
    f[1] = -S[98];
    reinterpret_cast<unsigned int*>(f)[3] = 0;
    f[2] = S[97];
    f = reinterpret_cast<float*>(p + 240);
    f[0] = S[100];
    f[1] = S[101];
    f[2] = S[102];
    if (*NORRATH_SCRATCH_BYTE(0x70000222) != 0) f[3] = 1.0f / S[115];
    else f[3] = 1.0f;
    f = reinterpret_cast<float*>(p + 256);
    reinterpret_cast<unsigned int*>(f)[0] = *reinterpret_cast<unsigned int*>(r + 17864);
    f[1] = S[139];
    reinterpret_cast<unsigned int*>(f)[2] = 0;
    f[3] = S[138];
    f = reinterpret_cast<float*>(p + 272);
    if (*NORRATH_SCRATCH_BYTE(0x70000221) != 0) {
        f[0] = S[107];
        f[1] = S[108];
        f[2] = S[109];
        f[3] = S[110];
    } else {
        reinterpret_cast<unsigned int*>(f)[0] = 0;
        reinterpret_cast<unsigned int*>(f)[1] = 0;
        reinterpret_cast<unsigned int*>(f)[2] = 0;
        reinterpret_cast<unsigned int*>(f)[3] = 0;
    }
    f += 4;
    if (*NORRATH_SCRATCH_BYTE(0x70000222) != 0) {
        f[0] = S[116];
        f[1] = S[117];
        reinterpret_cast<unsigned int*>(f)[3] = 0;
        f[2] = S[118];
    } else {
        reinterpret_cast<unsigned int*>(f)[0] = 0;
        reinterpret_cast<unsigned int*>(f)[1] = 0;
        reinterpret_cast<unsigned int*>(f)[2] = 0;
        reinterpret_cast<unsigned int*>(f)[3] = 0;
    }
    f += 4;
    if (*NORRATH_SCRATCH_BYTE(0x70000220) != 0) {
        f[0] = S[120];
        f[1] = S[121];
        f[2] = S[122];
        f[3] = S[123];
    } else {
        reinterpret_cast<unsigned int*>(f)[0] = 0;
        reinterpret_cast<unsigned int*>(f)[1] = 0;
        reinterpret_cast<unsigned int*>(f)[2] = 0;
        reinterpret_cast<unsigned int*>(f)[3] = 0;
    }
    f += 4;
    f[0] = S[132];
    f[1] = S[133];
    f[2] = S[134];
    f[3] = S[135];
    f += 4;
    f[0] = S[124];
    f[1] = S[125];
    f[2] = S[126];
    f[3] = S[127];
    f += 4;
    f[0] = S[128];
    f[1] = S[129];
    f[2] = S[130];
    f[3] = S[131];
    f += 4;
    reinterpret_cast<unsigned int*>(f)[0] = 0;
    reinterpret_cast<unsigned int*>(f)[1] = 0;
    reinterpret_cast<unsigned int*>(f)[2] = 0;
    reinterpret_cast<unsigned int*>(f)[3] = VIRasterInitOffset | 0x14000000;
    *reinterpret_cast<int*>(r + 17888) = 0;
    *NORRATH_SCRATCH_BYTE(0x70000224) = 0;
    *NORRATH_SCRATCH_BYTE(0x70000225) = 0;
    *NORRATH_SCRATCH_BYTE(0x70000226) = 0;
    *NORRATH_SCRATCH_BYTE(0x70000227) = 0;
    return 0;
}
}
