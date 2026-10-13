// Observed: BeginTriFan__8VIRaster at 0x011131b8, 124 bytes. If the word at raster + 19532
// is not 1 it calls UploadRasterMicro(raster, raster + 19492). If the word at raster +
// 17888 is nonzero it calls CalcMatrices(raster) and UploadMatrices(raster, raster + 19492).
// It then takes the pointer stored at raster + 19492 (the current DMA write pointer, read
// again after the calls) and stores: raster + 48 = 0, raster + 52 = pointer + 48,
// raster + 56 = pointer + 16, raster + 17884 = 2. It returns 0.
// Inferred: starts a triangle-fan packet. The pointer words at +52 and +56 are the vertex
// write cursor and the start of the packet body; +17884 is the primitive mode (2 = fan) that
// EndTriFan clears; +48 is the vertex count Vertex advances; the word at +19532 says which
// microprogram is loaded, and +17888 is the "state changed" flag other setters raise.
// Unknown: the member names, the microprogram identity behind the value 1, and what
// the two Upload calls transmit in detail.
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
int UploadRasterMicro__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer);
int CalcMatrices__8VIRaster(void* raster);
int UploadMatrices__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer);

int BeginTriFan__8VIRaster(void* raster) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    if (*reinterpret_cast<int*>(r + 19532) != 1) UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, r + 19492);
    if (*reinterpret_cast<int*>(r + 17888) != 0) {
        CalcMatrices__8VIRaster(raster);
        UploadMatrices__8VIRasterPP10_sceDmaTag(raster, r + 19492);
    }
    unsigned char* dma = NORRATH_LOAD_POINTER(r + 19492);
    *reinterpret_cast<int*>(r + 48) = 0;
    NORRATH_STORE_POINTER(r + 52, dma + 48);
    NORRATH_STORE_POINTER(r + 56, dma + 16);
    *reinterpret_cast<int*>(r + 17884) = 2;
    return 0;
}
}
