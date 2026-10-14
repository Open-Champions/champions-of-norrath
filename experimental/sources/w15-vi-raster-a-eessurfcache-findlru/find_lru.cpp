// Observed: FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster at 0x01132bb8 is 320 bytes in the pinned SLUS-20565 image. Arguments: the
// cache (this), an allocation request record, and the raster. It loops:
//   r = FindFree__13VIEESurfCacheR13VIEESurfAlloc(cache, request); if r is nonzero, return r.
//   Otherwise take the first-entry index f (word at cache+8); if it is -1 return -1. With e the 104-byte entry at
//   (array pointer at cache+64) + 104*f, if the word at e+28 is nonzero call Deallocate__13VIEESurfCacheR13VIEESurfAlloc(cache, e+20)
//   (negative -> return -1); call Deallocate__13VIEESurfCacheR13VIEESurfAlloc(cache, e+36) (negative -> return -1); reread the first
//   index and call RemoveFromLRU__13VIEESurfCachei(cache, it) (negative -> return -1); look up the surface index with
//   Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi(cache+56, it) (the first word of what it returns), get that surface from the raster with
//   Surface__8VIRasteri(raster, key), store -1 at surface+24, call Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(cache+56, it) (negative ->
//   return -1) and loop.
// Inferred: evicts least-recently-used entries until a free block of the requested size is found, as in Decache__13VIEESurfCacheR9VISurface
// but also resolving the surface from the raster. The return value is whatever FindFree reports (zero means not found yet).
// Unknown: the request record, FindFree's result convention beyond zero/nonzero, and the helpers' behavior (not authored here).
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int FindFree__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* self, unsigned char* request);
int Deallocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* self, unsigned char* alloc);
int RemoveFromLRU__13VIEESurfCachei(unsigned char* self, int index);
const int* Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int index);
unsigned char* Surface__8VIRasteri(unsigned char* raster, int index);
int Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int key);
int FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(unsigned char* self, unsigned char* request, unsigned char* raster) {
    int found;
    while ((found = FindFree__13VIEESurfCacheR13VIEESurfAlloc(self, request)) == 0) {
        int first = *reinterpret_cast<int*>(self + 8);
        if (first == -1) return -1;
        unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
        unsigned char* entry = entries + first * 104 + 20;
        if (*reinterpret_cast<int*>(entry + 8) != 0) {
            if (Deallocate__13VIEESurfCacheR13VIEESurfAlloc(self, entry) < 0) return -1;
            first = *reinterpret_cast<int*>(self + 8);
        }
        entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
        if (Deallocate__13VIEESurfCacheR13VIEESurfAlloc(self, entries + first * 104 + 36) < 0) return -1;
        int index = *reinterpret_cast<int*>(self + 8);
        if (RemoveFromLRU__13VIEESurfCachei(self, index) < 0) return -1;
        unsigned char* surface = Surface__8VIRasteri(raster, *Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi(self + 56, index));
        *reinterpret_cast<int*>(surface + 24) = -1;
        if (Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(self + 56, index) < 0) return -1;
    }
    return found;
}
}
