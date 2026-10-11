// Observed .data symbol _vt$4Gold at 0x44cd48, 144 bytes.
// Authored virtual calls load an adjustment word and a function pointer 4 bytes later.
// Each slot below is that pair. A zero function word is a null slot, not a file symbol.
// Unknown: dispatch, overrides, and the bodies of the named functions.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf4Gold");
void slot_fn_1() asm("_$_4Gold");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__4Gold");
void slot_fn_5() asm("msg_draw__4Gold");
void slot_fn_6() asm("msg_collision__4GoldP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__4GoldPc");
void slot_fn_9() asm("msg_save__4GoldPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
void slot_fn_14() asm("msg_poke__10GameObjecti");
void slot_fn_15() asm("clone__4Gold");
void slot_fn_16() asm("getModel__10GameObject");
VirtualSlot norrath_player_view_44cd48[18] = {
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
