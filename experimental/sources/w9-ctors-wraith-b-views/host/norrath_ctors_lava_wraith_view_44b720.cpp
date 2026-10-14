// Host comparison only. Not an EE build input.
const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void slot_fn_0() asm("__tf3Orc");
extern "C" void slot_fn_0() { hit = "__tf3Orc"; }
extern "C" void slot_fn_1() asm("_$_3Orc");
extern "C" void slot_fn_1() { hit = "_$_3Orc"; }
extern "C" void slot_fn_2() asm("msg_hurt__3OrcfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__3OrcfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__8Creaturei");
extern "C" void slot_fn_3() { hit = "msg_trigger__8Creaturei"; }
extern "C" void slot_fn_4() asm("msg_run__3Orc");
extern "C" void slot_fn_4() { hit = "msg_run__3Orc"; }
extern "C" void slot_fn_5() asm("msg_draw__3Orc");
extern "C" void slot_fn_5() { hit = "msg_draw__3Orc"; }
extern "C" void slot_fn_6() asm("msg_collision__8CreatureP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__8CreatureP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__8Creaturei");
extern "C" void slot_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void slot_fn_8() asm("msg_load__3OrcPc");
extern "C" void slot_fn_8() { hit = "msg_load__3OrcPc"; }
extern "C" void slot_fn_9() asm("msg_save__3OrcPc");
extern "C" void slot_fn_9() { hit = "msg_save__3OrcPc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__8CreaturePP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__8CreaturePP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__3OrcifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__3OrcifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__8CreaturePUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__8Creaturei");
extern "C" void slot_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void slot_fn_15() asm("clone__3Orc");
extern "C" void slot_fn_15() { hit = "clone__3Orc"; }
extern "C" void slot_fn_16() asm("getModel__8Creature");
extern "C" void slot_fn_16() { hit = "getModel__8Creature"; }
extern "C" void slot_fn_17() asm("runSpells__8CreatureG6Point3");
extern "C" void slot_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void slot_fn_18() asm("initMiniboss__3Orc");
extern "C" void slot_fn_18() { hit = "initMiniboss__3Orc"; }
extern "C" void slot_fn_19() asm("getMinibossName__3Orc");
extern "C" void slot_fn_19() { hit = "getMinibossName__3Orc"; }
extern "C" void slot_fn_20() asm("isEnemy__8CreatureP10GameObject");
extern "C" void slot_fn_20() { hit = "isEnemy__8CreatureP10GameObject"; }
extern "C" void slot_fn_21() asm("updateEnemy__8Creature");
extern "C" void slot_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void slot_fn_22() asm("turnToFace__8CreatureiG6Point3");
extern "C" void slot_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void slot_fn_23() asm("setState__3Orci");
extern "C" void slot_fn_23() { hit = "setState__3Orci"; }
extern "C" void slot_fn_24() asm("death__8Creature");
extern "C" void slot_fn_24() { hit = "death__8Creature"; }
extern "C" void slot_fn_25() asm("setActiveChunks__8Creature");
extern "C" void slot_fn_25() { hit = "setActiveChunks__8Creature"; }
extern "C" void slot_fn_26() asm("stun__8Creaturei");
extern "C" void slot_fn_26() { hit = "stun__8Creaturei"; }
extern "C" void slot_fn_27() asm("confuse__8Creaturei");
extern "C" void slot_fn_27() { hit = "confuse__8Creaturei"; }
extern "C" void slot_fn_28() asm("getSkillRank__8Creaturei");
extern "C" void slot_fn_28() { hit = "getSkillRank__8Creaturei"; }
struct VirtualSlot { unsigned adjustment; void (*function)(); };
extern "C" VirtualSlot norrath_ctors_lava_wraith_view_44b720[30];
int main() {
    if (norrath_ctors_lava_wraith_view_44b720[0].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[0].function();
    if (!same(hit, "__tf3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[1].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[1].function();
    if (!same(hit, "_$_3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[2].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[2].function();
    if (!same(hit, "msg_hurt__3OrcfP10GameObjecti")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[3].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[3].function();
    if (!same(hit, "msg_trigger__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[4].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[4].function();
    if (!same(hit, "msg_run__3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[5].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[5].function();
    if (!same(hit, "msg_draw__3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[6].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[6].function();
    if (!same(hit, "msg_collision__8CreatureP10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[7].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[7].function();
    if (!same(hit, "msg_route__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[8].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[8].function();
    if (!same(hit, "msg_load__3OrcPc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[9].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[9].function();
    if (!same(hit, "msg_save__3OrcPc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[10].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[10].function();
    if (!same(hit, "msg_collisionWorld__8CreatureG6Point3")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[11].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[11].function();
    if (!same(hit, "msg_moveCollision__8CreaturePP10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[12].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[12].function();
    if (!same(hit, "msg_alert__3OrcifG6Point3P10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[13].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[13].function();
    if (!same(hit, "msg_getPokeName__8CreaturePUsi")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[14].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[14].function();
    if (!same(hit, "msg_poke__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[15].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[15].function();
    if (!same(hit, "clone__3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[16].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[16].function();
    if (!same(hit, "getModel__8Creature")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[17].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[17].function();
    if (!same(hit, "runSpells__8CreatureG6Point3")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[18].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[18].function();
    if (!same(hit, "initMiniboss__3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[19].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[19].function();
    if (!same(hit, "getMinibossName__3Orc")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[20].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[20].function();
    if (!same(hit, "isEnemy__8CreatureP10GameObject")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[21].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[21].function();
    if (!same(hit, "updateEnemy__8Creature")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[22].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[22].function();
    if (!same(hit, "turnToFace__8CreatureiG6Point3")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[23].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[23].function();
    if (!same(hit, "setState__3Orci")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[24].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[24].function();
    if (!same(hit, "death__8Creature")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[25].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[25].function();
    if (!same(hit, "setActiveChunks__8Creature")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[26].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[26].function();
    if (!same(hit, "stun__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[27].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[27].function();
    if (!same(hit, "confuse__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[28].adjustment != 0x0u) return 1;
    hit = "";
    norrath_ctors_lava_wraith_view_44b720[28].function();
    if (!same(hit, "getSkillRank__8Creaturei")) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[29].adjustment != 0x0u) return 1;
    if (norrath_ctors_lava_wraith_view_44b720[29].function != 0) return 1;
    return 0;
}
