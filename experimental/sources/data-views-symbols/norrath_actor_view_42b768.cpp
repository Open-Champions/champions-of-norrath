// Observed .data symbol mermaidResourceMap at 0x42b768, 128 bytes.
// Each slot is a name pointer followed by a zero word. The last slot is null.
// Inferred: callers walk that 8-byte stride. Unknown: which slot indexes the game uses.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
#if defined(__mips__)
static_assert(sizeof(ResourceSlot) == 8, "resource slot is a pointer plus a zero word");
#endif
static char text_0[] = "mermaid.lmp";
static char text_1[] = "mermaid.vif";
static char text_2[] = "mermaid.tex";
static char text_3[] = "idlerok1.anm";
static char text_4[] = "idlerok2.anm";
static char text_5[] = "idlrok1d2.anm";
static char text_6[] = "rockflip.anm";
static char text_7[] = "sunbath1.anm";
static char text_8[] = "sunbath1d2.anm";
static char text_9[] = "sunbath2.anm";
static char text_10[] = "swim.anm";
static char text_11[] = "swimidle.anm";
static char text_12[] = "swimleft.anm";
static char text_13[] = "swimright.anm";
static char text_14[] = "swimstop.anm";
extern "C" {
ResourceSlot norrath_actor_view_42b768[16] = {
    {text_0, 0u},
    {text_1, 0u},
    {text_2, 0u},
    {text_3, 0u},
    {text_4, 0u},
    {text_5, 0u},
    {text_6, 0u},
    {text_7, 0u},
    {text_8, 0u},
    {text_9, 0u},
    {text_10, 0u},
    {text_11, 0u},
    {text_12, 0u},
    {text_13, 0u},
    {text_14, 0u},
    {nullptr, 0u},
};
}
