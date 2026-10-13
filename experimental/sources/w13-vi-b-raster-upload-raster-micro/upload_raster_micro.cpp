// Observed: UploadRasterMicro__8VIRasterPP10_sceDmaTag at 0x01112460, 232 bytes. The second
// argument is the address of the DMA write pointer (raster + 19492 in the callers). Each
// step below reads the pointer afresh. The function appends, as 16-byte entries:
//   tag A (byte 3 = 0x10, word 1 = 0, 16-bit field 0 = 1) with one payload entry
//         (0, 0, 0, 0x13000000);
//   tag B (byte 3 = 0x50, word 1 = address of VIDMARasterMicrocode, field 0 left unwritten);
//   tag C (field 0 = 0, byte 3 = 0x10, word 1 = 0, field 0 of the next entry = 1) with one
//         payload entry (0, 0, PrimBuffer1 | 0x03000000, (PrimBuffer2 - PrimBuffer1) | 0x02000000).
// It then stores 1 at raster + 17896, 19532 and 17888 and returns 0. PrimBuffer1 and
// PrimBuffer2 are the static words _8VIRaster$PrimBuffer1 and _8VIRaster$PrimBuffer2.
// Inferred: queues a microprogram upload: the 0x10 tags look like "cnt" DMA tags, the 0x50 tag a
// "call" tag into the microcode image; the payload words are VIF commands (0x13000000 a
// wait/flush, the last pair an unpack or mscal-style setup with the buffer size).
// The word at raster + 19532 (set to 1 here, 2 by the billboard variant) records which
// microprogram is loaded; BeginTriFan skips this upload when it already is 1.
// Unknown: member names, the exact VIF/DMA semantics, the static words' types (unsigned int
// is assumed) and the microcode image's size.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_STORE_POINTER(field, value) norrath_host_store_pointer(field, value)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_STORE_POINTER(field, value) (*reinterpret_cast<unsigned char**>(field) = (value))
#endif

extern "C" {
extern unsigned char VIDMARasterMicrocode[];
extern unsigned int VIRasterPrimBuffer1 __asm__("_8VIRaster$PrimBuffer1");
extern unsigned int VIRasterPrimBuffer2 __asm__("_8VIRaster$PrimBuffer2");

int UploadRasterMicro__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned char** slot = static_cast<unsigned char**>(tagPointer);
    unsigned char* p;
    unsigned int* w;
    NORRATH_LOAD_POINTER(slot)[3] = 0x10;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = 0;
    p = NORRATH_LOAD_POINTER(slot);
    w = reinterpret_cast<unsigned int*>(p + 16);
    NORRATH_STORE_POINTER(slot, p + 32);
    *reinterpret_cast<unsigned short*>(p) = 1;
    w[0] = 0;
    w[3] = 0x13000000;
    w[1] = 0;
    w[2] = 0;
    NORRATH_LOAD_POINTER(slot)[3] = 0x50;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = static_cast<unsigned int>(reinterpret_cast<unsigned long>(VIDMARasterMicrocode));
    p = NORRATH_LOAD_POINTER(slot);
    *reinterpret_cast<unsigned short*>(p) = 0;
    NORRATH_STORE_POINTER(slot, p + 16);
    (p + 16)[3] = 0x10;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = 0;
    p = NORRATH_LOAD_POINTER(slot);
    w = reinterpret_cast<unsigned int*>(p + 16);
    *reinterpret_cast<unsigned short*>(p) = 1;
    NORRATH_STORE_POINTER(slot, p + 32);
    w[0] = 0;
    w[1] = 0;
    unsigned int first = VIRasterPrimBuffer1;
    w[2] = first | 0x03000000;
    w[3] = (VIRasterPrimBuffer2 - first) | 0x02000000;
    *reinterpret_cast<int*>(r + 17896) = 1;
    *reinterpret_cast<int*>(r + 19532) = 1;
    *reinterpret_cast<int*>(r + 17888) = 1;
    return 0;
}
}
