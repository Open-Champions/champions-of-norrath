// Host contract for _vt$13MissileWeapon; not an EE input. Every slot function is replaced by a stub that records
// its own symbol name; the table must hold the stubs in order with zero adjustments and end with a null slot.
#include <cstdio>
#include <cstring>
static const char* hit = "";
#include "vt_13missileweapon.cpp"
extern "C" void slot_fn_0() { hit = "__tf13MissileWeapon"; }
extern "C" void slot_fn_1() { hit = "_$_13MissileWeapon"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__13MissileWeapon"; }
extern "C" void slot_fn_5() { hit = "msg_draw__13MissileWeapon"; }
extern "C" void slot_fn_6() { hit = "msg_collision__13MissileWeaponP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void slot_fn_15() { hit = "clone__13MissileWeapon"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }
extern "C" void slot_fn_17() { hit = "findClosestShadowBall__13MissileWeaponP10GameObject"; }
int main() {
    int bad = 0;
    const char* const expected[] = {"__tf13MissileWeapon", "_$_13MissileWeapon", "msg_hurt__10GameObjectfP10GameObjecti", "msg_trigger__10GameObjecti", "msg_run__13MissileWeapon", "msg_draw__13MissileWeapon", "msg_collision__13MissileWeaponP10GameObject", "msg_route__10GameObjecti", "msg_load__10GameObjectPc", "msg_save__10GameObjectPc", "msg_collisionWorld__10GameObjectG6Point3", "msg_moveCollision__10GameObjectPP10GameObject", "msg_alert__10GameObjectifG6Point3P10GameObject", "msg_getPokeName__10GameObjectPUsi", "msg_poke__10GameObjecti", "clone__13MissileWeapon", "getModel__10GameObject", "findClosestShadowBall__13MissileWeaponP10GameObject"};
    const unsigned count = 18, total = 19;
    static_assert(sizeof(vt_13MissileWeapon) == sizeof(VirtualSlot) * 19, "table size");
    for (unsigned i = 0; i < count; ++i) {
        if (vt_13MissileWeapon[i].adjustment != 0u || vt_13MissileWeapon[i].function == nullptr) { std::printf("fail slot %u\n", i); bad = 1; continue; }
        hit = ""; vt_13MissileWeapon[i].function();
        if (std::strcmp(hit, expected[i]) != 0) { std::printf("fail slot %u: %s\n", i, hit); bad = 1; }
    }
    for (unsigned i = count; i < total; ++i) if (vt_13MissileWeapon[i].adjustment != 0u || vt_13MissileWeapon[i].function != nullptr) { std::printf("fail tail %u\n", i); bad = 1; }
    return bad;
}
