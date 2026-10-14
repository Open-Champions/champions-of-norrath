// Host comparison only; not an EE build input. Compiles vtable.cpp for the host with a stub for each
// slot function and checks every slot's adjustment and function word, and the table length.
#include <cstdio>
const char* hit = "";
extern "C" void slot_fn_0() __asm__("__tf10Projectile");
extern "C" void slot_fn_0() { hit = "__tf10Projectile"; }
extern "C" void slot_fn_1() __asm__("_$_10Projectile");
extern "C" void slot_fn_1() { hit = "_$_10Projectile"; }
extern "C" void slot_fn_2() __asm__("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() __asm__("msg_trigger__10GameObjecti");
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() __asm__("msg_run__10GameObject");
extern "C" void slot_fn_4() { hit = "msg_run__10GameObject"; }
extern "C" void slot_fn_5() __asm__("msg_draw__10GameObject");
extern "C" void slot_fn_5() { hit = "msg_draw__10GameObject"; }
extern "C" void slot_fn_6() __asm__("msg_collision__10GameObjectP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() __asm__("msg_route__10GameObjecti");
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() __asm__("msg_load__10GameObjectPc");
extern "C" void slot_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void slot_fn_9() __asm__("msg_save__10GameObjectPc");
extern "C" void slot_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void slot_fn_10() __asm__("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() __asm__("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() __asm__("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() __asm__("msg_getPokeName__10GameObjectPUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void slot_fn_14() __asm__("msg_poke__10GameObjecti");
extern "C" void slot_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void slot_fn_15() __asm__("clone__10Projectile");
extern "C" void slot_fn_15() { hit = "clone__10Projectile"; }
extern "C" void slot_fn_16() __asm__("getModel__10GameObject");
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }
struct Slot { unsigned adjustment; void (*function)(); };
extern "C" Slot vtable[18] __asm__("_vt$10Projectile");
int main() {
    const char* want[] = {
        "__tf10Projectile",
        "_$_10Projectile",
        "msg_hurt__10GameObjectfP10GameObjecti",
        "msg_trigger__10GameObjecti",
        "msg_run__10GameObject",
        "msg_draw__10GameObject",
        "msg_collision__10GameObjectP10GameObject",
        "msg_route__10GameObjecti",
        "msg_load__10GameObjectPc",
        "msg_save__10GameObjectPc",
        "msg_collisionWorld__10GameObjectG6Point3",
        "msg_moveCollision__10GameObjectPP10GameObject",
        "msg_alert__10GameObjectifG6Point3P10GameObject",
        "msg_getPokeName__10GameObjectPUsi",
        "msg_poke__10GameObjecti",
        "clone__10Projectile",
        "getModel__10GameObject",
        nullptr,
    };
    int bad = 0;
    for (int i = 0; i < 18; ++i) {
        if (vtable[i].adjustment != 0) bad = 1;
        if (!want[i]) { if (vtable[i].function) bad = 1; continue; }
        hit = "";
        if (!vtable[i].function) { bad = 1; continue; }
        vtable[i].function();
        const char* a = hit; const char* b = want[i];
        while (*a && *a == *b) { ++a; ++b; }
        if (*a != *b) { std::printf("slot %d: %s\n", i, hit); bad = 1; }
    }
    return bad;
}
