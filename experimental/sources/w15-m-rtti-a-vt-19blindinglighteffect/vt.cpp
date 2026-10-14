// Observed _vt$19BlindingLightEffect at 0x00452c60, 144 bytes (18 slots of 8 bytes) in the image data.
// Each slot is a zero adjustment word and a function word. The function words, in slot order, are the
// image symbols named below; a null function word is written as nullptr. The bodies of these functions
// are not in this unit, so every slot refers to the observed name and this unit adds no definitions.
// Inferred: the first slot is the type-info function of the class and the second its destructor, as in
// the other tables of this kind. Unknown: the owner of each named function beyond the name itself.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf19BlindingLightEffect");
void vt_fn_1() asm("_$_19BlindingLightEffect");
void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void vt_fn_3() asm("msg_trigger__10GameObjecti");
void vt_fn_4() asm("msg_run__19BlindingLightEffect");
void vt_fn_5() asm("msg_draw__10GameObject");
void vt_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void vt_fn_7() asm("msg_route__10GameObjecti");
void vt_fn_8() asm("msg_load__10GameObjectPc");
void vt_fn_9() asm("msg_save__10GameObjectPc");
void vt_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void vt_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void vt_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void vt_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void vt_fn_14() asm("msg_poke__10GameObjecti");
void vt_fn_15() asm("clone__10GameObject");
void vt_fn_16() asm("getModel__10GameObject");
VtSlot vt_19BlindingLightEffect[18] asm("_vt$19BlindingLightEffect") = {
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
