// Observed _$_5Spell at 0x00314990, 100 bytes (weak symbol in the private executable).
// This is the base destructor of the spell class. In order it stores the table _vt$5Spell at object+20, calls
// listDestroy__FP8ListHead on object+4, and then, when bit 0 of the incoming flags is set, calls blockFree__FPv
// on the object (a tail call in the image). Other flag bits do not free.
// Inferred: the base destructor of the spell class, following the same shape as the derived destructors in this
// family. Unknown: the layout of the list head at object+4 beyond the call. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_5Spell[] __asm__("_vt$5Spell");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_5Spell(void* self, int flags) __asm__("_$_5Spell");
extern "C" void destructor_5Spell(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell));
    listDestroy__FP8ListHead(object + 4);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
