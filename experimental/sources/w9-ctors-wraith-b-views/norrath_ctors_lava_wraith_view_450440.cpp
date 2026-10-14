// Observed .data symbol _vt$11MaleDarkElf at 0x450440, 240 bytes.
// Each slot is the adjustment word followed by a function pointer.
// A zero function word is a null slot. Names are the sized ELF
// function symbols at those addresses. Compiler marker symbols that
// share an address are not the slot. Unknown: dispatch and overrides.
struct VirtualSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VirtualSlot) == 8, "virtual slot is an adjustment word plus a pointer");
#endif
extern "C" {
void slot_fn_0() asm("__tf11MaleDarkElf");
void slot_fn_1() asm("_$_11MaleDarkElf");
void slot_fn_2() asm("msg_hurt__8CreaturefP10GameObjecti");
void slot_fn_3() asm("msg_trigger__8Creaturei");
void slot_fn_4() asm("msg_run__11MaleDarkElf");
void slot_fn_5() asm("msg_draw__11MaleDarkElf");
void slot_fn_6() asm("msg_collision__8CreatureP10GameObject");
void slot_fn_7() asm("msg_route__8Creaturei");
void slot_fn_8() asm("msg_load__11MaleDarkElfPc");
void slot_fn_9() asm("msg_save__11MaleDarkElfPc");
void slot_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
void slot_fn_11() asm("msg_moveCollision__8CreaturePP10GameObject");
void slot_fn_12() asm("msg_alert__11MaleDarkElfifG6Point3P10GameObject");
void slot_fn_13() asm("msg_getPokeName__8CreaturePUsi");
void slot_fn_14() asm("msg_poke__8Creaturei");
void slot_fn_15() asm("clone__11MaleDarkElf");
void slot_fn_16() asm("getModel__8Creature");
void slot_fn_17() asm("runSpells__8CreatureG6Point3");
void slot_fn_18() asm("initMiniboss__11MaleDarkElf");
void slot_fn_19() asm("getMinibossName__11MaleDarkElf");
void slot_fn_20() asm("isEnemy__8CreatureP10GameObject");
void slot_fn_21() asm("updateEnemy__8Creature");
void slot_fn_22() asm("turnToFace__8CreatureiG6Point3");
void slot_fn_23() asm("setState__11MaleDarkElfi");
void slot_fn_24() asm("death__8Creature");
void slot_fn_25() asm("setActiveChunks__8Creature");
void slot_fn_26() asm("stun__8Creaturei");
void slot_fn_27() asm("confuse__8Creaturei");
void slot_fn_28() asm("getSkillRank__8Creaturei");
VirtualSlot norrath_ctors_lava_wraith_view_450440[30] = {
    {0x0u, slot_fn_0},
    {0x0u, slot_fn_1},
    {0x0u, slot_fn_2},
    {0x0u, slot_fn_3},
    {0x0u, slot_fn_4},
    {0x0u, slot_fn_5},
    {0x0u, slot_fn_6},
    {0x0u, slot_fn_7},
    {0x0u, slot_fn_8},
    {0x0u, slot_fn_9},
    {0x0u, slot_fn_10},
    {0x0u, slot_fn_11},
    {0x0u, slot_fn_12},
    {0x0u, slot_fn_13},
    {0x0u, slot_fn_14},
    {0x0u, slot_fn_15},
    {0x0u, slot_fn_16},
    {0x0u, slot_fn_17},
    {0x0u, slot_fn_18},
    {0x0u, slot_fn_19},
    {0x0u, slot_fn_20},
    {0x0u, slot_fn_21},
    {0x0u, slot_fn_22},
    {0x0u, slot_fn_23},
    {0x0u, slot_fn_24},
    {0x0u, slot_fn_25},
    {0x0u, slot_fn_26},
    {0x0u, slot_fn_27},
    {0x0u, slot_fn_28},
    {0x0u, nullptr},
};
}
