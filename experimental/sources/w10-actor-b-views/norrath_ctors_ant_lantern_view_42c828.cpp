// Observed .data symbol iceChestLargeResourceMap at 0x42c828, 40 bytes.
// Four name slots, each a name pointer followed by a zero word, then a null
// slot. Chest picks it through chestResourceList by kind. Unknown: which slot
// indexes the game uses. Pointer identity with other copies of these names
// is not preserved.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "icechst1.lmp";
static char text_1[] = "ice_chest_large.vif";
static char text_2[] = "ice_chest_large.tex";
static char text_3[] = "ice_chest_large_open.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c828[5] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {nullptr, 0u},
};
}
