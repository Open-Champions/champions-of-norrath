// Observed _$_3Orc at 0x0030f398, 108 bytes (weak). Stores the class table (_vt$3Orc) at
// object+72 first, then calls listDestroy__FP8ListHead on the list head at object+1128, then
// runs the Creature destructor on the object with flag 0. When bit 0 of the incoming flags is
// set, blockFree__FPv is then called on the object (a tail call in the image). Other bits of
// the flags are not read. Nothing here ran on the EE.
extern "C" {
void listDestroy__FP8ListHead(void* head);
void base_destroy(void* object, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* object);
extern unsigned char vt_class[] __asm__("_vt$3Orc");
void destroy(void* object, int flags) __asm__("_$_3Orc");
}

extern "C" void destroy(void* object, int flags) {
    unsigned char* const o = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned*>(o + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_class));
    listDestroy__FP8ListHead(o + 1128);
    base_destroy(object, 0);
    if (flags & 1) blockFree__FPv(object);
}
