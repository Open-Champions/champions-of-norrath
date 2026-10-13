// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_ant.cpp
// Every slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstddef>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void vt_fn_0() asm("__tf3Ant");
extern "C" void vt_fn_0() { hit = "__tf3Ant"; }
extern "C" void vt_fn_1() asm("_$_3Ant");
extern "C" void vt_fn_1() { hit = "_$_3Ant"; }
extern "C" void vt_fn_2() asm("msg_hurt__8CreaturefP10GameObjecti");
extern "C" void vt_fn_2() { hit = "msg_hurt__8CreaturefP10GameObjecti"; }
extern "C" void vt_fn_3() asm("msg_trigger__8Creaturei");
extern "C" void vt_fn_3() { hit = "msg_trigger__8Creaturei"; }
extern "C" void vt_fn_4() asm("msg_run__3Ant");
extern "C" void vt_fn_4() { hit = "msg_run__3Ant"; }
extern "C" void vt_fn_5() asm("msg_draw__3Ant");
extern "C" void vt_fn_5() { hit = "msg_draw__3Ant"; }
extern "C" void vt_fn_6() asm("msg_collision__8CreatureP10GameObject");
extern "C" void vt_fn_6() { hit = "msg_collision__8CreatureP10GameObject"; }
extern "C" void vt_fn_7() asm("msg_route__8Creaturei");
extern "C" void vt_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void vt_fn_8() asm("msg_load__3AntPc");
extern "C" void vt_fn_8() { hit = "msg_load__3AntPc"; }
extern "C" void vt_fn_9() asm("msg_save__3AntPc");
extern "C" void vt_fn_9() { hit = "msg_save__3AntPc"; }
extern "C" void vt_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
extern "C" void vt_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void vt_fn_11() asm("msg_moveCollision__8CreaturePP10GameObject");
extern "C" void vt_fn_11() { hit = "msg_moveCollision__8CreaturePP10GameObject"; }
extern "C" void vt_fn_12() asm("msg_alert__8CreatureifG6Point3P10GameObject");
extern "C" void vt_fn_12() { hit = "msg_alert__8CreatureifG6Point3P10GameObject"; }
extern "C" void vt_fn_13() asm("msg_getPokeName__8CreaturePUsi");
extern "C" void vt_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void vt_fn_14() asm("msg_poke__8Creaturei");
extern "C" void vt_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void vt_fn_15() asm("clone__3Ant");
extern "C" void vt_fn_15() { hit = "clone__3Ant"; }
extern "C" void vt_fn_16() asm("getModel__8Creature");
extern "C" void vt_fn_16() { hit = "getModel__8Creature"; }
extern "C" void vt_fn_17() asm("runSpells__8CreatureG6Point3");
extern "C" void vt_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void vt_fn_18() asm("initMiniboss__8Creature");
extern "C" void vt_fn_18() { hit = "initMiniboss__8Creature"; }
extern "C" void vt_fn_19() asm("getMinibossName__8Creature");
extern "C" void vt_fn_19() { hit = "getMinibossName__8Creature"; }
extern "C" void vt_fn_20() asm("isEnemy__8CreatureP10GameObject");
extern "C" void vt_fn_20() { hit = "isEnemy__8CreatureP10GameObject"; }
extern "C" void vt_fn_21() asm("updateEnemy__8Creature");
extern "C" void vt_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void vt_fn_22() asm("turnToFace__8CreatureiG6Point3");
extern "C" void vt_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void vt_fn_23() asm("setState__3Anti");
extern "C" void vt_fn_23() { hit = "setState__3Anti"; }
extern "C" void vt_fn_24() asm("death__8Creature");
extern "C" void vt_fn_24() { hit = "death__8Creature"; }
extern "C" void vt_fn_25() asm("setActiveChunks__8Creature");
extern "C" void vt_fn_25() { hit = "setActiveChunks__8Creature"; }
extern "C" void vt_fn_26() asm("stun__8Creaturei");
extern "C" void vt_fn_26() { hit = "stun__8Creaturei"; }
extern "C" void vt_fn_27() asm("confuse__8Creaturei");
extern "C" void vt_fn_27() { hit = "confuse__8Creaturei"; }
extern "C" void vt_fn_28() asm("getSkillRank__8Creaturei");
extern "C" void vt_fn_28() { hit = "getSkillRank__8Creaturei"; }
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_ant[30] asm("_vt$3Ant");

int main() {
    static const char* const want[29] = {
    "__tf3Ant",
    "_$_3Ant",
    "msg_hurt__8CreaturefP10GameObjecti",
    "msg_trigger__8Creaturei",
    "msg_run__3Ant",
    "msg_draw__3Ant",
    "msg_collision__8CreatureP10GameObject",
    "msg_route__8Creaturei",
    "msg_load__3AntPc",
    "msg_save__3AntPc",
    "msg_collisionWorld__8CreatureG6Point3",
    "msg_moveCollision__8CreaturePP10GameObject",
    "msg_alert__8CreatureifG6Point3P10GameObject",
    "msg_getPokeName__8CreaturePUsi",
    "msg_poke__8Creaturei",
    "clone__3Ant",
    "getModel__8Creature",
    "runSpells__8CreatureG6Point3",
    "initMiniboss__8Creature",
    "getMinibossName__8Creature",
    "isEnemy__8CreatureP10GameObject",
    "updateEnemy__8Creature",
    "turnToFace__8CreatureiG6Point3",
    "setState__3Anti",
    "death__8Creature",
    "setActiveChunks__8Creature",
    "stun__8Creaturei",
    "confuse__8Creaturei",
    "getSkillRank__8Creaturei",
    };
    if (sizeof(vt_ant) != 30 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 29; ++i) {
        if (vt_ant[i].adjustment != 0) return 2;
        hit = "";
        vt_ant[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_ant[29].adjustment != 0 || vt_ant[29].function != nullptr) return 4;
    std::printf("ok 3Ant 30 slots\n");
    return 0;
}
