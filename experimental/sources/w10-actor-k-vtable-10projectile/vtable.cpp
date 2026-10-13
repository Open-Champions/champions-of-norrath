// Observed _vt$10Projectile at 0x44d378, 144 bytes in .data. 18 slots; each slot is a zero adjustment word plus a
// function word. The function words, in order, are:
// 0 __tf10Projectile; 1 _$_10Projectile; 2 msg_hurt__10GameObjectfP10GameObjecti;
// 3 msg_trigger__10GameObjecti; 4 msg_run__10GameObject; 5 msg_draw__10GameObject;
// 6 msg_collision__10GameObjectP10GameObject; 7 msg_route__10GameObjecti;
// 8 msg_load__10GameObjectPc; 9 msg_save__10GameObjectPc;
// 10 msg_collisionWorld__10GameObjectG6Point3; 11 msg_moveCollision__10GameObjectPP10GameObject;
// 12 msg_alert__10GameObjectifG6Point3P10GameObject; 13 msg_getPokeName__10GameObjectPUsi;
// 14 msg_poke__10GameObjecti; 15 clone__10Projectile; 16 getModel__10GameObject; 17 null.
// The bodies behind those words are not defined here; the slots reference the observed names,
// so they are imports of this unit.
// Nothing here ran on the EE; dispatch through this table is not exercised.
struct Slot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(Slot) == 8, "a slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() __asm__("__tf10Projectile");
void slot_fn_1() __asm__("_$_10Projectile");
void slot_fn_2() __asm__("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() __asm__("msg_trigger__10GameObjecti");
void slot_fn_4() __asm__("msg_run__10GameObject");
void slot_fn_5() __asm__("msg_draw__10GameObject");
void slot_fn_6() __asm__("msg_collision__10GameObjectP10GameObject");
void slot_fn_7() __asm__("msg_route__10GameObjecti");
void slot_fn_8() __asm__("msg_load__10GameObjectPc");
void slot_fn_9() __asm__("msg_save__10GameObjectPc");
void slot_fn_10() __asm__("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() __asm__("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() __asm__("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() __asm__("msg_getPokeName__10GameObjectPUsi");
void slot_fn_14() __asm__("msg_poke__10GameObjecti");
void slot_fn_15() __asm__("clone__10Projectile");
void slot_fn_16() __asm__("getModel__10GameObject");
Slot vtable[18] __asm__("_vt$10Projectile") = {
    {0u, slot_fn_0},
    {0u, slot_fn_1},
    {0u, slot_fn_2},
    {0u, slot_fn_3},
    {0u, slot_fn_4},
    {0u, slot_fn_5},
    {0u, slot_fn_6},
    {0u, slot_fn_7},
    {0u, slot_fn_8},
    {0u, slot_fn_9},
    {0u, slot_fn_10},
    {0u, slot_fn_11},
    {0u, slot_fn_12},
    {0u, slot_fn_13},
    {0u, slot_fn_14},
    {0u, slot_fn_15},
    {0u, slot_fn_16},
    {0u, nullptr},
};
}
