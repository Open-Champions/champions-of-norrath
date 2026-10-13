// Observed _vt$8Skeleton at 0x0044fee8, 240 bytes in .data (weak object in the image).
// 30 slots of 8 bytes: a zero adjustment word and a function word. The last slot is null.
// The 29 function words, in slot order, are the observed symbol names listed below; each
// name is the image symbol at that address. The bodies are not in this unit.
// Inferred: a constructor of this class stores this table address in the object (another unit).
// Unknown: dispatch order beyond the table itself, and the role of the null last slot.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_skeleton_fn_0() asm("__tf8Skeleton");
void vt_skeleton_fn_1() asm("_$_8Skeleton");
void vt_skeleton_fn_2() asm("msg_hurt__8SkeletonfP10GameObjecti");
void vt_skeleton_fn_3() asm("msg_trigger__8Creaturei");
void vt_skeleton_fn_4() asm("msg_run__8Skeleton");
void vt_skeleton_fn_5() asm("msg_draw__8Skeleton");
void vt_skeleton_fn_6() asm("msg_collision__8CreatureP10GameObject");
void vt_skeleton_fn_7() asm("msg_route__8Creaturei");
void vt_skeleton_fn_8() asm("msg_load__8SkeletonPc");
void vt_skeleton_fn_9() asm("msg_save__8SkeletonPc");
void vt_skeleton_fn_10() asm("msg_collisionWorld__8CreatureG6Point3");
void vt_skeleton_fn_11() asm("msg_moveCollision__8CreaturePP10GameObject");
void vt_skeleton_fn_12() asm("msg_alert__8SkeletonifG6Point3P10GameObject");
void vt_skeleton_fn_13() asm("msg_getPokeName__8CreaturePUsi");
void vt_skeleton_fn_14() asm("msg_poke__8Creaturei");
void vt_skeleton_fn_15() asm("clone__8Skeleton");
void vt_skeleton_fn_16() asm("getModel__8Creature");
void vt_skeleton_fn_17() asm("runSpells__8CreatureG6Point3");
void vt_skeleton_fn_18() asm("initMiniboss__8Creature");
void vt_skeleton_fn_19() asm("getMinibossName__8Creature");
void vt_skeleton_fn_20() asm("isEnemy__8SkeletonP10GameObject");
void vt_skeleton_fn_21() asm("updateEnemy__8Creature");
void vt_skeleton_fn_22() asm("turnToFace__8CreatureiG6Point3");
void vt_skeleton_fn_23() asm("setState__8Skeletoni");
void vt_skeleton_fn_24() asm("death__8Creature");
void vt_skeleton_fn_25() asm("setActiveChunks__8Creature");
void vt_skeleton_fn_26() asm("stun__8Creaturei");
void vt_skeleton_fn_27() asm("confuse__8Creaturei");
void vt_skeleton_fn_28() asm("getSkillRank__8Creaturei");
VtSlot vt_skeleton[30] asm("_vt$8Skeleton") = {
    {0x0u, vt_skeleton_fn_0},
    {0x0u, vt_skeleton_fn_1},
    {0x0u, vt_skeleton_fn_2},
    {0x0u, vt_skeleton_fn_3},
    {0x0u, vt_skeleton_fn_4},
    {0x0u, vt_skeleton_fn_5},
    {0x0u, vt_skeleton_fn_6},
    {0x0u, vt_skeleton_fn_7},
    {0x0u, vt_skeleton_fn_8},
    {0x0u, vt_skeleton_fn_9},
    {0x0u, vt_skeleton_fn_10},
    {0x0u, vt_skeleton_fn_11},
    {0x0u, vt_skeleton_fn_12},
    {0x0u, vt_skeleton_fn_13},
    {0x0u, vt_skeleton_fn_14},
    {0x0u, vt_skeleton_fn_15},
    {0x0u, vt_skeleton_fn_16},
    {0x0u, vt_skeleton_fn_17},
    {0x0u, vt_skeleton_fn_18},
    {0x0u, vt_skeleton_fn_19},
    {0x0u, vt_skeleton_fn_20},
    {0x0u, vt_skeleton_fn_21},
    {0x0u, vt_skeleton_fn_22},
    {0x0u, vt_skeleton_fn_23},
    {0x0u, vt_skeleton_fn_24},
    {0x0u, vt_skeleton_fn_25},
    {0x0u, vt_skeleton_fn_26},
    {0x0u, vt_skeleton_fn_27},
    {0x0u, vt_skeleton_fn_28},
    {0x0u, nullptr},
};
}
