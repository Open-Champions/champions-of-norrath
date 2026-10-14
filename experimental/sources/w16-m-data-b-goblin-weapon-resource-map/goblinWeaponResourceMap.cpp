// goblinWeaponResourceMap: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: goblinWeaponResourceMap at 0x0042cfc8, size 0x80, type D (initialised data).
//   - Caller: w11-actor-a-drop-item-6goblin/drop_item.cpp reads 8-byte entries at index n * 8.
//   - ctors-ant-lantern-views/norrath_ctors_ant_lantern_view_42cfc8.cpp names the same address as a separate view.
// Inferred:
//   - 128 bytes, 16 entries of 8 bytes, from the symbol size and the caller stride.
// Unknown:
//   - The entry contents.
//   - Which storage survives integration: this unit and the ctors-ant-lantern view name one address.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char goblinWeaponResourceMap[128];
}
static_assert(sizeof goblinWeaponResourceMap == 128u, "goblinWeaponResourceMap extent");
