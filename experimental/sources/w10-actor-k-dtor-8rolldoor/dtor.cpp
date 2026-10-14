// Observed _$_8RollDoor at 0x003119a0, 100 bytes (weak). Stores the class table (_vt$8RollDoor) at
// object+72 first, then runs the base destructor _$_9WorldPart on the object with flag 0. When bit 0 of the
// incoming flags is set, blockFree__FPv is then called on the object (a tail call in the
// image). Other bits of the flags are not read. Nothing here ran on the EE.
extern "C" {
void base_destroy(void* object, int flags) __asm__("_$_9WorldPart");
void blockFree__FPv(void* object);
extern unsigned char vt_class[] __asm__("_vt$8RollDoor");
void destroy(void* object, int flags) __asm__("_$_8RollDoor");
}

extern "C" void destroy(void* object, int flags) {
    unsigned char* const o = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned*>(o + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_class));
    base_destroy(object, 0);
    if (flags & 1) blockFree__FPv(object);
}
