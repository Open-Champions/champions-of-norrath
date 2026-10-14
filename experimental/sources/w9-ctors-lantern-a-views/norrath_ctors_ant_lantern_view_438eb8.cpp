// Observed .data symbol catapultResourceMap at 0x438eb8, 32 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Constructors pass this address as the resource map. Unknown: which slot
// indexes the game uses. Pointer identity with other copies of these names
// is not preserved.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "catapult.lmp";
static char text_1[] = "catapult.vif";
static char text_2[] = "catapult.tex";
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_438eb8[4] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {nullptr, 0u},
};
}
