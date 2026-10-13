// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null slots observed in the image.
#include "vt_torch.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf5Torch"; }
extern "C" void slot_fn_1() { hit = "_$_5Torch"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__5Torch"; }
extern "C" void slot_fn_5() { hit = "msg_draw__5Torch"; }
extern "C" void slot_fn_6() { hit = "msg_collision__5TorchP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__5TorchPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__5TorchPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void slot_fn_15() { hit = "clone__5Torch"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }

static const char* const expected[18] = {
    "__tf5Torch",
    "_$_5Torch",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__5Torch",
    "msg_draw__5Torch",
    "msg_collision__5TorchP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__5TorchPc",
    "msg_save__5TorchPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__10GameObjectPUsi",
    "msg_poke__10GameObjecti",
    "clone__5Torch",
    "getModel__10GameObject",
    nullptr,
};

int main() {
    if (sizeof(_vt_5Torch) / sizeof(_vt_5Torch[0]) != 18) return 1;
    for (int i = 0; i < 18; ++i) {
        if (_vt_5Torch[i].adjustment != 0) return 2;
        if (expected[i] == nullptr) {
            if (_vt_5Torch[i].function != nullptr) return 4;
            continue;
        }
        hit = "";
        _vt_5Torch[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 3;
    }
    return 0;
}
