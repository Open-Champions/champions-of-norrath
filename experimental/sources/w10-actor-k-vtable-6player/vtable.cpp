// Observed _vt$6Player at 0x44c620, 240 bytes in .data. 30 slots; each slot is a zero adjustment word plus a
// function word. The function words, in order, are:
// 0 __tf6Player; 1 _$_6Player; 2 msg_hurt__6PlayerfP10GameObjecti; 3 msg_trigger__8Creaturei;
// 4 msg_run__6Player; 5 msg_draw__6Player; 6 msg_collision__6PlayerP10GameObject;
// 7 msg_route__8Creaturei; 8 msg_load__8CreaturePc; 9 msg_save__8CreaturePc;
// 10 msg_collisionWorld__8CreatureG6Point3; 11 msg_moveCollision__8CreaturePP10GameObject;
// 12 msg_alert__8CreatureifG6Point3P10GameObject; 13 msg_getPokeName__8CreaturePUsi;
// 14 msg_poke__8Creaturei; 15 clone__6Player; 16 getModel__6Player;
// 17 runSpells__8CreatureG6Point3; 18 initMiniboss__8Creature; 19 getMinibossName__8Creature;
// 20 isEnemy__6PlayerP10GameObject; 21 updateEnemy__8Creature;
// 22 turnToFace__8CreatureiG6Point3; 23 setState__6Playeri; 24 death__8Creature;
// 25 setActiveChunks__6Player; 26 stun__6Playeri; 27 confuse__8Creaturei;
// 28 getSkillRank__6Playeri; 29 null.
// The bodies behind those words are not defined here; the slots reference the observed names,
// so they are imports of this unit.
// Nothing here ran on the EE; dispatch through this table is not exercised.
struct Slot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(Slot) == 8, "a slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() __asm__("__tf6Player");
void slot_fn_1() __asm__("_$_6Player");
void slot_fn_2() __asm__("msg_hurt__6PlayerfP10GameObjecti");
void slot_fn_3() __asm__("msg_trigger__8Creaturei");
void slot_fn_4() __asm__("msg_run__6Player");
void slot_fn_5() __asm__("msg_draw__6Player");
void slot_fn_6() __asm__("msg_collision__6PlayerP10GameObject");
void slot_fn_7() __asm__("msg_route__8Creaturei");
void slot_fn_8() __asm__("msg_load__8CreaturePc");
void slot_fn_9() __asm__("msg_save__8CreaturePc");
void slot_fn_10() __asm__("msg_collisionWorld__8CreatureG6Point3");
void slot_fn_11() __asm__("msg_moveCollision__8CreaturePP10GameObject");
void slot_fn_12() __asm__("msg_alert__8CreatureifG6Point3P10GameObject");
void slot_fn_13() __asm__("msg_getPokeName__8CreaturePUsi");
void slot_fn_14() __asm__("msg_poke__8Creaturei");
void slot_fn_15() __asm__("clone__6Player");
void slot_fn_16() __asm__("getModel__6Player");
void slot_fn_17() __asm__("runSpells__8CreatureG6Point3");
void slot_fn_18() __asm__("initMiniboss__8Creature");
void slot_fn_19() __asm__("getMinibossName__8Creature");
void slot_fn_20() __asm__("isEnemy__6PlayerP10GameObject");
void slot_fn_21() __asm__("updateEnemy__8Creature");
void slot_fn_22() __asm__("turnToFace__8CreatureiG6Point3");
void slot_fn_23() __asm__("setState__6Playeri");
void slot_fn_24() __asm__("death__8Creature");
void slot_fn_25() __asm__("setActiveChunks__6Player");
void slot_fn_26() __asm__("stun__6Playeri");
void slot_fn_27() __asm__("confuse__8Creaturei");
void slot_fn_28() __asm__("getSkillRank__6Playeri");
Slot vtable[30] __asm__("_vt$6Player") = {
    {0u, slot_fn_0},
    {0u, slot_fn_1},
    {0u, slot_fn_2},
    {0u, slot_fn_3},
    {0u, slot_fn_4},
    {0u, slot_fn_5},
    {0u, slot_fn_6},
    {0u, slot_fn_7},
    {0u, slot_fn_8},
    {0u, slot_fn_9},
    {0u, slot_fn_10},
    {0u, slot_fn_11},
    {0u, slot_fn_12},
    {0u, slot_fn_13},
    {0u, slot_fn_14},
    {0u, slot_fn_15},
    {0u, slot_fn_16},
    {0u, slot_fn_17},
    {0u, slot_fn_18},
    {0u, slot_fn_19},
    {0u, slot_fn_20},
    {0u, slot_fn_21},
    {0u, slot_fn_22},
    {0u, slot_fn_23},
    {0u, slot_fn_24},
    {0u, slot_fn_25},
    {0u, slot_fn_26},
    {0u, slot_fn_27},
    {0u, slot_fn_28},
    {0u, nullptr},
};
}
