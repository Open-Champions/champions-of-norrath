// Observed .data symbol chestResourceList at 0x42c8a0, 32 bytes.
// Eight words, each the address of a resource map. Callers index it by the
// chest kind times four. The maps are listed by their original names, each
// an import named for its address. Unknown: owner of those maps.
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
ResourceSlot* norrath_ctors_ant_lantern_view_42c8a0[8] = {
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
