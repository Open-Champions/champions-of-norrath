// Observed .data symbol rollDoorResourceMap at 0x42c630, 32 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "rolldoor.lmp";
static char text_1[] = "rolldoor.vif";
static char text_2[] = "rolldoor.tex";
extern "C" {
ResourceSlot norrath_ctors_lava_wraith_view_42c630[4] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {nullptr, 0u},
};
}
