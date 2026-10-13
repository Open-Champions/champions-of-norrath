// Host check: each slot function records its own image name; the table must list them in slot
// order with the adjustment words and the null slots observed in the image.
#include "vt_wraith.cpp"
#include <cstring>

static const char* hit = "";
extern "C" void slot_fn_0() { hit = "__tf6Wraith"; }
extern "C" void slot_fn_1() { hit = "_$_6Wraith"; }
extern "C" void slot_fn_2() { hit = "msg_hurt__8CreaturefP10GameObjecti"; }
extern "C" void slot_fn_3() { hit = "msg_trigger__8Creaturei"; }
extern "C" void slot_fn_4() { hit = "msg_run__6Wraith"; }
extern "C" void slot_fn_5() { hit = "msg_draw__6Wraith"; }
extern "C" void slot_fn_6() { hit = "msg_collision__8CreatureP10GameObject"; }
extern "C" void slot_fn_7() { hit = "msg_route__8Creaturei"; }
extern "C" void slot_fn_8() { hit = "msg_load__8CreaturePc"; }
extern "C" void slot_fn_9() { hit = "msg_save__8CreaturePc"; }
extern "C" void slot_fn_10() { hit = "msg_collisionWorld__8CreatureG6Point3"; }
extern "C" void slot_fn_11() { hit = "msg_moveCollision__8CreaturePP10GameObject"; }
extern "C" void slot_fn_12() { hit = "msg_alert__8CreatureifG6Point3P10GameObject"; }
extern "C" void slot_fn_13() { hit = "msg_getPokeName__8CreaturePUsi"; }
extern "C" void slot_fn_14() { hit = "msg_poke__8Creaturei"; }
extern "C" void slot_fn_15() { hit = "clone__6Wraith"; }
extern "C" void slot_fn_16() { hit = "getModel__8Creature"; }
extern "C" void slot_fn_17() { hit = "runSpells__8CreatureG6Point3"; }
extern "C" void slot_fn_18() { hit = "initMiniboss__6Wraith"; }
extern "C" void slot_fn_19() { hit = "getMinibossName__6Wraith"; }
extern "C" void slot_fn_20() { hit = "isEnemy__6WraithP10GameObject"; }
extern "C" void slot_fn_21() { hit = "updateEnemy__8Creature"; }
extern "C" void slot_fn_22() { hit = "turnToFace__8CreatureiG6Point3"; }
extern "C" void slot_fn_23() { hit = "setState__6Wraithi"; }
extern "C" void slot_fn_24() { hit = "death__8Creature"; }
extern "C" void slot_fn_25() { hit = "setActiveChunks__8Creature"; }
extern "C" void slot_fn_26() { hit = "stun__8Creaturei"; }
extern "C" void slot_fn_27() { hit = "confuse__8Creaturei"; }
extern "C" void slot_fn_28() { hit = "getSkillRank__8Creaturei"; }

static const char* const expected[30] = {
    "__tf6Wraith",
    "_$_6Wraith",
    "msg_hurt__8CreaturefP10GameObjecti",
    "msg_trigger__8Creaturei",
    "msg_run__6Wraith",
    "msg_draw__6Wraith",
    "msg_collision__8CreatureP10GameObject",
    "msg_route__8Creaturei",
    "msg_load__8CreaturePc",
    "msg_save__8CreaturePc",
    "msg_collisionWorld__8CreatureG6Point3",
    "msg_moveCollision__8CreaturePP10GameObject",
    "msg_alert__8CreatureifG6Point3P10GameObject",
    "msg_getPokeName__8CreaturePUsi",
    "msg_poke__8Creaturei",
    "clone__6Wraith",
    "getModel__8Creature",
    "runSpells__8CreatureG6Point3",
    "initMiniboss__6Wraith",
    "getMinibossName__6Wraith",
    "isEnemy__6WraithP10GameObject",
    "updateEnemy__8Creature",
    "turnToFace__8CreatureiG6Point3",
    "setState__6Wraithi",
    "death__8Creature",
    "setActiveChunks__8Creature",
    "stun__8Creaturei",
    "confuse__8Creaturei",
    "getSkillRank__8Creaturei",
    nullptr,
};

int main() {
    if (sizeof(_vt_6Wraith) / sizeof(_vt_6Wraith[0]) != 30) return 1;
    for (int i = 0; i < 30; ++i) {
        if (_vt_6Wraith[i].adjustment != 0) return 2;
        if (expected[i] == nullptr) {
            if (_vt_6Wraith[i].function != nullptr) return 4;
            continue;
        }
        hit = "";
        _vt_6Wraith[i].function();
        if (std::strcmp(hit, expected[i]) != 0) return 3;
    }
    return 0;
}
