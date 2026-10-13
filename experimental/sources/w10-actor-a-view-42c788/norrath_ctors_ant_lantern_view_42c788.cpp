// Observed .data symbol chest2ResourceMap at 0x42c788, 40 bytes.
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
static char text_0[] = "chest2.lmp";
static char text_1[] = "chest2.vif";
static char text_2[] = "chest2.tex";
static char text_3[] = "chest2.anm";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c788[5] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {nullptr, 0u},
};
}
