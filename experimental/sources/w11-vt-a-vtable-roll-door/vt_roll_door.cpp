// Observed _vt$8RollDoor at 0x449940, 152 bytes in .data (weak object in the image).
// 19 slots of 8 bytes: a zero adjustment word and a function word, the last
// function word being zero. The function words, in order, are the symbols
// listed in the slot table of docs/agents/w11-vt-a.md (18 distinct names).
// Each name is the sized function symbol at that address in the image; the
// bodies behind them are not in this unit, so every slot refers to the
// observed name and this unit adds no function definitions of its own.
// Inferred: the constructor of RollDoor stores this table's address at object+72.
// That store is made by a different unit and is not exercised here.
// Unknown: dispatch through this table, and the role of the last (null) slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf8RollDoor");
void vt_fn_1() asm("_$_8RollDoor");
void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void vt_fn_3() asm("msg_trigger__8RollDoori");
void vt_fn_4() asm("msg_run__8RollDoor");
void vt_fn_5() asm("msg_draw__8RollDoor");
void vt_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void vt_fn_7() asm("msg_route__10GameObjecti");
void vt_fn_8() asm("msg_load__8RollDoorPc");
void vt_fn_9() asm("msg_save__8RollDoorPc");
void vt_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void vt_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void vt_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void vt_fn_13() asm("msg_getPokeName__8RollDoorPUsi");
void vt_fn_14() asm("msg_poke__8RollDoori");
void vt_fn_15() asm("clone__8RollDoor");
void vt_fn_16() asm("getModel__10GameObject");
void vt_fn_17() asm("msg_presave__8RollDoor");
VtSlot vt_roll_door[19] asm("_vt$8RollDoor") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, vt_fn_5},
    {0x0u, vt_fn_6},
    {0x0u, vt_fn_7},
    {0x0u, vt_fn_8},
    {0x0u, vt_fn_9},
    {0x0u, vt_fn_10},
    {0x0u, vt_fn_11},
    {0x0u, vt_fn_12},
    {0x0u, vt_fn_13},
    {0x0u, vt_fn_14},
    {0x0u, vt_fn_15},
    {0x0u, vt_fn_16},
    {0x0u, vt_fn_17},
    {0x0u, nullptr},
};
}
#if defined(__mips__)
static_assert(sizeof(vt_roll_door) == 152, "table size is 152 bytes");
#endif
