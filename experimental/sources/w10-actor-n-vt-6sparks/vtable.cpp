// Observed .data symbol _vt$6Sparks at 0x0044a270, 144 bytes.
// Eighteen 8-byte slots. Every slot's first word is zero (no adjustment) and its second word is a
// function address, except slot 17 whose function word is also zero (an end marker). Slots in
// order, named by the sized ELF function symbols at the stored addresses:
//   0 __tf6Sparks, 1 _$_6Sparks, 2 msg_hurt__10GameObject..., 3 msg_trigger__10GameObjecti,
//   4 msg_run__6Sparks, 5 msg_draw__6Sparks, 6 msg_collision__6SparksP10GameObject,
//   7 msg_route__10GameObjecti, 8 msg_load__10GameObjectPc, 9 msg_save__10GameObjectPc,
//   10 msg_collisionWorld__10GameObjectG6Point3, 11 msg_moveCollision__10GameObjectPP10GameObject,
//   12 msg_alert__10GameObjectifG6Point3P10GameObject, 13 msg_getPokeName__10GameObjectPUsi,
//   14 msg_poke__10GameObjecti, 15 clone__6Sparks, 16 getModel__10GameObject.
// Slots 2, 3 and 7..14 and 16 are the GameObject versions (the class overrides slots 0, 1, 4, 5, 6
// and 15). Unknown: what each slot is called in the source and what an end marker means to the
// dispatcher. Nothing here ran on the EE.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf6Sparks");
void slot_fn_1() asm("_$_6Sparks");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__6Sparks");
void slot_fn_5() asm("msg_draw__6Sparks");
void slot_fn_6() asm("msg_collision__6SparksP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__10GameObjectPc");
void slot_fn_9() asm("msg_save__10GameObjectPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void slot_fn_14() asm("msg_poke__10GameObjecti");
void slot_fn_15() asm("clone__6Sparks");
void slot_fn_16() asm("getModel__10GameObject");

VirtualSlot vt_6Sparks[18] asm("_vt$6Sparks") = {
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
    {0x0u, nullptr},
};
}
