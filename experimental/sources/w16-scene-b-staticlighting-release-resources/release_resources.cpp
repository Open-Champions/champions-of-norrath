// Observed: ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary at
// 0x010122f8 is 140 bytes. The lighting object holds an array at +8 (count at +8, data
// pointer at +12). For index 0 up to the count, re-read after every call, it calls
// ReleaseColorBuffer__8VIRasteriR12VIDictionary(raster, data[index], dictionary), with
// the data pointer re-read each pass. It returns 0.
// Inferred: the elements are colour-buffer ids released back to the raster.
// Unknown: the element type beyond a 32-bit word and ReleaseColorBuffer's result.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int ReleaseColorBuffer__8VIRasteriR12VIDictionary(void* raster, int buffer, void* dictionary);

int ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(unsigned char* self, void* raster, void* dictionary) {
    unsigned* array = reinterpret_cast<unsigned*>(self + 8);
    int i = 0;
    if (static_cast<int>(array[0]) > 0) {
        do {
            const int* data = reinterpret_cast<const int*>(at(array[1]));
            ReleaseColorBuffer__8VIRasteriR12VIDictionary(raster, data[i], dictionary);
            ++i;
        } while (i < static_cast<int>(array[0]));
    }
    return 0;
}
}
