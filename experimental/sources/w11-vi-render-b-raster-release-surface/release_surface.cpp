// Observed: ReleaseSurface__8VIRasteriR12VIDictionary at 0x0110d990 is 224 bytes. The integer is
// an index into the pool at offset 19380 of the raster; the pool's element array pointer is at
// pool+8 and elements are 12 bytes, the first word being the surface pointer. The surface's
// word at offset 4 is decremented (and stored); if the result is still positive the function
// returns zero. Otherwise, in order, it:
//   1. calls Decache__13VIEESurfCacheR9VISurface(&SurfCache, surface); a negative result returns -1;
//   2. if the surface's word at offset 0 is nonzero, calls
//      Remove__12VIDictionaryR10VIResource(dictionary, surface); a negative result returns -1;
//   3. stores the raster word at 17880 into the surface's word at offset 36;
//   4. calls Add__t6VIList1ZP9VISurfaceRCP9VISurface(raster+19404, &surface) with the surface
//      pointer held in a local; a negative result returns -1;
//   5. calls Erase__t6VIPool1ZP9VISurfacei(pool, index); the result is zero if that is not
//      negative and -1 otherwise.
// Inferred: offset 4 is a reference count; offset 36 is the swap counter at the time of release
// (the value ReleaseDeferred compares against); offset 0 is a dictionary-membership word.
// Unknown: the surface layout beyond these words, and the exact type of the first element word.
extern "C" {
struct VIRaster;
struct VIDictionary;

extern unsigned char norrath_surf_cache[] __asm__("SurfCache");
int Decache__13VIEESurfCacheR9VISurface(void* cache, void* surface);
int Remove__12VIDictionaryR10VIResource(VIDictionary* dictionary, void* resource);
int Add__t6VIList1ZP9VISurfaceRCP9VISurface(void* list, void* surfaceSlot);
int Erase__t6VIPool1ZP9VISurfacei(void* pool, int index);

int ReleaseSurface__8VIRasteriR12VIDictionary(VIRaster* raster, int index, VIDictionary& dictionary) {
    char* base = reinterpret_cast<char*>(raster);
    char* pool = base + 19380;
    char* surface = *reinterpret_cast<char**>(*reinterpret_cast<char**>(pool + 8) + index * 12);
    char* slot = surface;
    int references = *reinterpret_cast<int*>(surface + 4) - 1;
    *reinterpret_cast<int*>(surface + 4) = references;
    if (references > 0) return 0;
    if (Decache__13VIEESurfCacheR9VISurface(norrath_surf_cache, slot) < 0) return -1;
    if (*reinterpret_cast<int*>(slot) != 0) {
        if (Remove__12VIDictionaryR10VIResource(&dictionary, slot) < 0) return -1;
    }
    *reinterpret_cast<int*>(slot + 36) = *reinterpret_cast<int*>(base + 17880);
    if (Add__t6VIList1ZP9VISurfaceRCP9VISurface(base + 19404, &slot) < 0) return -1;
    if (Erase__t6VIPool1ZP9VISurfacei(pool, index) < 0) return -1;
    return 0;
}
}
