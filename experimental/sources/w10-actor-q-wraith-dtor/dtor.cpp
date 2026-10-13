// Observed _$_6Wraith at 0x0030fa28, 108 bytes (weak).
// Stores the Wraith table pointer (_vt$6Wraith) at object+72, destroys the list head at object+968 with
// listDestroy__FP8ListHead, then calls the Creature destructor (_$_8Creature) with flag 0, and finally
// calls blockFree__FPv on the object when bit 0 of the incoming flags is set (a tail call in the
// image). Other flag bits do not free. The list head word is not cleared here.
// Inferred: this is the Wraith destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Wraith[] __asm__("_vt$6Wraith");
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void wraith_destructor(void* self, int flags) __asm__("_$_6Wraith");
extern "C" void wraith_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const int release = flags & 1;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Wraith));
    listDestroy__FP8ListHead(object + 968);
    creature_destructor(object, 0);
    if (release != 0) blockFree__FPv(object);
}
