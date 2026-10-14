// Observed: Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi at 0x011320f0 is 804 bytes in the
// pinned SLUS-20565 image. Arguments, in order: the cache (this), a surface, a material, a material pass, the raster, a pointer to the
// current DMA write pointer and an int flag. Cache layout used: a word at +16, a pointer to 24-byte records at +40 (first word of a
// record is an offset), an embedded map starting at +56 whose entry array pointer is the word at +64; entries are 104 bytes and the
// cache-info block of entry i lives at entry + 20, with its word at +80 of that block (entry+100) holding a version stamp.
// Surface layout used: key word +20, cache slot index +24 (-1 when not cached), lock flag +32, a word at +40. Material: flag +32.
// Pass: word +72.
// Flow, with all negative callee results returning -1:
//   1. if surface+32 is nonzero, Decache__13VIEESurfCacheR9VISurface(cache, surface).
//   2. slot = surface+24. If slot != -1 (already cached): with flag 0 call MoveToLRUEnd__13VIEESurfCachei(cache, slot), otherwise
//      RemoveFromLRU__13VIEESurfCachei(cache, slot). If the material's +32 is zero and the entry's version word (entry+100) equals
//      the pass word +72, return 0 immediately. Otherwise call Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass
//      (cache, surface, entry+20, pass); then set material+32 = 1, pass+72 = entry version word, and return 0.
//   3. otherwise (not cached): fill a local info block with CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(cache, surface,
//      info). If surface+40 is nonzero: FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(cache, info, raster), add the first word of
//      the 24-byte record selected by info[0] (from the table at cache+40) to info word 3, and call
//      Allocate__13VIEESurfCacheR13VIEESurfAlloc(cache, info). Then, with a second block at info+16 (overlapping the info block):
//      FindLRU(cache, info+16, raster); with n = block word 3, add the record word (selected by block word 0) to each of the n words
//      starting at block word 4, re-reading n and the selector on every pass; Allocate(cache, info+16).
//      Then key = surface+20 and slot = Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(cache+56, &key, info); store it
//      in surface+24; with flag 0 call MoveToLRUEnd(cache, slot). If cache+16 is zero, call
//      Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(cache, surface, entry+20, dma pointer). Finally set
//      material+32 = 1, pass+72 = entry version word, call Set(cache, surface, entry+20, pass), clear surface+32 and return 1.
// Inferred: this ensures a surface's texture is resident in the EE-side surface cache (allocating, uploading and recording it if not),
// updates the LRU order unless the flag is set, and refreshes the pass if the cache entry changed. The result says whether a new entry
// was made (1) or an existing one used (0).
// Unknown: the info and alloc block layouts beyond the words named above (they overlap at +16, as in the original frame), the
// callees' bodies, and what the int flag means. The raster argument is only passed through. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Decache__13VIEESurfCacheR9VISurface(unsigned char* cache, unsigned char* surface);
void MoveToLRUEnd__13VIEESurfCachei(unsigned char* cache, int slot);
void RemoveFromLRU__13VIEESurfCachei(unsigned char* cache, int slot);
int Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(unsigned char* cache, unsigned char* surface, unsigned char* info, unsigned char* pass);
int CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(unsigned char* cache, unsigned char* surface, unsigned char* info);
int FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(unsigned char* cache, unsigned char* alloc, unsigned char* raster);
int Allocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* cache, unsigned char* alloc);
int Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(unsigned char* map, const int* key, const unsigned char* info);
int Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char* cache, unsigned char* surface, unsigned char* info, unsigned* dma);

static inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
static inline unsigned char* entries(unsigned char* cache) {
    return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(cache + 64));
}

int Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(
    unsigned char* cache, unsigned char* surface, unsigned char* material, unsigned char* pass, unsigned char* raster, unsigned* dma,
    int flag) {
    if (word(surface, 32) != 0) {
        if (Decache__13VIEESurfCacheR9VISurface(cache, surface) < 0) return -1;
    }
    int slot = word(surface, 24);
    if (slot != -1) {
        if (flag == 0) {
            MoveToLRUEnd__13VIEESurfCachei(cache, slot);
        } else {
            RemoveFromLRU__13VIEESurfCachei(cache, slot);
        }
        if (word(material, 32) == 0) {
            if (word(entries(cache) + slot * 104, 100) == word(pass, 72)) return 0;
        }
        if (Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(cache, surface, entries(cache) + slot * 104 + 20, pass) < 0) {
            return -1;
        }
        word(material, 32) = 1;
        word(pass, 72) = word(entries(cache) + slot * 104, 100);
        return 0;
    }
    alignas(16) unsigned char info[96];
    int key;
    if (CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(cache, surface, info) < 0) return -1;
    if (word(surface, 40) != 0) {
        if (FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(cache, info, raster) < 0) return -1;
        int selector = word(info, 0);
        unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(cache + 40));
        int adjusted = word(info, 12) + *reinterpret_cast<int*>(records + selector * 24);
        word(info, 12) = adjusted;
        if (Allocate__13VIEESurfCacheR13VIEESurfAlloc(cache, info) < 0) return -1;
    }
    unsigned char* block = info + 16;
    if (FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(cache, block, raster) < 0) return -1;
    if (word(block, 12) > 0) {
        unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(cache + 40));
        for (int i = 0; i < word(block, 12); ++i) {
            word(block, 16 + 4 * i) = word(block, 16 + 4 * i) + *reinterpret_cast<int*>(records + word(block, 0) * 24);
        }
    }
    if (Allocate__13VIEESurfCacheR13VIEESurfAlloc(cache, block) < 0) return -1;
    key = word(surface, 20);
    int created = Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(cache + 56, &key, info);
    if (created < 0) return -1;
    word(surface, 24) = created;
    if (flag == 0) MoveToLRUEnd__13VIEESurfCachei(cache, created);
    if (word(cache, 16) == 0) {
        if (Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(cache, surface, entries(cache) + created * 104 + 20, dma) < 0) {
            return -1;
        }
    }
    unsigned char* entry = entries(cache) + created * 104 + 20;
    word(material, 32) = 1;
    word(pass, 72) = word(entry, 80);
    if (Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(cache, surface, entry, pass) < 0) return -1;
    word(surface, 32) = 0;
    return 1;
}
}
