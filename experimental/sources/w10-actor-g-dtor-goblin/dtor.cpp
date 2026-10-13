// Observed _$_6Goblin at 0x0030f220, 108 bytes (weak).
// Stores the Goblin table pointer (_vt$6Goblin) at object+72, runs listDestroy__FP8ListHead on the
// list head embedded at object+984, calls the Creature destructor
// (_$_8Creature) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the Goblin destructor from its symbol name, and +984 is a list head. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Goblin[] __asm__("_vt$6Goblin");
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void goblin_destructor(void* self, int flags) __asm__("_$_6Goblin");
extern "C" void goblin_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Goblin));
    listDestroy__FP8ListHead(object + 984);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
