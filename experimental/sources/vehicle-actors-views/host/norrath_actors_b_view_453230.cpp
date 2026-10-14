// Host comparison only. Not an EE build input.
const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void slot_fn_0() asm("__tf8SkulBoat");
extern "C" void slot_fn_0() { hit = "__tf8SkulBoat"; }
extern "C" void slot_fn_1() asm("_$_8SkulBoat");
extern "C" void slot_fn_1() { hit = "_$_8SkulBoat"; }
extern "C" void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() asm("msg_run__7Vehicle");
extern "C" void slot_fn_4() { hit = "msg_run__7Vehicle"; }
extern "C" void slot_fn_5() asm("msg_draw__7Vehicle");
extern "C" void slot_fn_5() { hit = "msg_draw__7Vehicle"; }
extern "C" void slot_fn_6() asm("msg_collision__7VehicleP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__7VehicleP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__10GameObjecti");
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() asm("msg_load__10GameObjectPc");
extern "C" void slot_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void slot_fn_9() asm("msg_save__10GameObjectPc");
extern "C" void slot_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__7VehiclePUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__7VehiclePUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__7Vehiclei");
extern "C" void slot_fn_14() { hit = "msg_poke__7Vehiclei"; }
extern "C" void slot_fn_15() asm("clone__10GameObject");
extern "C" void slot_fn_15() { hit = "clone__10GameObject"; }
extern "C" void slot_fn_16() asm("getModel__10GameObject");
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }
extern "C" void slot_fn_17() asm("GetPlayerPosition__7VehicleiP6Point3Ps");
extern "C" void slot_fn_17() { hit = "GetPlayerPosition__7VehicleiP6Point3Ps"; }
extern "C" void slot_fn_18() asm("GetNPCPosition__7VehicleP6Point3Ps");
extern "C" void slot_fn_18() { hit = "GetNPCPosition__7VehicleP6Point3Ps"; }
extern "C" void slot_fn_19() asm("GetPositionAndYaw__7VehicleP6Point3T1Ps");
extern "C" void slot_fn_19() { hit = "GetPositionAndYaw__7VehicleP6Point3T1Ps"; }
extern "C" void slot_fn_20() asm("eventActivate__7Vehicle");
extern "C" void slot_fn_20() { hit = "eventActivate__7Vehicle"; }
extern "C" void slot_fn_21() asm("eventDeActivate__7Vehicle");
extern "C" void slot_fn_21() { hit = "eventDeActivate__7Vehicle"; }
extern "C" void slot_fn_22() asm("eventGasOn__7Vehicle");
extern "C" void slot_fn_22() { hit = "eventGasOn__7Vehicle"; }
extern "C" void slot_fn_23() asm("eventGasOff__7Vehicle");
extern "C" void slot_fn_23() { hit = "eventGasOff__7Vehicle"; }
extern "C" void slot_fn_24() asm("eventCurRPM__7Vehicle");
extern "C" void slot_fn_24() { hit = "eventCurRPM__7Vehicle"; }
extern "C" void slot_fn_25() asm("eventCollisionGround__7Vehicle");
extern "C" void slot_fn_25() { hit = "eventCollisionGround__7Vehicle"; }
extern "C" void slot_fn_26() asm("eventCollisionWall__7VehicleP6Point3T1");
extern "C" void slot_fn_26() { hit = "eventCollisionWall__7VehicleP6Point3T1"; }
extern "C" void slot_fn_27() asm("eventSteeringChange__7Vehicle");
extern "C" void slot_fn_27() { hit = "eventSteeringChange__7Vehicle"; }
extern "C" void slot_fn_28() asm("set__8SkulBoat");
extern "C" void slot_fn_28() { hit = "set__8SkulBoat"; }
extern "C" void slot_fn_29() asm("doMyUniqueForces__8SkulBoat");
extern "C" void slot_fn_29() { hit = "doMyUniqueForces__8SkulBoat"; }
struct VirtualSlot { unsigned adjustment; void (*function)(); };
extern "C" VirtualSlot norrath_actors_b_view_453230[31];
int main() {
    if (norrath_actors_b_view_453230[0].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[0].function();
    if (!same(hit, "__tf8SkulBoat")) return 1;
    if (norrath_actors_b_view_453230[1].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[1].function();
    if (!same(hit, "_$_8SkulBoat")) return 1;
    if (norrath_actors_b_view_453230[2].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[2].function();
    if (!same(hit, "msg_hurt__10GameObjectfP10GameObjecti")) return 1;
    if (norrath_actors_b_view_453230[3].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[3].function();
    if (!same(hit, "msg_trigger__10GameObjecti")) return 1;
    if (norrath_actors_b_view_453230[4].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[4].function();
    if (!same(hit, "msg_run__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[5].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[5].function();
    if (!same(hit, "msg_draw__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[6].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[6].function();
    if (!same(hit, "msg_collision__7VehicleP10GameObject")) return 1;
    if (norrath_actors_b_view_453230[7].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[7].function();
    if (!same(hit, "msg_route__10GameObjecti")) return 1;
    if (norrath_actors_b_view_453230[8].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[8].function();
    if (!same(hit, "msg_load__10GameObjectPc")) return 1;
    if (norrath_actors_b_view_453230[9].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[9].function();
    if (!same(hit, "msg_save__10GameObjectPc")) return 1;
    if (norrath_actors_b_view_453230[10].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[10].function();
    if (!same(hit, "msg_collisionWorld__10GameObjectG6Point3")) return 1;
    if (norrath_actors_b_view_453230[11].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[11].function();
    if (!same(hit, "msg_moveCollision__10GameObjectPP10GameObject")) return 1;
    if (norrath_actors_b_view_453230[12].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[12].function();
    if (!same(hit, "msg_alert__10GameObjectifG6Point3P10GameObject")) return 1;
    if (norrath_actors_b_view_453230[13].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[13].function();
    if (!same(hit, "msg_getPokeName__7VehiclePUsi")) return 1;
    if (norrath_actors_b_view_453230[14].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[14].function();
    if (!same(hit, "msg_poke__7Vehiclei")) return 1;
    if (norrath_actors_b_view_453230[15].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[15].function();
    if (!same(hit, "clone__10GameObject")) return 1;
    if (norrath_actors_b_view_453230[16].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[16].function();
    if (!same(hit, "getModel__10GameObject")) return 1;
    if (norrath_actors_b_view_453230[17].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[17].function();
    if (!same(hit, "GetPlayerPosition__7VehicleiP6Point3Ps")) return 1;
    if (norrath_actors_b_view_453230[18].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[18].function();
    if (!same(hit, "GetNPCPosition__7VehicleP6Point3Ps")) return 1;
    if (norrath_actors_b_view_453230[19].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[19].function();
    if (!same(hit, "GetPositionAndYaw__7VehicleP6Point3T1Ps")) return 1;
    if (norrath_actors_b_view_453230[20].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[20].function();
    if (!same(hit, "eventActivate__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[21].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[21].function();
    if (!same(hit, "eventDeActivate__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[22].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[22].function();
    if (!same(hit, "eventGasOn__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[23].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[23].function();
    if (!same(hit, "eventGasOff__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[24].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[24].function();
    if (!same(hit, "eventCurRPM__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[25].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[25].function();
    if (!same(hit, "eventCollisionGround__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[26].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[26].function();
    if (!same(hit, "eventCollisionWall__7VehicleP6Point3T1")) return 1;
    if (norrath_actors_b_view_453230[27].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[27].function();
    if (!same(hit, "eventSteeringChange__7Vehicle")) return 1;
    if (norrath_actors_b_view_453230[28].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[28].function();
    if (!same(hit, "set__8SkulBoat")) return 1;
    if (norrath_actors_b_view_453230[29].adjustment != 0x0u) return 1;
    hit = "";
    norrath_actors_b_view_453230[29].function();
    if (!same(hit, "doMyUniqueForces__8SkulBoat")) return 1;
    if (norrath_actors_b_view_453230[30].adjustment != 0x0u) return 1;
    if (norrath_actors_b_view_453230[30].function != 0) return 1;
    return 0;
}
