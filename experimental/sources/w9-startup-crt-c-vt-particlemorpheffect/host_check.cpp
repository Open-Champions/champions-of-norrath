// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null terminator observed in the image.
#include "vt_particlemorpheffect.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf19ParticleMorphEffect"; }
extern "C" void slot_fn_1() { hit = "_$_19ParticleMorphEffect"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__19ParticleMorphEffect"; }
extern "C" void slot_fn_5() { hit = "msg_draw__10GameObject"; }
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__10GameObjectPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__10GameObjectPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void slot_fn_15() { hit = "clone__10GameObject"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }

static const char* const expected[17] = {
    "__tf19ParticleMorphEffect",
    "_$_19ParticleMorphEffect",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__19ParticleMorphEffect",
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
    "clone__10GameObject",
    "getModel__10GameObject",
};

int main() {
    static_assert(sizeof(_vt_19ParticleMorphEffect) == 18 * sizeof(VirtualSlot), "slot count");
    for (int i = 0; i < 17; ++i) {
        if (_vt_19ParticleMorphEffect[i].adjustment != 0) return 1;
        hit = "";
        _vt_19ParticleMorphEffect[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 2;
    }
    if (_vt_19ParticleMorphEffect[17].adjustment != 0 || _vt_19ParticleMorphEffect[17].function != nullptr) return 3;
    return 0;
}
