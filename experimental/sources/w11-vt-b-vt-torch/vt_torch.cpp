// Observed .data symbol _vt$5Torch at 0x44ab70, 144 bytes (weak in the image). It is a table of 18 slots of 8 bytes:
// an adjustment word (zero in every slot) and a function address. Slot 17 is null. The function names are the sized ELF
// function symbols at the addresses stored in the image, kept as assembler names. Slots whose
// name carries the class name Torch are 0, 1, 4, 5, 6, 8, 9, 15; the other slots name routines of
// other classes. What the slots do is only what their names say; none of the slot bodies is
// defined in this unit. Unknown: how the table is selected and called.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf5Torch");
void slot_fn_1() asm("_$_5Torch");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__5Torch");
void slot_fn_5() asm("msg_draw__5Torch");
void slot_fn_6() asm("msg_collision__5TorchP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__5TorchPc");
void slot_fn_9() asm("msg_save__5TorchPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void slot_fn_14() asm("msg_poke__10GameObjecti");
void slot_fn_15() asm("clone__5Torch");
void slot_fn_16() asm("getModel__10GameObject");
VirtualSlot _vt_5Torch[18] asm("_vt$5Torch") = {
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
