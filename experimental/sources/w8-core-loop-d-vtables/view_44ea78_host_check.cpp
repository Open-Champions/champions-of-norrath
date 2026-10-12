// Host comparison only. Not an EE build input. Each slot function records
// its own image name; the table must list them in slot order with the
// adjustment words and null slots observed in the image.
#include <cstring>
#include <cstdio>
static const char* hit = "";
extern "C" void slot_fn_0() asm("__tf11AutoChestLP");
extern "C" void slot_fn_0() { hit = "__tf11AutoChestLP"; }
extern "C" void slot_fn_1() asm("_$_11AutoChestLP");
extern "C" void slot_fn_1() { hit = "_$_11AutoChestLP"; }
extern "C" void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() asm("msg_run__11AutoChestLP");
extern "C" void slot_fn_4() { hit = "msg_run__11AutoChestLP"; }
extern "C" void slot_fn_5() asm("msg_draw__11AutoChestLP");
extern "C" void slot_fn_5() { hit = "msg_draw__11AutoChestLP"; }
extern "C" void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__10GameObjecti");
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() asm("msg_load__9AutoChestPc");
extern "C" void slot_fn_8() { hit = "msg_load__9AutoChestPc"; }
extern "C" void slot_fn_9() asm("msg_save__9AutoChestPc");
extern "C" void slot_fn_9() { hit = "msg_save__9AutoChestPc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__9AutoChestPUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__9AutoChestPUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__11AutoChestLPi");
extern "C" void slot_fn_14() { hit = "msg_poke__11AutoChestLPi"; }
extern "C" void slot_fn_15() asm("clone__10GameObject");
extern "C" void slot_fn_15() { hit = "clone__10GameObject"; }
extern "C" void slot_fn_16() asm("getModel__10AutoObject");
extern "C" void slot_fn_16() { hit = "getModel__10AutoObject"; }
#include "view_44ea78.cpp"
static const char* const expected[18] = {
    "__tf11AutoChestLP",
    "_$_11AutoChestLP",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__11AutoChestLP",
    "msg_draw__11AutoChestLP",
    "msg_collision__10GameObjectP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__9AutoChestPc",
    "msg_save__9AutoChestPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__9AutoChestPUsi",
    "msg_poke__11AutoChestLPi",
    "clone__10GameObject",
    "getModel__10AutoObject",
    nullptr,
};
static const unsigned adjustments[18] = {0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_44ea78 / sizeof norrath_ctors_ant_lantern_view_44ea78[0] != 18) return 1;
    for (unsigned i = 0; i < 18; ++i) {
        if (norrath_ctors_ant_lantern_view_44ea78[i].adjustment != adjustments[i]) return 2;
        if (expected[i] == nullptr) {
            if (norrath_ctors_ant_lantern_view_44ea78[i].function != nullptr) return 3;
            continue;
        }
        if (norrath_ctors_ant_lantern_view_44ea78[i].function == nullptr) return 4;
        hit = "";
        norrath_ctors_ant_lantern_view_44ea78[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 5;
    }
    return 0;
}
