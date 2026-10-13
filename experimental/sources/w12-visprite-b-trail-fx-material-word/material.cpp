// Observed: the image's local symbol sMaterialHandle at 0x0118e7d8, published here as `VITrailFx$sMaterialHandle`. One word; the image holds 0xffffffff, which VITrailFxClear tests against -1 and writes back after releasing the material, and VITrailFxInit overwrites with the handle it creates.
// Unknown: which class owns it, if any. Nothing here ran on the EE.
extern "C" {
int norrath_trail_material __asm__("VITrailFx$sMaterialHandle") = -1;
}
static_assert(sizeof(norrath_trail_material) == 4, "width of sMaterialHandle");
