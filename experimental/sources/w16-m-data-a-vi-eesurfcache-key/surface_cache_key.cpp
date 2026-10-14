// Storage for the surface-cache download counter read and incremented by
// Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag
// (see w15-vi-raster-a-eessurfcache-download).
// Observed: the reference image holds a 4-byte word at 0x011b0254 whose value is zero. The symbol is named
// _13VIEESurfCache$Key and sits in the SRS-loaded region, next to the BGM version word.
// Inferred: the word starts at zero and is only written by the download counter.
// Unknown: whether another routine writes it, and the value after the first download in the original.
extern "C" {
int norrath_eessurfcache_key __asm__("_13VIEESurfCache$Key") = 0;
}
