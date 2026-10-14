// Host comparison only. Not an EE build input.
struct VirtualSlot { unsigned adjustment; void (*function)(); };
const char* hit = "";
int same(const char* a, const char* b) { for (unsigned i = 0;; ++i) { if (a[i] != b[i]) return 0; if (a[i] == 0) return 1; } }
extern "C" void slot_fn_0() asm("__tf9Savepoint");
extern "C" void slot_fn_0() { hit = "__tf9Savepoint"; }
extern "C" void slot_fn_1() asm("_$_9Savepoint");
extern "C" void slot_fn_1() { hit = "_$_9Savepoint"; }
extern "C" void slot_fn_2() asm("msg_hurt__10GameObjectfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__10GameObjectfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__10GameObjecti");
extern "C" void slot_fn_3() { hit = "msg_trigger__10GameObjecti"; }
extern "C" void slot_fn_4() asm("msg_run__9Savepoint");
extern "C" void slot_fn_4() { hit = "msg_run__9Savepoint"; }
extern "C" void slot_fn_5() asm("msg_draw__9Savepoint");
extern "C" void slot_fn_5() { hit = "msg_draw__9Savepoint"; }
extern "C" void slot_fn_6() asm("msg_collision__10GameObjectP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__10GameObjectP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__10GameObjecti");
extern "C" void slot_fn_7() { hit = "msg_route__10GameObjecti"; }
extern "C" void slot_fn_8() asm("msg_load__9SavepointPc");
extern "C" void slot_fn_8() { hit = "msg_load__9SavepointPc"; }
extern "C" void slot_fn_9() asm("msg_save__9SavepointPc");
extern "C" void slot_fn_9() { hit = "msg_save__9SavepointPc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__10GameObjectG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__10GameObjectG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__10GameObjectPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10GameObjectPP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__10GameObjectifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__10GameObjectifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__9SavepointPUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__9SavepointPUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__9Savepointi");
extern "C" void slot_fn_14() { hit = "msg_poke__9Savepointi"; }
extern "C" void slot_fn_15() asm("clone__9Savepoint");
extern "C" void slot_fn_15() { hit = "clone__9Savepoint"; }
extern "C" void slot_fn_16() asm("getModel__10GameObject");
extern "C" void slot_fn_16() { hit = "getModel__10GameObject"; }
extern "C" VirtualSlot norrath_ctors_lava_wraith_view_449550[18];
int main() {
    if (norrath_ctors_lava_wraith_view_449550[0].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[0].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[0].function(); if (!same(hit, "__tf9Savepoint")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[1].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[1].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[1].function(); if (!same(hit, "_$_9Savepoint")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[2].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[2].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[2].function(); if (!same(hit, "msg_hurt__10GameObjectfP10GameObjecti")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[3].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[3].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[3].function(); if (!same(hit, "msg_trigger__10GameObjecti")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[4].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[4].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[4].function(); if (!same(hit, "msg_run__9Savepoint")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[5].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[5].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[5].function(); if (!same(hit, "msg_draw__9Savepoint")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[6].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[6].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[6].function(); if (!same(hit, "msg_collision__10GameObjectP10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[7].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[7].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[7].function(); if (!same(hit, "msg_route__10GameObjecti")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[8].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[8].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[8].function(); if (!same(hit, "msg_load__9SavepointPc")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[9].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[9].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[9].function(); if (!same(hit, "msg_save__9SavepointPc")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[10].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[10].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[10].function(); if (!same(hit, "msg_collisionWorld__10GameObjectG6Point3")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[11].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[11].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[11].function(); if (!same(hit, "msg_moveCollision__10GameObjectPP10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[12].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[12].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[12].function(); if (!same(hit, "msg_alert__10GameObjectifG6Point3P10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[13].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[13].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[13].function(); if (!same(hit, "msg_getPokeName__9SavepointPUsi")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[14].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[14].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[14].function(); if (!same(hit, "msg_poke__9Savepointi")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[15].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[15].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[15].function(); if (!same(hit, "clone__9Savepoint")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[16].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[16].function == nullptr) return 1;
    hit = ""; norrath_ctors_lava_wraith_view_449550[16].function(); if (!same(hit, "getModel__10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_449550[17].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_449550[17].function != nullptr) return 1;
    return 0;
}
