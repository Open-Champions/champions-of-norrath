// Observed _$_12UndeadKnight at 0x00316090, 116 bytes (weak). Stores the 12UndeadKnight table pointer
// (_vt$12UndeadKnight at 0x44fc18) at object+72, then calls listDestroy__FP8ListHead on the head at object+964, then listDestroy__FP8ListHead on the head at object+960, then calls the Creature destructor (_$_8Creature) with
// flag 0 on the object, then calls blockFree__FPv on the object when bit 0 of the incoming flags
// is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 12UndeadKnight destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_UndeadKnight[] __asm__("_vt$12UndeadKnight");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
void listDestroy__FP8ListHead(void* head);
}

extern "C" void destructor_UndeadKnight(void* self, int flags) __asm__("_$_12UndeadKnight");
extern "C" void destructor_UndeadKnight(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_UndeadKnight));
    listDestroy__FP8ListHead(object + 964);
    listDestroy__FP8ListHead(object + 960);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
