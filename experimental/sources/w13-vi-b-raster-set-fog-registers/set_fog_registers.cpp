// Observed: SetFogRegisters__8VIRaster at 0x0110fbc8, 504 bytes. It reads the first three of
// the four floats at raster + 17560, turns each into a byte as (unsigned)(value * 255.0f + 0.5f)
// (the fourth float is converted too but its byte is never used), and appends four 16-byte
// entries at the DMA write pointer (raster + 19492), advancing the pointer by 64:
//   entry 0: byte 3 = 0x10, word 1 = 0, 16-bit field 0 = 3 (a three-entry payload follows);
//   entry 1: words (0, 0, 0x10000000, 0x50000002);
//   entry 2: two 64-bit values 0x1000000000008001 and 14;
//   entry 3: two 64-bit values (red | green << 8 | blue << 16) and 61 (0x3d).
// It returns 0.
// Inferred: a GIF packet that writes the fog color register (0x3d is the GS FOGCOL register
// address; 14 selects the A+D register mode) from a floating-point color kept in the raster,
// with the 0x10 DMA tag, as in the other display-list builders.
// Unknown: the member names, the role of the fourth float, and out-of-range handling beyond
// the compiler's own float-to-unsigned conversion.
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

static inline unsigned char VIFogByte(float value) {
    return static_cast<unsigned char>(static_cast<unsigned int>(value * 255.0f + 0.5f));
}

extern "C" {
int SetFogRegisters__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned char** slot = reinterpret_cast<unsigned char**>(r + 19492);
    const float* color = reinterpret_cast<const float*>(r + 17560);
    NORRATH_LOAD_POINTER(slot)[3] = 0x10;
    unsigned char red = VIFogByte(color[0]);
    reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(slot))[1] = 0;
    unsigned char* p = NORRATH_LOAD_POINTER(slot);
    NORRATH_STORE_POINTER(slot, p + 64);
    *reinterpret_cast<unsigned short*>(p) = 3;
    unsigned char green = VIFogByte(color[1]);
    unsigned char blue = VIFogByte(color[2]);
    unsigned int* words = reinterpret_cast<unsigned int*>(p + 16);
    words[3] = 0x50000002;
    words[2] = 0x10000000;
    words[0] = 0;
    words[1] = 0;
    unsigned long long* tag = reinterpret_cast<unsigned long long*>(p + 32);
    tag[1] = 14;
    tag[0] = 0x1000000000008001ull;
    unsigned long long* data = reinterpret_cast<unsigned long long*>(p + 48);
    data[1] = 61;
    data[0] = static_cast<unsigned long long>(red) | (static_cast<unsigned long long>(green) << 8) | (static_cast<unsigned long long>(blue) << 16);
    return 0;
}
}
