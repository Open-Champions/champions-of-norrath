// vt_BlindingLight: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: _vt$18SpellBlindingLight at 0x00452d80, size 0x30, weak object (V).
//   - Callers: w15-m-spell-ctors-a-ctor-blinding-light/spell_blinding_light_ctor.cpp stores the table address at object +20.
// Inferred:
//   - 48 bytes, read as six 8-byte slots. Alignment 4 is inferred from pointer-sized words.
// Unknown:
//   - Every slot value, including the slots that replace the base-class table.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
alignas(4) unsigned char vt_BlindingLight[0x30] __asm__("_vt$18SpellBlindingLight");
}
static_assert(sizeof vt_BlindingLight == 0x30u, "vt_BlindingLight extent");
