// Observed: Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag at 0x01133d18 is 100 bytes in the pinned SLUS-20565
// image. Arguments: the cache (this), a surface, a cache-info record, and the DMA write-pointer slot. It calls
// ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag with the same four arguments; a negative result
// returns -1 and changes nothing else. Otherwise it stores the static counter _13VIEESurfCache$Key into the info record at +80,
// increments that counter, adds one to the word at +28 of the cache, and returns 0.
// Inferred: the counter hands out increasing download keys (a version stamp that Cache/Set compare); +28 of the cache counts downloads.
// Unknown: what ConstructDMAPacket emits (not authored here), the counter's initial value and width, and the info layout beyond +80.
// Nothing here ran on the EE.
extern "C" {
extern int norrath_eessurfcache_key __asm__("_13VIEESurfCache$Key");
int ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char* self, unsigned char* surface,
                                                                                    unsigned char* info, unsigned char* writePointer);
int Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char* self, unsigned char* surface, unsigned char* info,
                                                                           unsigned char* writePointer) {
    if (ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(self, surface, info, writePointer) < 0) return -1;
    int key = norrath_eessurfcache_key;
    *reinterpret_cast<int*>(info + 80) = key;
    norrath_eessurfcache_key = key + 1;
    *reinterpret_cast<int*>(self + 28) += 1;
    return 0;
}
}
