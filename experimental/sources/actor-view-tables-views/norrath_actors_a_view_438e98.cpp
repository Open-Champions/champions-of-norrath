// Observed .data symbol bombResMap at 0x438e98, 32 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// FireBomb's constructor passes this address as the resource map.
// Unknown: which slot indexes the game uses. Pointer identity with other
// copies of these names is not preserved.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "firebomb.lmp";
static char text_1[] = "firebomb.vif";
static char text_2[] = "firebomb.tex";
extern "C" {
ResourceSlot norrath_actors_a_view_438e98[4] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {nullptr, 0u},
};
}
