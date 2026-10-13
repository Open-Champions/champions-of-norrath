// Observed _$_7WebTrap at 0x00311610, 108 bytes (weak). Stores the 7WebTrap table pointer
// (_vt$7WebTrap) at object+72, calls listDestroy__FP8ListHead on the list head embedded at object+136, then calls the GameObject destructor (_$_10GameObject) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 7WebTrap destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_7WebTrap[] __asm__("_vt$7WebTrap");
void base_destructor(void* self, int flags) __asm__("_$_10GameObject");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_7WebTrap(void* self, int flags) __asm__("_$_7WebTrap");
extern "C" void destructor_7WebTrap(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_7WebTrap));
    listDestroy__FP8ListHead(object + 136);
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
