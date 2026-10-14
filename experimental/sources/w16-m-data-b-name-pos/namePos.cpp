// namePos: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: namePos at 0x00486d10, type b (zero-initialised).
//   - Its neighbours nameGridW (0x00486d14) and nameGridCount (0x00486d18) are defined elsewhere as 4-byte ints, so this word is 4 bytes.
//   - Callers: w15-gameloop-a-do-name-pad/do_name_pad.cpp and w15-gameloop-a-reset-name/reset_name.cpp use it as an int.
// Inferred:
//   - The name-pool character count, from the callers.
// Unknown:
//   - The value after start-up, beyond the zero it has in the image.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
int namePos;
}
static_assert(sizeof namePos == 4u, "namePos extent");
