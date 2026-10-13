// Host comparison only. Not an EE build input. Each slot function records
// its own image name; the table must list them in slot order with the
// adjustment words and null slots observed in the image.
#include <cstring>
static const char* hit = "";
extern "C" void slot_fn_0() asm("__tf10AutoPropLP");
extern "C" void slot_fn_0() { hit = "__tf10AutoPropLP"; }
extern "C" void slot_fn_1() asm("_$_10AutoPropLP");
extern "C" void slot_fn_1() { hit = "_$_10AutoPropLP"; }
extern "C" void slot_fn_2() asm("msg_hurt__8AutoPropfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__8AutoPropfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__8AutoPropi");
extern "C" void slot_fn_3() { hit = "msg_trigger__8AutoPropi"; }
extern "C" void slot_fn_4() asm("msg_run__10AutoPropLP");
extern "C" void slot_fn_4() { hit = "msg_run__10AutoPropLP"; }
extern "C" void slot_fn_5() asm("msg_draw__10AutoPropLP");
extern "C" void slot_fn_5() { hit = "msg_draw__10AutoPropLP"; }
extern "C" void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__10GameObjecti");
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() asm("msg_load__8AutoPropPc");
extern "C" void slot_fn_8() { hit = "msg_load__8AutoPropPc"; }
extern "C" void slot_fn_9() asm("msg_save__8AutoPropPc");
extern "C" void slot_fn_9() { hit = "msg_save__8AutoPropPc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__8AutoPropPUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8AutoPropPUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__8AutoPropi");
extern "C" void slot_fn_14() { hit = "msg_poke__8AutoPropi"; }
extern "C" void slot_fn_15() asm("clone__10GameObject");
extern "C" void slot_fn_15() { hit = "clone__10GameObject"; }
extern "C" void slot_fn_16() asm("getModel__10AutoObject");
extern "C" void slot_fn_16() { hit = "getModel__10AutoObject"; }
extern "C" void slot_fn_17() asm("advanceState__8AutoProp");
extern "C" void slot_fn_17() { hit = "advanceState__8AutoProp"; }
extern "C" void slot_fn_18() asm("setState__8AutoPropi");
extern "C" void slot_fn_18() { hit = "setState__8AutoPropi"; }
extern "C" void slot_fn_19() asm("setPassableState__8AutoPropb");
extern "C" void slot_fn_19() { hit = "setPassableState__8AutoPropb"; }
#include "../view_44e9d0.cpp"
static const char* const expected[21] = {
    "__tf10AutoPropLP",
    "_$_10AutoPropLP",
    "msg_hurt__8AutoPropfP10GameObjecti",
    "msg_trigger__8AutoPropi",
    "msg_run__10AutoPropLP",
    "msg_draw__10AutoPropLP",
    "msg_collision__10GameObjectP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__8AutoPropPc",
    "msg_save__8AutoPropPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__8AutoPropPUsi",
    "msg_poke__8AutoPropi",
    "clone__10GameObject",
    "getModel__10AutoObject",
    "advanceState__8AutoProp",
    "setState__8AutoPropi",
    "setPassableState__8AutoPropb",
    nullptr,
};
static const unsigned adjustments[21] = {0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_44e9d0 / sizeof norrath_ctors_ant_lantern_view_44e9d0[0] != 21) return 1;
    for (unsigned i = 0; i < 21; ++i) {
        if (norrath_ctors_ant_lantern_view_44e9d0[i].adjustment != adjustments[i]) return 2;
        if (expected[i] == nullptr) {
            if (norrath_ctors_ant_lantern_view_44e9d0[i].function != nullptr) return 3;
            continue;
        }
        if (norrath_ctors_ant_lantern_view_44e9d0[i].function == nullptr) return 4;
        hit = "";
        norrath_ctors_ant_lantern_view_44e9d0[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 5;
    }
    return 0;
}
