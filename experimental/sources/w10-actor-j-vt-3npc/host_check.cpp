// Host contract for _vt$3NPC; not an EE input. Every slot function is replaced by a stub that records
// its own symbol name; the table must hold the stubs in order with zero adjustments and end with a null slot.
#include <cstdio>
#include <cstring>
static const char* hit = "";
#include "vt_3npc.cpp"
extern "C" void slot_fn_0() { hit = "__tf3NPC"; }
extern "C" void slot_fn_1() { hit = "_$_3NPC"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__3NPCfP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__3NPCi"; }
extern "C" void slot_fn_4() { hit = "msg_run__3NPC"; }
extern "C" void slot_fn_5() { hit = "msg_draw__3NPC"; }
extern "C" void slot_fn_6() { hit = "msg_collision__8CreatureP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void slot_fn_8() { hit = "msg_load__3NPCPc"; }
extern "C" void slot_fn_9() { hit = "msg_save__3NPCPc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__8CreaturePP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__8CreatureifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void slot_fn_15() { hit = "clone__3NPC"; }
extern "C" void slot_fn_16() { hit = "getModel__3NPC"; }
extern "C" void slot_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void slot_fn_18() { hit = "initMiniboss__8Creature"; }
extern "C" void slot_fn_19() { hit = "getMinibossName__8Creature"; }
extern "C" void slot_fn_20() { hit = "isEnemy__8CreatureP10GameObject"; }
extern "C" void slot_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void slot_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void slot_fn_23() { hit = "setState__3NPCi"; }
extern "C" void slot_fn_24() { hit = "death__8Creature"; }
extern "C" void slot_fn_25() { hit = "setActiveChunks__8Creature"; }
extern "C" void slot_fn_26() { hit = "stun__8Creaturei"; }
extern "C" void slot_fn_27() { hit = "confuse__8Creaturei"; }
extern "C" void slot_fn_28() { hit = "getSkillRank__8Creaturei"; }
int main() {
    int bad = 0;
    const char* const expected[] = {"__tf3NPC", "_$_3NPC", "msg_hurt__3NPCfP10GameObjecti", "msg_trigger__3NPCi", "msg_run__3NPC", "msg_draw__3NPC", "msg_collision__8CreatureP10GameObject", "msg_route__8Creaturei", "msg_load__3NPCPc", "msg_save__3NPCPc", "msg_collisionWorld__8CreatureG6Point3", "msg_moveCollision__8CreaturePP10GameObject", "msg_alert__8CreatureifG6Point3P10GameObject", "msg_getPokeName__8CreaturePUsi", "msg_poke__8Creaturei", "clone__3NPC", "getModel__3NPC", "runSpells__8CreatureG6Point3", "initMiniboss__8Creature", "getMinibossName__8Creature", "isEnemy__8CreatureP10GameObject", "updateEnemy__8Creature", "turnToFace__8CreatureiG6Point3", "setState__3NPCi", "death__8Creature", "setActiveChunks__8Creature", "stun__8Creaturei", "confuse__8Creaturei", "getSkillRank__8Creaturei"};
    const unsigned count = 29, total = 30;
    static_assert(sizeof(vt_3NPC) == sizeof(VirtualSlot) * 30, "table size");
    for (unsigned i = 0; i < count; ++i) {
        if (vt_3NPC[i].adjustment != 0u || vt_3NPC[i].function == nullptr) { std::printf("fail slot %u\n", i); bad = 1; continue; }
        hit = ""; vt_3NPC[i].function();
        if (std::strcmp(hit, expected[i]) != 0) { std::printf("fail slot %u: %s\n", i, hit); bad = 1; }
    }
    for (unsigned i = count; i < total; ++i) if (vt_3NPC[i].adjustment != 0u || vt_3NPC[i].function != nullptr) { std::printf("fail tail %u\n", i); bad = 1; }
    return bad;
}
