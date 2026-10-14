// nameGridPos: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: nameGridPos at 0x00486d0c, type b (zero-initialised).
//   - Callers: w15-gameloop-a-draw-name-pad, w15-gameloop-a-switch-to-name-pad and w15-gameloop-a-do-name-pad declare it as extern int.
// Inferred:
//   - 4 bytes, from the address spacing of the neighbouring words.
// Unknown:
//   - The meaning of the special values the callers test, such as -3 in switch-to-name-pad.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
int nameGridPos;
}
static_assert(sizeof nameGridPos == 4u, "nameGridPos extent");
