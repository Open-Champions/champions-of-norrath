// Observed .data symbol _vt$7Shooter at 0x44a420, 144 bytes.
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
void slot_fn_0() asm("__tf7Shooter");
void slot_fn_1() asm("_$_7Shooter");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__7Shooter");
void slot_fn_5() asm("msg_draw__10GameObject");
void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__7ShooterPc");
void slot_fn_9() asm("msg_save__7ShooterPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void slot_fn_14() asm("msg_poke__10GameObjecti");
void slot_fn_15() asm("clone__7Shooter");
void slot_fn_16() asm("getModel__10GameObject");
VirtualSlot norrath_actors_b_view_44a420[18] = {
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
