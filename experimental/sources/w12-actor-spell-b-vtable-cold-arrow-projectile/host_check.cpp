// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_cold_arrow_projectile.cpp
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
extern "C" void vt_fn_0() asm("__tf19ColdArrowProjectile");
extern "C" void vt_fn_0() { hit = "__tf19ColdArrowProjectile"; }
extern "C" void vt_fn_1() asm("_$_19ColdArrowProjectile");
extern "C" void vt_fn_1() { hit = "_$_19ColdArrowProjectile"; }
extern "C" void vt_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void vt_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void vt_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void vt_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void vt_fn_4() asm("msg_run__19ColdArrowProjectile");
extern "C" void vt_fn_4() { hit = "msg_run__19ColdArrowProjectile"; }
extern "C" void vt_fn_5() asm("msg_draw__19ColdArrowProjectile");
extern "C" void vt_fn_5() { hit = "msg_draw__19ColdArrowProjectile"; }
extern "C" void vt_fn_6() asm("msg_collision__19ColdArrowProjectileP10GameObject");
extern "C" void vt_fn_6() { hit = "msg_collision__19ColdArrowProjectileP10GameObject"; }
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
extern "C" void vt_fn_15() asm("clone__10Projectile");
extern "C" void vt_fn_15() { hit = "clone__10Projectile"; }
extern "C" void vt_fn_16() asm("getModel__10GameObject");
extern "C" void vt_fn_16() { hit = "getModel__10GameObject"; }
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_cold_arrow_projectile[18] asm("_vt$19ColdArrowProjectile");

int main() {
    static const char* const want[17] = {
    "__tf19ColdArrowProjectile",
    "_$_19ColdArrowProjectile",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__19ColdArrowProjectile",
    "msg_draw__19ColdArrowProjectile",
    "msg_collision__19ColdArrowProjectileP10GameObject",
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
    };
    if (sizeof(vt_cold_arrow_projectile) != 18 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 17; ++i) {
        if (vt_cold_arrow_projectile[i].adjustment != 0) return 2;
        hit = "";
        vt_cold_arrow_projectile[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_cold_arrow_projectile[17].adjustment != 0 || vt_cold_arrow_projectile[17].function != nullptr) return 4;
    std::printf("ok 19ColdArrowProjectile 18 slots\n");
    return 0;
}
