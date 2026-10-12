// Host comparison only. Not an EE build input. Each slot function records
// its own image name; the table must list them in slot order with the
// adjustment words and null slots observed in the image.
#include <cstring>
#include <cstdio>
static const char* hit = "";
extern "C" void slot_fn_0() asm("__tf4Boat");
extern "C" void slot_fn_0() { hit = "__tf4Boat"; }
extern "C" void slot_fn_1() asm("_$_4Boat");
extern "C" void slot_fn_1() { hit = "_$_4Boat"; }
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
extern "C" void slot_fn_28() asm("set__4Boat");
extern "C" void slot_fn_28() { hit = "set__4Boat"; }
extern "C" void slot_fn_29() asm("doMyUniqueForces__4Boat");
extern "C" void slot_fn_29() { hit = "doMyUniqueForces__4Boat"; }
#include "view_453328.cpp"
static const char* const expected[31] = {
    "__tf4Boat",
    "_$_4Boat",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__7Vehicle",
    "msg_draw__7Vehicle",
    "msg_collision__7VehicleP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__10GameObjectPc",
    "msg_save__10GameObjectPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__7VehiclePUsi",
    "msg_poke__7Vehiclei",
    "clone__10GameObject",
    "getModel__10GameObject",
    "GetPlayerPosition__7VehicleiP6Point3Ps",
    "GetNPCPosition__7VehicleP6Point3Ps",
    "GetPositionAndYaw__7VehicleP6Point3T1Ps",
    "eventActivate__7Vehicle",
    "eventDeActivate__7Vehicle",
    "eventGasOn__7Vehicle",
    "eventGasOff__7Vehicle",
    "eventCurRPM__7Vehicle",
    "eventCollisionGround__7Vehicle",
    "eventCollisionWall__7VehicleP6Point3T1",
    "eventSteeringChange__7Vehicle",
    "set__4Boat",
    "doMyUniqueForces__4Boat",
    nullptr,
};
static const unsigned adjustments[31] = {0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_453328 / sizeof norrath_ctors_ant_lantern_view_453328[0] != 31) return 1;
    for (unsigned i = 0; i < 31; ++i) {
        if (norrath_ctors_ant_lantern_view_453328[i].adjustment != adjustments[i]) return 2;
        if (expected[i] == nullptr) {
            if (norrath_ctors_ant_lantern_view_453328[i].function != nullptr) return 3;
            continue;
        }
        if (norrath_ctors_ant_lantern_view_453328[i].function == nullptr) return 4;
        hit = "";
        norrath_ctors_ant_lantern_view_453328[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 5;
    }
    return 0;
}
