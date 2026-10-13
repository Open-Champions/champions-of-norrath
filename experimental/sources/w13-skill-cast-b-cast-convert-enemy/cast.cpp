// Observed cast__17SkillConvertEnemyiP8CreatureG6Point3s at 0x00298b00, 156 bytes in .text.
// Steps, in order: (1) the by-value Point3 is copied to the frame and not read; (2) a 16-byte local list head is
// created with its first word zero (the other words are not written); (3) 120 bytes are allocated with blockAlloc__Fi;
// (4) when the block is not null, __17SpellConvertEnemyiP8CreatureP5Skill is called with (block, rank, creature, skill); (5) listDestroy__FP8ListHead is
// called on the local list whether or not the allocation succeeded. The block pointer is returned, null when the
// allocation failed.
// Inferred: the list is an empty leftover of an inlined helper; nothing between its creation and its destruction
// adds to it. Unknown: the real layout of the list head (only its first word is written here) and whether the
// destroy call does any work on an empty list.
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };
extern "C" {
void* blockAlloc__Fi(int size);
void spell_ctor_convert_enemy(void* self, int rank, void* creature, void* skill) asm("__17SpellConvertEnemyiP8CreatureP5Skill");
void listDestroy__FP8ListHead(ListHead* list);
void* cast_convert_enemy(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillConvertEnemyiP8CreatureG6Point3s");
}
extern "C" void* cast_convert_enemy(void* skill, int rank, void* creature, Point3, short) {
    ListHead list;
    list.first = nullptr;
    void* spell = blockAlloc__Fi(120);
    if (spell) spell_ctor_convert_enemy(spell, rank, creature, skill);
    listDestroy__FP8ListHead(&list);
    return spell;
}
