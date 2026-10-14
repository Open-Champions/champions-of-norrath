// Observed .data symbol leverResourceMap at 0x42c650, 32 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "lever.lmp";
static char text_1[] = "lever.vif";
static char text_2[] = "lever.tex";
extern "C" {
ResourceSlot norrath_next_view_42c650[4] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {nullptr, 0u},
};
}
