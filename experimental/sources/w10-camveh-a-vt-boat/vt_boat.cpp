// Observed .data symbol _vt$4Boat at 0x453328, 248 bytes (weak in the image). It is the
// virtual table of Boat: 30 function slots of 8 bytes (an adjustment word, zero in every
// slot, then a function address) followed by one all-zero terminator slot. The function
// names are the sized ELF function symbols at the addresses stored in the image, kept as
// assembler names. Slot 0 is the type-info function __tf4Boat and slot 1 the destructor
// _$_4Boat, by position and name. Slots 2 to 27 are the same addresses as the Vehicle table
// (shared GameObject routines or Vehicle routines), and the last two slots are Boat's own
// set__4Boat and doMyUniqueForces__4Boat. What the slots do is only what their names say.
// None of the slot bodies is defined in this unit.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf4Boat");
void slot_fn_1() asm("_$_4Boat");
void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
void slot_fn_3() asm("msg_trigger__10GameObjecti");
void slot_fn_4() asm("msg_run__7Vehicle");
void slot_fn_5() asm("msg_draw__7Vehicle");
void slot_fn_6() asm("msg_collision__7VehicleP10GameObject");
void slot_fn_7() asm("msg_route__10GameObjecti");
void slot_fn_8() asm("msg_load__10GameObjectPc");
void slot_fn_9() asm("msg_save__10GameObjectPc");
void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__7VehiclePUsi");
void slot_fn_14() asm("msg_poke__7Vehiclei");
void slot_fn_15() asm("clone__10GameObject");
void slot_fn_16() asm("getModel__10GameObject");
void slot_fn_17() asm("GetPlayerPosition__7VehicleiP6Point3Ps");
void slot_fn_18() asm("GetNPCPosition__7VehicleP6Point3Ps");
void slot_fn_19() asm("GetPositionAndYaw__7VehicleP6Point3T1Ps");
void slot_fn_20() asm("eventActivate__7Vehicle");
void slot_fn_21() asm("eventDeActivate__7Vehicle");
void slot_fn_22() asm("eventGasOn__7Vehicle");
void slot_fn_23() asm("eventGasOff__7Vehicle");
void slot_fn_24() asm("eventCurRPM__7Vehicle");
void slot_fn_25() asm("eventCollisionGround__7Vehicle");
void slot_fn_26() asm("eventCollisionWall__7VehicleP6Point3T1");
void slot_fn_27() asm("eventSteeringChange__7Vehicle");
void slot_fn_28() asm("set__4Boat");
void slot_fn_29() asm("doMyUniqueForces__4Boat");
VirtualSlot _vt_4Boat[31] asm("_vt$4Boat") = {
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
    {0x0u, slot_fn_20},
    {0x0u, slot_fn_21},
    {0x0u, slot_fn_22},
    {0x0u, slot_fn_23},
    {0x0u, slot_fn_24},
    {0x0u, slot_fn_25},
    {0x0u, slot_fn_26},
    {0x0u, slot_fn_27},
    {0x0u, slot_fn_28},
    {0x0u, slot_fn_29},
    {0x0u, nullptr},
};
}
