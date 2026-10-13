// Observed _$_6Switch at 0x00311b80, 100 bytes (weak). Stores the 6Switch table pointer
// (_vt$6Switch) at object+72, calls the WorldPart destructor (_$_9WorldPart) with flag 0, then
// calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other flag bits do
// not free. The free is a tail call in the image.
// Inferred: this is the 6Switch destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_6Switch[] __asm__("_vt$6Switch");
void worldpart_destructor(void* self, int flags) __asm__("_$_9WorldPart");
void blockFree__FPv(void* block);
}

extern "C" void destructor_6Switch(void* self, int flags) __asm__("_$_6Switch");
extern "C" void destructor_6Switch(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_6Switch));
    worldpart_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
