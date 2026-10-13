// Observed _$_14WoodElfSoldier at 0x0030f968, 108 bytes (weak). Stores the 14WoodElfSoldier table pointer
// (_vt$14WoodElfSoldier) at object+72, calls listDestroy__FP8ListHead on the list head embedded at object+960, then calls the Creature destructor (_$_8Creature) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 14WoodElfSoldier destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_14WoodElfSoldier[] __asm__("_vt$14WoodElfSoldier");
void base_destructor(void* self, int flags) __asm__("_$_8Creature");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_14WoodElfSoldier(void* self, int flags) __asm__("_$_14WoodElfSoldier");
extern "C" void destructor_14WoodElfSoldier(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_14WoodElfSoldier));
    listDestroy__FP8ListHead(object + 960);
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
