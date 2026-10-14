// Observed .data symbol _vt$8GutChunk at 0x44ad20, 144 bytes (weak in the image). It is the
// virtual table of GutChunk: 17 function slots of 8 bytes (an adjustment word, zero in every slot, then
// a function address) followed by one all-zero terminator slot. The function names are the sized ELF
// function symbols at the addresses in the image, kept as assembler names. Slot 0 holds the class's
// type-info function and slot 1 its destructor by their position and names; what the later slots do is
// only what those names say. A slot may name a shared GameObject routine where the class adds nothing
// of its own. None of the slot bodies is defined in this unit.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf8GutChunk");
void slot_fn_1() asm("_$_8GutChunk");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__8GutChunk");
void slot_fn_5() asm("msg_draw__8GutChunk");
void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__4PropPc");
void slot_fn_9() asm("msg_save__4PropPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__6PhPropPUsi");
void slot_fn_14() asm("msg_poke__6PhPropi");
void slot_fn_15() asm("clone__8GutChunk");
void slot_fn_16() asm("getModel__10GameObject");
VirtualSlot _vt_8GutChunk[18] asm("_vt$8GutChunk") = {
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
