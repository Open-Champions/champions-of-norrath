// Observed unnamed .rodata run at 0x471030, 288 bytes: 18 text slots of 16 bytes, each a string followed by zero padding.
// AutoContainer reads slot i at 0x471030 + 16*i and compares the object name against it (case-insensitive).
// Slot 18 would be the string at 0x471150, which is a separate view.
extern "C" const char norrath_ctors_ant_lantern_view_471030[18][16];
const char norrath_ctors_ant_lantern_view_471030[18][16] = {
    "weaprack",
    "smalcrat",
    "baskcold",
    "greenpot",
    "kelbask1",
    "kelbask2",
    "gothpic1",
    "gothpic2",
    "gothpic3",
    "gothpic4",
    "gothpic5",
    "gothpic6",
    "crispy01",
    "crispy02",
    "crispy03",
    "gothpot1",
    "gothpot2",
    "ap_barrel"};
static_assert(sizeof norrath_ctors_ant_lantern_view_471030 == 288, "18 slots of 16 bytes");
