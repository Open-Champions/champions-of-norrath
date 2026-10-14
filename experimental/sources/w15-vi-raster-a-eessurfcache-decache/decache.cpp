// Observed: Decache__13VIEESurfCacheR9VISurface at 0x01132418 is 236 bytes in the pinned SLUS-20565 image. Arguments: the cache (this) and
// a surface. idx is the word at +24 of the surface; if it is -1 the routine returns 0 at once. Otherwise, with e the 104-byte cache entry
// at (array pointer at cache+64) + 104*idx, it:
//   1. if the word at e+28 is nonzero, calls Deallocate__13VIEESurfCacheR13VIEESurfAlloc(cache, e+20); a negative result returns -1;
//   2. calls Deallocate__13VIEESurfCacheR13VIEESurfAlloc(cache, e+36); a negative result returns -1;
//   3. calls RemoveFromLRU__13VIEESurfCachei(cache, idx); a negative result returns -1;
//   4. stores -1 at surface+24;
//   5. calls Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(cache+56, idx) and returns 0 if its result is not negative, else -1.
// Inferred: removes a surface from the cache: frees the two allocation records held in its entry (e+20 and e+36 are
// VIEESurfAlloc records; the +28 word of the first looks like its in-use mark), drops it from the LRU list, marks the surface as not cached
// (-1) and erases its map entry (the map object sits at cache+56 and its entry array pointer is the one at cache+64).
// Unknown: the record layouts, why the first record is freed conditionally, and the helpers' behavior (not authored here).
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Deallocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* self, unsigned char* alloc);
int RemoveFromLRU__13VIEESurfCachei(unsigned char* self, int index);
int Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* map, int key);
int Decache__13VIEESurfCacheR9VISurface(unsigned char* self, unsigned char* surface) {
    int index = *reinterpret_cast<int*>(surface + 24);
    if (index == -1) return 0;
    unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
    unsigned char* first = entries + index * 104 + 20;
    if (*reinterpret_cast<int*>(first + 8) != 0) {
        if (Deallocate__13VIEESurfCacheR13VIEESurfAlloc(self, first) < 0) return -1;
    }
    entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
    if (Deallocate__13VIEESurfCacheR13VIEESurfAlloc(self, entries + index * 104 + 36) < 0) return -1;
    if (RemoveFromLRU__13VIEESurfCachei(self, index) < 0) return -1;
    *reinterpret_cast<int*>(surface + 24) = -1;
    return Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(self + 56, index) >= 0 ? 0 : -1;
}
}
