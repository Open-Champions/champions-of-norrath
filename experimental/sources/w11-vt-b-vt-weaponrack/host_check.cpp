// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null slots observed in the image.
#include "vt_weaponrack.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf10WeaponRack"; }
extern "C" void slot_fn_1() { hit = "_$_10WeaponRack"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__10GameObject"; }
extern "C" void slot_fn_5() { hit = "msg_draw__10WeaponRack"; }
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10WeaponRackPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__10WeaponRacki"; }
extern "C" void slot_fn_15() { hit = "clone__10WeaponRack"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }

static const char* const expected[18] = {
    "__tf10WeaponRack",
    "_$_10WeaponRack",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__10GameObject",
    "msg_draw__10WeaponRack",
    "msg_collision__10GameObjectP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__10GameObjectPc",
    "msg_save__10GameObjectPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__10WeaponRackPUsi",
    "msg_poke__10WeaponRacki",
    "clone__10WeaponRack",
    "getModel__10GameObject",
    nullptr,
};

int main() {
    if (sizeof(_vt_10WeaponRack) / sizeof(_vt_10WeaponRack[0]) != 18) return 1;
    for (int i = 0; i < 18; ++i) {
        if (_vt_10WeaponRack[i].adjustment != 0) return 2;
        if (expected[i] == nullptr) {
            if (_vt_10WeaponRack[i].function != nullptr) return 4;
            continue;
        }
        hit = "";
        _vt_10WeaponRack[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 3;
    }
    return 0;
}
