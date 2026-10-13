// Observed .data symbol chestResourceMap at 0x42c760, 40 bytes.
// Five slots, each a name pointer followed by a zero word; the last slot is null.
// Constructors pass this address as the resource map. Pointer identity with other copies of
// these names is not preserved. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "chest.lmp";
static char text_1[] = "chest_large.vif";
static char text_2[] = "chest_large.tex";
static char text_3[] = "chest_large_open.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c760[5] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {nullptr, 0u},
};
}
