// nameGridSrcW: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: nameGridSrcW at 0x00486d64, type b (zero-initialised). Its address spacing (4 bytes) matches the neighbouring words.
//   - Callers: w15-gameloop-a-draw-name-pad/draw_name_pad.cpp declares it extern float and uses it in the Hermite interpolation of the name grid.
// Inferred:
//   - A 4-byte single-precision value, from the caller declaration and the address spacing.
// Unknown:
//   - The value after start-up, beyond the zero it has in the image.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
float nameGridSrcW;
}
static_assert(sizeof nameGridSrcW == 4u, "nameGridSrcW extent");
