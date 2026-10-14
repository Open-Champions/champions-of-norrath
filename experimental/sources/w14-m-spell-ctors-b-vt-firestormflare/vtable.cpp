// Observed _vt$14FireStormFlare at 0x00451190, 144 bytes in .data (weak object in the image).
// 18 slots of 8 bytes: a zero adjustment word and a function word. The last slot is null.
// The 17 function words, in slot order, are the observed symbol names listed below; each
// name is the image symbol at that address. The bodies are not in this unit.
// Inferred: a constructor of this class stores this table address in the object (another unit).
// Unknown: dispatch order beyond the table itself, and the role of the null last slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_firestormflare_fn_0() asm("__tf14FireStormFlare");
void vt_firestormflare_fn_1() asm("_$_14FireStormFlare");
void vt_firestormflare_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void vt_firestormflare_fn_3() asm("msg_trigger__10GameObjecti");
void vt_firestormflare_fn_4() asm("msg_run__14FireStormFlare");
void vt_firestormflare_fn_5() asm("msg_draw__14FireStormFlare");
void vt_firestormflare_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void vt_firestormflare_fn_7() asm("msg_route__10GameObjecti");
void vt_firestormflare_fn_8() asm("msg_load__10GameObjectPc");
void vt_firestormflare_fn_9() asm("msg_save__10GameObjectPc");
void vt_firestormflare_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void vt_firestormflare_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void vt_firestormflare_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void vt_firestormflare_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void vt_firestormflare_fn_14() asm("msg_poke__10GameObjecti");
void vt_firestormflare_fn_15() asm("clone__10GameObject");
void vt_firestormflare_fn_16() asm("getModel__10GameObject");
VtSlot vt_firestormflare[18] asm("_vt$14FireStormFlare") = {
    {0x0u, vt_firestormflare_fn_0},
    {0x0u, vt_firestormflare_fn_1},
    {0x0u, vt_firestormflare_fn_2},
    {0x0u, vt_firestormflare_fn_3},
    {0x0u, vt_firestormflare_fn_4},
    {0x0u, vt_firestormflare_fn_5},
    {0x0u, vt_firestormflare_fn_6},
    {0x0u, vt_firestormflare_fn_7},
    {0x0u, vt_firestormflare_fn_8},
    {0x0u, vt_firestormflare_fn_9},
    {0x0u, vt_firestormflare_fn_10},
    {0x0u, vt_firestormflare_fn_11},
    {0x0u, vt_firestormflare_fn_12},
    {0x0u, vt_firestormflare_fn_13},
    {0x0u, vt_firestormflare_fn_14},
    {0x0u, vt_firestormflare_fn_15},
    {0x0u, vt_firestormflare_fn_16},
    {0x0u, nullptr},
};
}
