// vt_SpellRoot: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: _vt$9SpellRoot at 0x00452180, size 0x30, weak object (V).
//   - Callers: w14-m-spell-ctors-b-ctor-root/ctor.cpp stores the table address at object +20.
// Inferred:
//   - 48 bytes, read as six 8-byte slots. Alignment 4 is inferred from pointer-sized words.
// Unknown:
//   - Every slot value.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
alignas(4) unsigned char vt_SpellRoot[0x30] __asm__("_vt$9SpellRoot");
}
static_assert(sizeof vt_SpellRoot == 0x30u, "vt_SpellRoot extent");
