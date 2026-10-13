// Observed: the .rodata string "game_lvl_complet.anm" at 0x466348, including its
// terminator (21 bytes). inventoryExit__Fiii (0x1acc20) passes this address as the
// resource name to lumpFindResource__FPcT0, with the lump name at 0x484828, and
// plays the result as a sequence when the player is local.
// Unknown: the padding after the terminator, which this symbol does not read.
extern "C" {
extern const unsigned char norrath_inventory_exit_sound_resource_view[];
const unsigned char norrath_inventory_exit_sound_resource_view[21] = { 0x67, 0x61, 0x6d, 0x65, 0x5f, 0x6c, 0x76, 0x6c, 0x5f, 0x63, 0x6f, 0x6d, 0x70, 0x6c, 0x65, 0x74, 0x2e, 0x61, 0x6e, 0x6d, 0x00 };
}
static_assert(sizeof(norrath_inventory_exit_sound_resource_view) == 21, "complete object size");
