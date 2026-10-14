// Host comparison only. Not an EE build input. Links against
// prop_vtable.cpp compiled for the host.
#include <cstdio>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void prop_fn_0() asm("__tf4Prop");
extern "C" void prop_fn_0() { hit = "__tf4Prop"; }
extern "C" void prop_fn_1() asm("_$_4Prop");
extern "C" void prop_fn_1() { hit = "_$_4Prop"; }
extern "C" void prop_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void prop_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void prop_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void prop_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void prop_fn_4() asm("msg_run__4Prop");
extern "C" void prop_fn_4() { hit = "msg_run__4Prop"; }
extern "C" void prop_fn_5() asm("msg_draw__4Prop");
extern "C" void prop_fn_5() { hit = "msg_draw__4Prop"; }
extern "C" void prop_fn_6() asm("msg_collision__10GameObjectP10GameObject");
extern "C" void prop_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void prop_fn_7() asm("msg_route__10GameObjecti");
extern "C" void prop_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void prop_fn_8() asm("msg_load__4PropPc");
extern "C" void prop_fn_8() { hit = "msg_load__4PropPc"; }
extern "C" void prop_fn_9() asm("msg_save__4PropPc");
extern "C" void prop_fn_9() { hit = "msg_save__4PropPc"; }
extern "C" void prop_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void prop_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void prop_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void prop_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void prop_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void prop_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void prop_fn_13() asm("msg_getPokeName__10GameObjectPUsi");
extern "C" void prop_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void prop_fn_14() asm("msg_poke__10GameObjecti");
extern "C" void prop_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void prop_fn_15() asm("clone__4Prop");
extern "C" void prop_fn_15() { hit = "clone__4Prop"; }
extern "C" void prop_fn_16() asm("getModel__10GameObject");
extern "C" void prop_fn_16() { hit = "getModel__10GameObject"; }
struct PropSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" PropSlot norrath_factory_prop_vtable[18];
int main() {
    const char* want[] = {
        "__tf4Prop", "_$_4Prop", "msg_hurt__10GameObjectfP10GameObjecti", "msg_trigger__10GameObjecti",
        "msg_run__4Prop", "msg_draw__4Prop", "msg_collision__10GameObjectP10GameObject",
        "msg_route__10GameObjecti", "msg_load__4PropPc", "msg_save__4PropPc",
        "msg_collisionWorld__10GameObjectG6Point3", "msg_moveCollision__10GameObjectPP10GameObject",
        "msg_alert__10GameObjectifG6Point3P10GameObject", "msg_getPokeName__10GameObjectPUsi",
        "msg_poke__10GameObjecti", "clone__4Prop", "getModel__10GameObject",
    };
    for (int slot = 0; slot < 17; ++slot) {
        if (norrath_factory_prop_vtable[slot].adjustment != 0x0u) return 1;
        hit = "";
        norrath_factory_prop_vtable[slot].function();
        if (!same(hit, want[slot])) {
            std::printf("slot %d hit %s want %s\n", slot, hit, want[slot]);
            return 2;
        }
    }
    if (norrath_factory_prop_vtable[17].adjustment != 0x0u ||
        norrath_factory_prop_vtable[17].function != nullptr)
        return 3;
    return 0;
}
