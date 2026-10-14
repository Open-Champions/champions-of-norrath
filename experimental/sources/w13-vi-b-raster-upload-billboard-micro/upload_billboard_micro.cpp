// Observed: UploadBillboardMicro__8VIRasterPP10_sceDmaTag at 0x01112548, 196 bytes. Like
// UploadRasterMicro__8VIRasterPP10_sceDmaTag the second argument is the address of the DMA
// write pointer, re-read at each step. It appends, as 16-byte entries:
//   tag A (byte 3 = 0x10, word 1 = 0, 16-bit field 0 = 1) with one payload entry
//         (0, 0, 0, 0x13000000);
//   tag B (byte 3 = 0x50, word 1 = address of VIDMABillboardMicrocode, field 0 unwritten);
//   tag C (field 0 = 0, byte 3 = 0x10, word 1 = 0, field 0 of the next entry = 1) with one
//         payload entry (0, 0, 0x0300021a, 0x020000f0).
// It then stores 1 at raster + 17892 and 2 at raster + 19532 and returns 0.
// Inferred: the billboard counterpart of the raster microprogram upload; the value 2 at
// raster + 19532 marks the billboard program as the loaded one (the raster variant stores 1).
// Unknown: member names, the exact VIF/DMA semantics of the constants, and the microcode
// image's size.
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
extern unsigned char VIDMABillboardMicrocode[];

int UploadBillboardMicro__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer) {
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
    w[1] = 0;
    w[3] = 0x13000000;
    w[2] = 0;
    NORRATH_LOAD_POINTER(slot)[3] = 0x50;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = static_cast<unsigned int>(reinterpret_cast<unsigned long>(VIDMABillboardMicrocode));
    p = NORRATH_LOAD_POINTER(slot);
    *reinterpret_cast<unsigned short*>(p) = 0;
    NORRATH_STORE_POINTER(slot, p + 16);
    (p + 16)[3] = 0x10;
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = 0;
    p = NORRATH_LOAD_POINTER(slot);
    w = reinterpret_cast<unsigned int*>(p + 16);
    NORRATH_STORE_POINTER(slot, p + 32);
    *reinterpret_cast<unsigned short*>(p) = 1;
    w[3] = 0x020000f0;
    w[2] = 0x0300021a;
    w[0] = 0;
    w[1] = 0;
    *reinterpret_cast<int*>(r + 17892) = 1;
    *reinterpret_cast<int*>(r + 19532) = 2;
    return 0;
}
}
