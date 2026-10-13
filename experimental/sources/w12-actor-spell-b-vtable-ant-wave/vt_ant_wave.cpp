// Observed _vt$7AntWave at 0x448b90, 240 bytes in .data (weak object in the image).
// 30 slots of 8 bytes: a zero adjustment word and a function word, the last
// function word being zero. The function words, in order, are the symbols
// listed in the slot table of docs/agents/w12-actor-spell-b.md (29 distinct names).
// Each name is the sized function symbol at that address in the image; the
// bodies behind them are not in this unit, so every slot refers to the
// observed name and this unit adds no function definitions of its own.
// Inferred: the constructor of AntWave stores this table's address at object+72.
// That store is made by a different unit and is not exercised here.
// Unknown: dispatch through this table, and the role of the last (null) slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf7AntWave");
void vt_fn_1() asm("_$_7AntWave");
void vt_fn_2() asm("msg_hurt__8CreaturefP10GameObjecti");
void vt_fn_3() asm("msg_trigger__8Creaturei");
void vt_fn_4() asm("msg_run__7AntWave");
void vt_fn_5() asm("msg_draw__7AntWave");
void vt_fn_6() asm("msg_collision__8CreatureP10GameObject");
void vt_fn_7() asm("msg_route__8Creaturei");
void vt_fn_8() asm("msg_load__8CreaturePc");
void vt_fn_9() asm("msg_save__8CreaturePc");
void vt_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
void vt_fn_11() asm("msg_moveCollision__8CreaturePP10GameObject");
void vt_fn_12() asm("msg_alert__8CreatureifG6Point3P10GameObject");
void vt_fn_13() asm("msg_getPokeName__8CreaturePUsi");
void vt_fn_14() asm("msg_poke__8Creaturei");
void vt_fn_15() asm("clone__7AntWave");
void vt_fn_16() asm("getModel__8Creature");
void vt_fn_17() asm("runSpells__8CreatureG6Point3");
void vt_fn_18() asm("initMiniboss__8Creature");
void vt_fn_19() asm("getMinibossName__8Creature");
void vt_fn_20() asm("isEnemy__8CreatureP10GameObject");
void vt_fn_21() asm("updateEnemy__8Creature");
void vt_fn_22() asm("turnToFace__8CreatureiG6Point3");
void vt_fn_23() asm("setState__8Creaturei");
void vt_fn_24() asm("death__8Creature");
void vt_fn_25() asm("setActiveChunks__8Creature");
void vt_fn_26() asm("stun__8Creaturei");
void vt_fn_27() asm("confuse__8Creaturei");
void vt_fn_28() asm("getSkillRank__8Creaturei");
VtSlot vt_ant_wave[30] asm("_vt$7AntWave") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, vt_fn_5},
    {0x0u, vt_fn_6},
    {0x0u, vt_fn_7},
    {0x0u, vt_fn_8},
    {0x0u, vt_fn_9},
    {0x0u, vt_fn_10},
    {0x0u, vt_fn_11},
    {0x0u, vt_fn_12},
    {0x0u, vt_fn_13},
    {0x0u, vt_fn_14},
    {0x0u, vt_fn_15},
    {0x0u, vt_fn_16},
    {0x0u, vt_fn_17},
    {0x0u, vt_fn_18},
    {0x0u, vt_fn_19},
    {0x0u, vt_fn_20},
    {0x0u, vt_fn_21},
    {0x0u, vt_fn_22},
    {0x0u, vt_fn_23},
    {0x0u, vt_fn_24},
    {0x0u, vt_fn_25},
    {0x0u, vt_fn_26},
    {0x0u, vt_fn_27},
    {0x0u, vt_fn_28},
    {0x0u, nullptr},
};
}
#if defined(__mips__)
static_assert(sizeof(vt_ant_wave) == 240, "table size is 240 bytes");
#endif
