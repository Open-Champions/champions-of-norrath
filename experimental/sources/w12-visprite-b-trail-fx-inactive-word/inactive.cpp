// Observed: the image's local symbol sInactiveTrailFx at 0x011e5208, published here as `VITrailFx$sInactiveTrailFx`. The next local symbol is 12 bytes on. The image holds zeros here.
// Inferred: one 12-byte list object (see the destructor offsets of the trail element).
// Unknown: which class owns it, if any. Nothing here ran on the EE.
extern "C" {
unsigned norrath_trail_inactive[3] __asm__("VITrailFx$sInactiveTrailFx");
}
static_assert(sizeof(norrath_trail_inactive) == 12, "width of sInactiveTrailFx");
