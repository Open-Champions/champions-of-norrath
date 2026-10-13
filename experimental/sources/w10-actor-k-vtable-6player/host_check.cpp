// Host comparison only; not an EE build input. Compiles vtable.cpp for the host with a stub for each
// slot function and checks every slot's adjustment and function word, and the table length.
#include <cstdio>
const char* hit = "";
extern "C" void slot_fn_0() __asm__("__tf6Player");
extern "C" void slot_fn_0() { hit = "__tf6Player"; }
extern "C" void slot_fn_1() __asm__("_$_6Player");
extern "C" void slot_fn_1() { hit = "_$_6Player"; }
extern "C" void slot_fn_2() __asm__("msg_hurt__6PlayerfP10GameObjecti");
extern "C" void slot_fn_2() { hit = "msg_hurt__6PlayerfP10GameObjecti"; }
extern "C" void slot_fn_3() __asm__("msg_trigger__8Creaturei");
extern "C" void slot_fn_3() { hit = "msg_trigger__8Creaturei"; }
extern "C" void slot_fn_4() __asm__("msg_run__6Player");
extern "C" void slot_fn_4() { hit = "msg_run__6Player"; }
extern "C" void slot_fn_5() __asm__("msg_draw__6Player");
extern "C" void slot_fn_5() { hit = "msg_draw__6Player"; }
extern "C" void slot_fn_6() __asm__("msg_collision__6PlayerP10GameObject");
extern "C" void slot_fn_6() { hit = "msg_collision__6PlayerP10GameObject"; }
extern "C" void slot_fn_7() __asm__("msg_route__8Creaturei");
extern "C" void slot_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void slot_fn_8() __asm__("msg_load__8CreaturePc");
extern "C" void slot_fn_8() { hit = "msg_load__8CreaturePc"; }
extern "C" void slot_fn_9() __asm__("msg_save__8CreaturePc");
extern "C" void slot_fn_9() { hit = "msg_save__8CreaturePc"; }
extern "C" void slot_fn_10() __asm__("msg_collisionWorld__8CreatureG6Point3");
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void slot_fn_11() __asm__("msg_moveCollision__8CreaturePP10GameObject");
extern "C" void slot_fn_11() { hit = "msg_moveCollision__8CreaturePP10GameObject"; }
extern "C" void slot_fn_12() __asm__("msg_alert__8CreatureifG6Point3P10GameObject");
extern "C" void slot_fn_12() { hit = "msg_alert__8CreatureifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() __asm__("msg_getPokeName__8CreaturePUsi");
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void slot_fn_14() __asm__("msg_poke__8Creaturei");
extern "C" void slot_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void slot_fn_15() __asm__("clone__6Player");
extern "C" void slot_fn_15() { hit = "clone__6Player"; }
extern "C" void slot_fn_16() __asm__("getModel__6Player");
extern "C" void slot_fn_16() { hit = "getModel__6Player"; }
extern "C" void slot_fn_17() __asm__("runSpells__8CreatureG6Point3");
extern "C" void slot_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void slot_fn_18() __asm__("initMiniboss__8Creature");
extern "C" void slot_fn_18() { hit = "initMiniboss__8Creature"; }
extern "C" void slot_fn_19() __asm__("getMinibossName__8Creature");
extern "C" void slot_fn_19() { hit = "getMinibossName__8Creature"; }
extern "C" void slot_fn_20() __asm__("isEnemy__6PlayerP10GameObject");
extern "C" void slot_fn_20() { hit = "isEnemy__6PlayerP10GameObject"; }
extern "C" void slot_fn_21() __asm__("updateEnemy__8Creature");
extern "C" void slot_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void slot_fn_22() __asm__("turnToFace__8CreatureiG6Point3");
extern "C" void slot_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void slot_fn_23() __asm__("setState__6Playeri");
extern "C" void slot_fn_23() { hit = "setState__6Playeri"; }
extern "C" void slot_fn_24() __asm__("death__8Creature");
extern "C" void slot_fn_24() { hit = "death__8Creature"; }
extern "C" void slot_fn_25() __asm__("setActiveChunks__6Player");
extern "C" void slot_fn_25() { hit = "setActiveChunks__6Player"; }
extern "C" void slot_fn_26() __asm__("stun__6Playeri");
extern "C" void slot_fn_26() { hit = "stun__6Playeri"; }
extern "C" void slot_fn_27() __asm__("confuse__8Creaturei");
extern "C" void slot_fn_27() { hit = "confuse__8Creaturei"; }
extern "C" void slot_fn_28() __asm__("getSkillRank__6Playeri");
extern "C" void slot_fn_28() { hit = "getSkillRank__6Playeri"; }
struct Slot { unsigned adjustment; void (*function)(); };
extern "C" Slot vtable[30] __asm__("_vt$6Player");
int main() {
    const char* want[] = {
        "__tf6Player",
        "_$_6Player",
        "msg_hurt__6PlayerfP10GameObjecti",
        "msg_trigger__8Creaturei",
        "msg_run__6Player",
        "msg_draw__6Player",
        "msg_collision__6PlayerP10GameObject",
        "msg_route__8Creaturei",
        "msg_load__8CreaturePc",
        "msg_save__8CreaturePc",
        "msg_collisionWorld__8CreatureG6Point3",
        "msg_moveCollision__8CreaturePP10GameObject",
        "msg_alert__8CreatureifG6Point3P10GameObject",
        "msg_getPokeName__8CreaturePUsi",
        "msg_poke__8Creaturei",
        "clone__6Player",
        "getModel__6Player",
        "runSpells__8CreatureG6Point3",
        "initMiniboss__8Creature",
        "getMinibossName__8Creature",
        "isEnemy__6PlayerP10GameObject",
        "updateEnemy__8Creature",
        "turnToFace__8CreatureiG6Point3",
        "setState__6Playeri",
        "death__8Creature",
        "setActiveChunks__6Player",
        "stun__6Playeri",
        "confuse__8Creaturei",
        "getSkillRank__6Playeri",
        nullptr,
    };
    int bad = 0;
    for (int i = 0; i < 30; ++i) {
        if (vtable[i].adjustment != 0) bad = 1;
        if (!want[i]) { if (vtable[i].function) bad = 1; continue; }
        hit = "";
        if (!vtable[i].function) { bad = 1; continue; }
        vtable[i].function();
        const char* a = hit; const char* b = want[i];
        while (*a && *a == *b) { ++a; ++b; }
        if (*a != *b) { std::printf("slot %d: %s\n", i, hit); bad = 1; }
    }
    return bad;
}
