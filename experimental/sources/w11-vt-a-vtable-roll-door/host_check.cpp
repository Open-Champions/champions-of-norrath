// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_roll_door.cpp
// Every slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstddef>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void vt_fn_0() asm("__tf8RollDoor");
extern "C" void vt_fn_0() { hit = "__tf8RollDoor"; }
extern "C" void vt_fn_1() asm("_$_8RollDoor");
extern "C" void vt_fn_1() { hit = "_$_8RollDoor"; }
extern "C" void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void vt_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void vt_fn_3() asm("msg_trigger__8RollDoori");
extern "C" void vt_fn_3() { hit = "msg_trigger__8RollDoori"; }
extern "C" void vt_fn_4() asm("msg_run__8RollDoor");
extern "C" void vt_fn_4() { hit = "msg_run__8RollDoor"; }
extern "C" void vt_fn_5() asm("msg_draw__8RollDoor");
extern "C" void vt_fn_5() { hit = "msg_draw__8RollDoor"; }
extern "C" void vt_fn_6() asm("msg_collision__10GameObjectP10GameObject");
extern "C" void vt_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void vt_fn_7() asm("msg_route__10GameObjecti");
extern "C" void vt_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void vt_fn_8() asm("msg_load__8RollDoorPc");
extern "C" void vt_fn_8() { hit = "msg_load__8RollDoorPc"; }
extern "C" void vt_fn_9() asm("msg_save__8RollDoorPc");
extern "C" void vt_fn_9() { hit = "msg_save__8RollDoorPc"; }
extern "C" void vt_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void vt_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void vt_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void vt_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void vt_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void vt_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void vt_fn_13() asm("msg_getPokeName__8RollDoorPUsi");
extern "C" void vt_fn_13() { hit = "msg_getPokeName__8RollDoorPUsi"; }
extern "C" void vt_fn_14() asm("msg_poke__8RollDoori");
extern "C" void vt_fn_14() { hit = "msg_poke__8RollDoori"; }
extern "C" void vt_fn_15() asm("clone__8RollDoor");
extern "C" void vt_fn_15() { hit = "clone__8RollDoor"; }
extern "C" void vt_fn_16() asm("getModel__10GameObject");
extern "C" void vt_fn_16() { hit = "getModel__10GameObject"; }
extern "C" void vt_fn_17() asm("msg_presave__8RollDoor");
extern "C" void vt_fn_17() { hit = "msg_presave__8RollDoor"; }
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_roll_door[19] asm("_vt$8RollDoor");

int main() {
    static const char* const want[18] = {
    "__tf8RollDoor",
    "_$_8RollDoor",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__8RollDoori",
    "msg_run__8RollDoor",
    "msg_draw__8RollDoor",
    "msg_collision__10GameObjectP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__8RollDoorPc",
    "msg_save__8RollDoorPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__8RollDoorPUsi",
    "msg_poke__8RollDoori",
    "clone__8RollDoor",
    "getModel__10GameObject",
    "msg_presave__8RollDoor",
    };
    if (sizeof(vt_roll_door) != 19 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 18; ++i) {
        if (vt_roll_door[i].adjustment != 0) return 2;
        hit = "";
        vt_roll_door[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_roll_door[18].adjustment != 0 || vt_roll_door[18].function != nullptr) return 4;
    std::printf("ok 8RollDoor 19 slots\n");
    return 0;
}
