// Observed: EndTriFan__8VIRaster at 0x0110ca88, 260 bytes. Using the packet cursor at
// raster + 52 and the packet start at raster + 56 (see BeginTriFan__8VIRaster) it:
//  1. zeroes three words at the cursor and stores (static word _8VIRaster$RFanOffset | 0x14000000)
//     into the cursor's fourth word, closing the packet with one more 16-byte entry;
//  2. sets n = (cursor + 16 - start) / 16, resets the cursor to start, zeroes start's first
//     three words and stores ((n - 2) << 16) | 0x6c008000 into start's fourth word;
//  3. moves the cursor to start + 16, stores the vertex counter (raster + 48) in its first
//     word, zeroes the other three, and moves the cursor on to start + 32;
//  4. in the DMA tag at the pointer at raster + 19492, stores 0x10 in byte 3, zero in word
//     1 and n in the 16-bit field at +0; advances that pointer by n * 16 + 16;
//  5. clears raster + 17884 and calls FlushDMABufferIfFull(raster). It returns 0.
// Inferred: closes a triangle-fan packet started by BeginTriFan: the 0x6c008000 header
// word looks like a VIF unpack command with count n - 2 and the 0x14000000 mask a GS/GIF-side
// tag, and the byte 0x10 sets the DMA tag's id field to "cnt".
// Unknown: the member names, the packed field meaning, the type of the static word (an
// unsigned int is assumed), and whether the cursor reset in step 2 is intentional.
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
extern unsigned int VIRasterFanOffset __asm__("_8VIRaster$RFanOffset");
int FlushDMABufferIfFull__8VIRaster(void* raster);

int EndTriFan__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    unsigned int* end = reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(r + 52));
    end[0] = 0;
    end[1] = 0;
    end[2] = 0;
    end[3] = VIRasterFanOffset | 0x14000000;
    unsigned int* start = reinterpret_cast<unsigned int*>(NORRATH_LOAD_POINTER(r + 56));
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(start));
    start[0] = 0;
    int count = static_cast<int>(reinterpret_cast<unsigned char*>(end) + 16 - reinterpret_cast<unsigned char*>(start)) >> 4;
    start[1] = 0;
    start[2] = 0;
    start[3] = (static_cast<unsigned int>(count - 2) << 16) | 0x6c008000u;
    unsigned int* body = start + 4;
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(body));
    body[0] = *reinterpret_cast<unsigned int*>(r + 48);
    body[1] = 0;
    body[2] = 0;
    body[3] = 0;
    NORRATH_STORE_POINTER(r + 52, reinterpret_cast<unsigned char*>(body + 4));
    unsigned char* tag = NORRATH_LOAD_POINTER(r + 19492);
    tag[3] = 0x10;
    *reinterpret_cast<int*>(r + 17884) = 0;
    reinterpret_cast<unsigned int*>(tag)[1] = 0;
    *reinterpret_cast<unsigned short*>(tag) = static_cast<unsigned short>(count);
    NORRATH_STORE_POINTER(r + 19492, tag + count * 16 + 16);
    FlushDMABufferIfFull__8VIRaster(raster);
    return 0;
}
}
