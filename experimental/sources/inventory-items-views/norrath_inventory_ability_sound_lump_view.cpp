// Observed: the .sdata word pair at 0x484828 holds the C string "fx.lmp" and
// zero padding. inventoryAdjustAbility__Fii (0x1b1938) passes this address as
// the lump name to lumpFindResource. The bytes before it end at 0x484828 and
// inventoryDrawDelay starts at 0x484830, so the object is those 8 bytes.
// Unknown: whether other code reads the padding.
extern "C" {
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
alignas(4) const unsigned char norrath_inventory_ability_sound_lump_view[8] = { 0x66, 0x78, 0x2e, 0x6c, 0x6d, 0x70, 0x00, 0x00 };
}
static_assert(sizeof(norrath_inventory_ability_sound_lump_view) == 8, "complete object size");
