// vt_VIFlatFile: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: _vt$10VIFlatFile at 0x011f3a90, size 0x18, weak object (V).
//   - Callers: w15-vi-containers-a-flat-file-directory-dtor/dtor.cpp declares it as an external array and stores its address at this+16.
// Inferred:
//   - Six 4-byte words, or three 8-byte slots. The slot width follows the other tables of this kind; not verified against a slot list.
// Unknown:
//   - Every slot value and every function the slots name.
//   - Whether the game writes the table after start-up.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
alignas(4) unsigned char vt_VIFlatFile[0x18] __asm__("_vt$10VIFlatFile");
}
static_assert(sizeof vt_VIFlatFile == 0x18u, "vt_VIFlatFile extent");
