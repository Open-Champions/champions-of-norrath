// Observed .data symbol _vt$8HateGate at 0x44f0a8, 168 bytes.
// Each slot is the adjustment word followed by a function pointer.
// A zero function word is a null slot. Names are the sized ELF
// function symbols at those addresses. Compiler marker symbols that
// share an address are not the slot. Unknown: dispatch and overrides.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf8HateGate");
void slot_fn_1() asm("_$_8HateGate");
void slot_fn_2() asm("msg_hurt__8AutoPropfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__8AutoPropi");
void slot_fn_4() asm("msg_run__8HateGate");
void slot_fn_5() asm("msg_draw__10AutoObject");
void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__8AutoPropPc");
void slot_fn_9() asm("msg_save__8AutoPropPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__8AutoPropPUsi");
void slot_fn_14() asm("msg_poke__8AutoPropi");
void slot_fn_15() asm("clone__10GameObject");
void slot_fn_16() asm("getModel__10AutoObject");
void slot_fn_17() asm("advanceState__8AutoProp");
void slot_fn_18() asm("setState__8AutoPropi");
void slot_fn_19() asm("setPassableState__8AutoPropb");
VirtualSlot norrath_actors_b_view_44f0a8[21] = {
    {0x0u, slot_fn_0},
    {0x0u, slot_fn_1},
    {0x0u, slot_fn_2},
    {0x0u, slot_fn_3},
    {0x0u, slot_fn_4},
    {0x0u, slot_fn_5},
    {0x0u, slot_fn_6},
    {0x0u, slot_fn_7},
    {0x0u, slot_fn_8},
    {0x0u, slot_fn_9},
    {0x0u, slot_fn_10},
    {0x0u, slot_fn_11},
    {0x0u, slot_fn_12},
    {0x0u, slot_fn_13},
    {0x0u, slot_fn_14},
    {0x0u, slot_fn_15},
    {0x0u, slot_fn_16},
    {0x0u, slot_fn_17},
    {0x0u, slot_fn_18},
    {0x0u, slot_fn_19},
    {0x0u, nullptr},
};
}
