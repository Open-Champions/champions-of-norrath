// Observed _$_19SpellSummonSkeleton at 0x003188d8, 116 bytes (weak symbol in the private executable).
// Destructor of the summon-skeleton spell class. In order it stores _vt$19SpellSummonSkeleton at object+20,
// calls listDestroy__FP8ListHead on object+24, stores the base table _vt$5Spell at object+20, calls
// listDestroy__FP8ListHead on object+4, and then, when bit 0 of the incoming flags is set, calls blockFree__FPv
// on the object (a tail call in the image). Other flag bits do not free.
// Inferred: the same family and shape as the hammer-of-wrath destructor. Unknown: the element type of the list at
// +24 and the reason for the base-table store between the two list calls. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_SummonSkeleton[] __asm__("_vt$19SpellSummonSkeleton");
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_SummonSkeleton(void* self, int flags) __asm__("_$_19SpellSummonSkeleton");
extern "C" void destructor_SummonSkeleton(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const int freeBlock = flags & 1;
    *reinterpret_cast<unsigned*>(object + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_SummonSkeleton));
    listDestroy__FP8ListHead(object + 24);
    *reinterpret_cast<unsigned*>(object + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Spell));
    listDestroy__FP8ListHead(object + 4);
    if (freeBlock != 0) blockFree__FPv(object);
}
