// Observed _vt$4Prop at 0x44d018, 144 bytes in .data. Eighteen slots;
// each slot is a zero adjustment word plus a function word. The function
// words, in order, are __tf4Prop, _$_4Prop, the GameObject hurt, trigger,
// run, draw, collision, route, load, save, collisionWorld, moveCollision,
// alert, getPokeName and poke methods (Prop overrides run, draw, load and
// save; the rest are the shared GameObject leaves), then clone__4Prop,
// getModel__10GameObject, and a null slot. The bodies behind those words
// are not in this package, so the slots reference the observed names; all
// seventeen are already unresolved imports of the partial link, and this
// unit opens none.
// Inferred: it is norrath_factory_prop_vtable, whose address the object
// factory stores at object+72 for Prop records. The items-player package
// already defines the same slot map as norrath_player_view_44d018; the
// lead decides whether the two names should become one symbol.
// Unknown: dispatch through this table and the bodies of the slot
// functions.
struct PropSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(PropSlot) == 8, "prop slot is an adjustment word plus a pointer");
#endif
extern "C" {
void prop_fn_0() asm("__tf4Prop");
void prop_fn_1() asm("_$_4Prop");
void prop_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void prop_fn_3() asm("msg_trigger__10GameObjecti");
void prop_fn_4() asm("msg_run__4Prop");
void prop_fn_5() asm("msg_draw__4Prop");
void prop_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void prop_fn_7() asm("msg_route__10GameObjecti");
void prop_fn_8() asm("msg_load__4PropPc");
void prop_fn_9() asm("msg_save__4PropPc");
void prop_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void prop_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void prop_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void prop_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void prop_fn_14() asm("msg_poke__10GameObjecti");
void prop_fn_15() asm("clone__4Prop");
void prop_fn_16() asm("getModel__10GameObject");
PropSlot norrath_factory_prop_vtable[18] = {
    {0x0u, prop_fn_0},
    {0x0u, prop_fn_1},
    {0x0u, prop_fn_2},
    {0x0u, prop_fn_3},
    {0x0u, prop_fn_4},
    {0x0u, prop_fn_5},
    {0x0u, prop_fn_6},
    {0x0u, prop_fn_7},
    {0x0u, prop_fn_8},
    {0x0u, prop_fn_9},
    {0x0u, prop_fn_10},
    {0x0u, prop_fn_11},
    {0x0u, prop_fn_12},
    {0x0u, prop_fn_13},
    {0x0u, prop_fn_14},
    {0x0u, prop_fn_15},
    {0x0u, prop_fn_16},
    {0x0u, nullptr},
};
}
