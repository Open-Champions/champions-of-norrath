// Observed _$_11VampireLord at 0x00316220, 108 bytes (weak). Stores the 11VampireLord table pointer
// (_vt$11VampireLord) at object+72, calls listDestroy__FP8ListHead on the list head embedded at object+1028, then calls the Creature destructor (_$_8Creature) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 11VampireLord destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_11VampireLord[] __asm__("_vt$11VampireLord");
void base_destructor(void* self, int flags) __asm__("_$_8Creature");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_11VampireLord(void* self, int flags) __asm__("_$_11VampireLord");
extern "C" void destructor_11VampireLord(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_11VampireLord));
    listDestroy__FP8ListHead(object + 1028);
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
