// chestResourceList at 0x0042c8a0, 32 bytes of initialized .data.
// Observed: eight pointer words, each the address of a 40-byte resource map. In order they are the
// maps the symbol table names chestResourceMap (0x42c760), chest2ResourceMap (0x42c788),
// chest3ResourceMap (0x42c7b0), chestCavResourceMap (0x42c7d8), onyxChestResourceMap (0x42c800),
// iceChestLargeResourceMap (0x42c828), wicker1ResourceMap (0x42c850) and wicker2ResourceMap
// (0x42c878); the words are consecutive 40-byte steps. The chest message loader selects an entry
// by a signed kind byte.
// The maps are provided by earlier packages under address-named views, so this table points at
// those views and no new import is opened. The view that repeats this table, at 0x42c8a0, is a
// separate object.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" {
extern ResourceSlot norrath_ctors_ant_lantern_view_42c760[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c788[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c7b0[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c7d8[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c800[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c828[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c850[];
extern ResourceSlot norrath_ctors_ant_lantern_view_42c878[];
ResourceSlot* chestResourceList[8] = {
    norrath_ctors_ant_lantern_view_42c760,
    norrath_ctors_ant_lantern_view_42c788,
    norrath_ctors_ant_lantern_view_42c7b0,
    norrath_ctors_ant_lantern_view_42c7d8,
    norrath_ctors_ant_lantern_view_42c800,
    norrath_ctors_ant_lantern_view_42c828,
    norrath_ctors_ant_lantern_view_42c850,
    norrath_ctors_ant_lantern_view_42c878,
};
}
