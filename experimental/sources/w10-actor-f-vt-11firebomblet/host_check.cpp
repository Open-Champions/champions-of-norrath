// Host contract for _vt$11FireBomblet. Defines stand-ins for every slot target; not an EE input.
#include "vtable.cpp"
#include <cstdio>
#define STUB(n, s) extern "C" void stub_##n() asm(s); extern "C" void stub_##n() {}
STUB(0, "__tf11FireBomblet") STUB(1, "_$_11FireBomblet") STUB(2, "msg_hurt__10GameObjectfP10GameObjecti")
STUB(3, "msg_trigger__10GameObjecti") STUB(4, "msg_run__11FireBomblet") STUB(5, "msg_draw__11FireBomblet")
STUB(6, "msg_collision__10GameObjectP10GameObject") STUB(7, "msg_route__10GameObjecti")
STUB(8, "msg_load__10GameObjectPc") STUB(9, "msg_save__10GameObjectPc")
STUB(10, "msg_collisionWorld__10GameObjectG6Point3") STUB(11, "msg_moveCollision__10GameObjectPP10GameObject")
STUB(12, "msg_alert__10GameObjectifG6Point3P10GameObject") STUB(13, "msg_getPokeName__10GameObjectPUsi")
STUB(14, "msg_poke__10GameObjecti") STUB(15, "clone__11FireBomblet") STUB(16, "getModel__10GameObject")
int main() {
    void (*const want[17])() = {stub_0, stub_1, stub_2, stub_3, stub_4, stub_5, stub_6, stub_7, stub_8,
                                stub_9, stub_10, stub_11, stub_12, stub_13, stub_14, stub_15, stub_16};
    if (sizeof(vt_11FireBomblet) != 18 * sizeof(VirtualSlot)) return 1;
    for (int i = 0; i < 17; ++i) {
        if (vt_11FireBomblet[i].adjustment != 0 || vt_11FireBomblet[i].function != want[i]) { std::printf("slot %d\n", i); return 2; }
    }
    return vt_11FireBomblet[17].adjustment == 0 && vt_11FireBomblet[17].function == nullptr ? 0 : 3;
}
