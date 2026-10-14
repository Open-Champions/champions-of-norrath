// Observed _$_11PushTrigger at 0x0030e710, 100 bytes (weak). Stores the class table (_vt$11PushTrigger) at
// object+72 first, then runs the base destructor _$_10GameObject on the object with flag 0. When bit 0 of the
// incoming flags is set, blockFree__FPv is then called on the object (a tail call in the
// image). Other bits of the flags are not read. Nothing here ran on the EE.
extern "C" {
void base_destroy(void* object, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* object);
extern unsigned char vt_class[] __asm__("_vt$11PushTrigger");
void destroy(void* object, int flags) __asm__("_$_11PushTrigger");
}

extern "C" void destroy(void* object, int flags) {
    unsigned char* const o = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned*>(o + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_class));
    base_destroy(object, 0);
    if (flags & 1) blockFree__FPv(object);
}
