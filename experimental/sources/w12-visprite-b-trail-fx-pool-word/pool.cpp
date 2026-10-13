// Observed: the image's local symbol sTrailFxPool at 0x011e5214, published here as `VITrailFx$sTrailFxPool`. One word, zero in the image. VITrailFxInit stores in it the address just past an eight-byte element-count header (or zero when the allocation fails); VITrailFxClear reads it.
// Unknown: which class owns it, if any. Nothing here ran on the EE.
extern "C" {
unsigned norrath_trail_pool __asm__("VITrailFx$sTrailFxPool");
}
static_assert(sizeof(norrath_trail_pool) == 4, "width of sTrailFxPool");
