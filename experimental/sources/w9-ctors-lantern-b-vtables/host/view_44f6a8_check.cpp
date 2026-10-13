// Host comparison only. Not an EE build input. Each slot function records
// its own image name; the table must list them in slot order with the
// adjustment words and null slots observed in the image.
#include <cstring>
static const char* hit = "";
extern "C" void slot_fn_0() asm("__tf10Arenabeast");
extern "C" void slot_fn_0() { hit = "__tf10Arenabeast"; }
extern "C" void slot_fn_1() asm("_$_10Arenabeast");
extern "C" void slot_fn_1() { hit = "_$_10Arenabeast"; }
extern "C" void slot_fn_2() asm("msg_hurt__10ArenabeastfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__10ArenabeastfP10GameObjecti"; }
extern "C" void slot_fn_3() asm("msg_trigger__10Arenabeasti");
extern "C" void slot_fn_3() { hit = "msg_trigger__10Arenabeasti"; }
extern "C" void slot_fn_4() asm("msg_run__10Arenabeast");
extern "C" void slot_fn_4() { hit = "msg_run__10Arenabeast"; }
extern "C" void slot_fn_5() asm("msg_draw__10Arenabeast");
extern "C" void slot_fn_5() { hit = "msg_draw__10Arenabeast"; }
extern "C" void slot_fn_6() asm("msg_collision__8CreatureP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__8CreatureP10GameObject"; }
extern "C" void slot_fn_7() asm("msg_route__8Creaturei");
extern "C" void slot_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void slot_fn_8() asm("msg_load__8CreaturePc");
extern "C" void slot_fn_8() { hit = "msg_load__8CreaturePc"; }
extern "C" void slot_fn_9() asm("msg_save__8CreaturePc");
extern "C" void slot_fn_9() { hit = "msg_save__8CreaturePc"; }
extern "C" void slot_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void slot_fn_11() asm("msg_moveCollision__10ArenabeastPP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__10ArenabeastPP10GameObject"; }
extern "C" void slot_fn_12() asm("msg_alert__8CreatureifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__8CreatureifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() asm("msg_getPokeName__8CreaturePUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void slot_fn_14() asm("msg_poke__8Creaturei");
extern "C" void slot_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void slot_fn_15() asm("clone__8Creature");
extern "C" void slot_fn_15() { hit = "clone__8Creature"; }
extern "C" void slot_fn_16() asm("getModel__8Creature");
extern "C" void slot_fn_16() { hit = "getModel__8Creature"; }
extern "C" void slot_fn_17() asm("runSpells__8CreatureG6Point3");
extern "C" void slot_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void slot_fn_18() asm("initMiniboss__8Creature");
extern "C" void slot_fn_18() { hit = "initMiniboss__8Creature"; }
extern "C" void slot_fn_19() asm("getMinibossName__8Creature");
extern "C" void slot_fn_19() { hit = "getMinibossName__8Creature"; }
extern "C" void slot_fn_20() asm("isEnemy__8CreatureP10GameObject");
extern "C" void slot_fn_20() { hit = "isEnemy__8CreatureP10GameObject"; }
extern "C" void slot_fn_21() asm("updateEnemy__8Creature");
extern "C" void slot_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void slot_fn_22() asm("turnToFace__8CreatureiG6Point3");
extern "C" void slot_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void slot_fn_23() asm("setState__10Arenabeasti");
extern "C" void slot_fn_23() { hit = "setState__10Arenabeasti"; }
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
#include "../view_44f6a8.cpp"
static const char* const expected[30] = {
    "__tf10Arenabeast",
    "_$_10Arenabeast",
    "msg_hurt__10ArenabeastfP10GameObjecti",
    "msg_trigger__10Arenabeasti",
    "msg_run__10Arenabeast",
    "msg_draw__10Arenabeast",
    "msg_collision__8CreatureP10GameObject",
    "msg_route__8Creaturei",
    "msg_load__8CreaturePc",
    "msg_save__8CreaturePc",
    "msg_collisionWorld__8CreatureG6Point3",
    "msg_moveCollision__10ArenabeastPP10GameObject",
    "msg_alert__8CreatureifG6Point3P10GameObject",
    "msg_getPokeName__8CreaturePUsi",
    "msg_poke__8Creaturei",
    "clone__8Creature",
    "getModel__8Creature",
    "runSpells__8CreatureG6Point3",
    "initMiniboss__8Creature",
    "getMinibossName__8Creature",
    "isEnemy__8CreatureP10GameObject",
    "updateEnemy__8Creature",
    "turnToFace__8CreatureiG6Point3",
    "setState__10Arenabeasti",
    "death__8Creature",
    "setActiveChunks__8Creature",
    "stun__8Creaturei",
    "confuse__8Creaturei",
    "getSkillRank__8Creaturei",
    nullptr,
};
static const unsigned adjustments[30] = {0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_44f6a8 / sizeof norrath_ctors_ant_lantern_view_44f6a8[0] != 30) return 1;
    for (unsigned i = 0; i < 30; ++i) {
        if (norrath_ctors_ant_lantern_view_44f6a8[i].adjustment != adjustments[i]) return 2;
        if (expected[i] == nullptr) {
            if (norrath_ctors_ant_lantern_view_44f6a8[i].function != nullptr) return 3;
            continue;
        }
        if (norrath_ctors_ant_lantern_view_44f6a8[i].function == nullptr) return 4;
        hit = "";
        norrath_ctors_ant_lantern_view_44f6a8[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 5;
    }
    return 0;
}
