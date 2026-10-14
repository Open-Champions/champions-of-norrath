// Host contract for _vt$8HolyBolt; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_8HolyBolt[18] asm("_vt$8HolyBolt");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf8HolyBolt");
extern "C" void vt_fn_0() { hit = "__tf8HolyBolt"; }
extern "C" void vt_fn_1() asm("_$_8HolyBolt");
extern "C" void vt_fn_1() { hit = "_$_8HolyBolt"; }
extern "C" void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void vt_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void vt_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void vt_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void vt_fn_4() asm("msg_run__8HolyBolt");
extern "C" void vt_fn_4() { hit = "msg_run__8HolyBolt"; }
extern "C" void vt_fn_5() asm("msg_draw__8HolyBolt");
extern "C" void vt_fn_5() { hit = "msg_draw__8HolyBolt"; }
extern "C" void vt_fn_6() asm("msg_collision__8HolyBoltP10GameObject");
extern "C" void vt_fn_6() { hit = "msg_collision__8HolyBoltP10GameObject"; }
extern "C" void vt_fn_7() asm("msg_route__10GameObjecti");
extern "C" void vt_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void vt_fn_8() asm("msg_load__10GameObjectPc");
extern "C" void vt_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void vt_fn_9() asm("msg_save__10GameObjectPc");
extern "C" void vt_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void vt_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void vt_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void vt_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void vt_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void vt_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void vt_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void vt_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
extern "C" void vt_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void vt_fn_14() asm("msg_poke__10GameObjecti");
extern "C" void vt_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void vt_fn_15() asm("clone__8HolyBolt");
extern "C" void vt_fn_15() { hit = "clone__8HolyBolt"; }
extern "C" void vt_fn_16() asm("getModel__10GameObject");
extern "C" void vt_fn_16() { hit = "getModel__10GameObject"; }
int main() {
    const char* want[] = {
        "__tf8HolyBolt",
        "_$_8HolyBolt",
        "msg_hurt__10GameObjectfP10GameObjecti",
        "msg_trigger__10GameObjecti",
        "msg_run__8HolyBolt",
        "msg_draw__8HolyBolt",
        "msg_collision__8HolyBoltP10GameObject",
        "msg_route__10GameObjecti",
        "msg_load__10GameObjectPc",
        "msg_save__10GameObjectPc",
        "msg_collisionWorld__10GameObjectG6Point3",
        "msg_moveCollision__10GameObjectPP10GameObject",
        "msg_alert__10GameObjectifG6Point3P10GameObject",
        "msg_getPokeName__10GameObjectPUsi",
        "msg_poke__10GameObjecti",
        "clone__8HolyBolt",
        "getModel__10GameObject",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_8HolyBolt / sizeof vt_8HolyBolt[0] != 18) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 18; ++i) {
        if (vt_8HolyBolt[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_8HolyBolt[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_8HolyBolt[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$8HolyBolt slots ok\n");
    return bad != 0;
}
