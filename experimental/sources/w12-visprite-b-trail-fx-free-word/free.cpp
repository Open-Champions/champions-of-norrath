// Observed: the image's local symbol sFreeTrailFx at 0x011e51e8, published here as `VITrailFx$sFreeTrailFx`. The next local symbol is 16 bytes on; the first 12 bytes are the list. The image holds zeros here.
// Inferred: the 12 bytes are one list object, since the trail element destructor takes its two member lists 12 bytes apart (+28 and +40), and the other trail routines run the list calls on this address. The remaining 4 bytes to the next symbol are alignment padding and are not defined here.
// Unknown: which class owns it, if any. Nothing here ran on the EE.
extern "C" {
unsigned norrath_trail_free[3] __asm__("VITrailFx$sFreeTrailFx");
}
static_assert(sizeof(norrath_trail_free) == 12, "width of sFreeTrailFx");
