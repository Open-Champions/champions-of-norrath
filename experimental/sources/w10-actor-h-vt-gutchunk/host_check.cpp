// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null terminator observed in the image.
#include "vt_gutchunk.cpp"
#include <cstdio>
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf8GutChunk"; }
extern "C" void slot_fn_1() { hit = "_$_8GutChunk"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() { hit = "msg_run__8GutChunk"; }
extern "C" void slot_fn_5() { hit = "msg_draw__8GutChunk"; }
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() { hit = "msg_load__4PropPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__4PropPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__6PhPropPUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__6PhPropi"; }
extern "C" void slot_fn_15() { hit = "clone__8GutChunk"; }
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }

static const char* const expected[17] = {
    "__tf8GutChunk",
    "_$_8GutChunk",
    "msg_hurt__10GameObjectfP10GameObjecti",
    "msg_trigger__10GameObjecti",
    "msg_run__8GutChunk",
    "msg_draw__8GutChunk",
    "msg_collision__10GameObjectP10GameObject",
    "msg_route__10GameObjecti",
    "msg_load__4PropPc",
    "msg_save__4PropPc",
    "msg_collisionWorld__10GameObjectG6Point3",
    "msg_moveCollision__10GameObjectPP10GameObject",
    "msg_alert__10GameObjectifG6Point3P10GameObject",
    "msg_getPokeName__6PhPropPUsi",
    "msg_poke__6PhPropi",
    "clone__8GutChunk",
    "getModel__10GameObject",
};

int main() {
    int bad = 0;
    for (int i = 0; i < 17; ++i) {
        hit = "";
        if (_vt_8GutChunk[i].adjustment != 0u) { std::printf("fail: adjustment of slot %d\n", i); bad = 1; }
        if (_vt_8GutChunk[i].function == nullptr) { std::printf("fail: null slot %d\n", i); bad = 1; continue; }
        _vt_8GutChunk[i].function();
        if (std::strcmp(hit, expected[i]) != 0) { std::printf("fail: slot %d is %s, expected %s\n", i, hit, expected[i]); bad = 1; }
    }
    if (_vt_8GutChunk[17].adjustment != 0u || _vt_8GutChunk[17].function != nullptr) { std::puts("fail: terminator slot"); bad = 1; }
    if (!bad) std::puts("GutChunk vtable ok");
    return bad;
}
