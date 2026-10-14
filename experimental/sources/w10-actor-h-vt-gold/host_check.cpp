// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null terminator observed in the image.
#include "vt_gold.cpp"
#include <cstdio>
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf4Gold"; }
extern "C" void slot_fn_1() { hit = "_$_4Gold"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__4Gold"; }
extern "C" void slot_fn_5() { hit = "msg_draw__4Gold"; }
extern "C" void slot_fn_6() { hit = "msg_collision__4GoldP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__4GoldPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__4GoldPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__10GameObjectPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__10GameObjecti"; }
extern "C" void slot_fn_15() { hit = "clone__4Gold"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }

static const char* const expected[17] = {
    "__tf4Gold",
    "_$_4Gold",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__4Gold",
    "msg_draw__4Gold",
    "msg_collision__4GoldP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__4GoldPc",
    "msg_save__4GoldPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__10GameObjectPUsi",
    "msg_poke__10GameObjecti",
    "clone__4Gold",
    "getModel__10GameObject",
};

int main() {
    int bad = 0;
    for (int i = 0; i < 17; ++i) {
        hit = "";
        if (_vt_4Gold[i].adjustment != 0u) { std::printf("fail: adjustment of slot %d\n", i); bad = 1; }
        if (_vt_4Gold[i].function == nullptr) { std::printf("fail: null slot %d\n", i); bad = 1; continue; }
        _vt_4Gold[i].function();
        if (std::strcmp(hit, expected[i]) != 0) { std::printf("fail: slot %d is %s, expected %s\n", i, hit, expected[i]); bad = 1; }
    }
    if (_vt_4Gold[17].adjustment != 0u || _vt_4Gold[17].function != nullptr) { std::puts("fail: terminator slot"); bad = 1; }
    if (!bad) std::puts("Gold vtable ok");
    return bad;
}
