// Observed _vt$5PhIce at 0x44cdd8, 144 bytes in .data (weak object in the image).
// 18 slots of 8 bytes: a zero adjustment word and a function word, the last
// function word being zero. The function words, in order, are the symbols
// listed in the slot table of docs/agents/w11-vt-a.md (17 distinct names).
// Each name is the sized function symbol at that address in the image; the
// bodies behind them are not in this unit, so every slot refers to the
// observed name and this unit adds no function definitions of its own.
// Inferred: the constructor of PhIce stores this table's address at object+72.
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
void vt_fn_0() asm("__tf5PhIce");
void vt_fn_1() asm("_$_5PhIce");
void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void vt_fn_3() asm("msg_trigger__10GameObjecti");
void vt_fn_4() asm("msg_run__5PhIce");
void vt_fn_5() asm("msg_draw__6PhProp");
void vt_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void vt_fn_7() asm("msg_route__10GameObjecti");
void vt_fn_8() asm("msg_load__4PropPc");
void vt_fn_9() asm("msg_save__4PropPc");
void vt_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void vt_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void vt_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void vt_fn_13() asm("msg_getPokeName__6PhPropPUsi");
void vt_fn_14() asm("msg_poke__6PhPropi");
void vt_fn_15() asm("clone__5PhIce");
void vt_fn_16() asm("getModel__10GameObject");
VtSlot vt_ph_ice[18] asm("_vt$5PhIce") = {
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
    {0x0u, nullptr},
};
}
#if defined(__mips__)
static_assert(sizeof(vt_ph_ice) == 144, "table size is 144 bytes");
#endif
