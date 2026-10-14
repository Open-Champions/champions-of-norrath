// Storage for the active-trail list of the trail effect system.
// Observed: the image's local symbol sActiveTrailFx is at 0x011e51f8 in the
// SRS overlay section, 16 bytes to the next symbol (sInactiveTrailFx at
// 0x011e5208); the file bytes are zero. Its siblings sFreeTrailFx (0x011e51e8)
// and sInactiveTrailFx (0x011e5208) have the same spacing and are not defined
// here. Authored code reaches the symbol as VITrailFx$sActiveTrailFx: the
// first-link word at +0 is what Render and Update walk, the list header is 12
// bytes (three words) as Init__7VILList sets it up, and the fourth word is
// padding to the 16-byte spacing.
// Unknown: whether the final word is padding or belongs to another object.
extern "C" {
alignas(8) unsigned norrath_trail_active_list[4] __asm__("VITrailFx$sActiveTrailFx");
}
