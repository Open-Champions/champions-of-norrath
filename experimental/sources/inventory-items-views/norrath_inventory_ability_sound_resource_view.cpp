// Observed: the .rodata string "menu_char_incr.anm" at 0x466548, including its
// terminator. inventoryAdjustAbility__Fii (0x1b1938) passes this address as the
// resource name to lumpFindResource and plays the result as a sequence.
// Unknown: the padding after the terminator, which this symbol does not read.
extern "C" {
extern const unsigned char norrath_inventory_ability_sound_resource_view[];
const unsigned char norrath_inventory_ability_sound_resource_view[19] = { 0x6d, 0x65, 0x6e, 0x75, 0x5f, 0x63, 0x68, 0x61, 0x72, 0x5f, 0x69, 0x6e, 0x63, 0x72, 0x2e, 0x61, 0x6e, 0x6d, 0x00 };
}
static_assert(sizeof(norrath_inventory_ability_sound_resource_view) == 19, "complete object size");
