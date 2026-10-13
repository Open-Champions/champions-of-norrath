// Observed _$_9WaveMaker at 0x0030e658, 100 bytes (weak). Stores the 9WaveMaker table pointer
// (_vt$9WaveMaker) at object+72, calls the GameObject destructor (_$_10GameObject) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 9WaveMaker destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_9WaveMaker[] __asm__("_vt$9WaveMaker");
void base_destructor(void* self, int flags) __asm__("_$_10GameObject");

void blockFree__FPv(void* block);
}

extern "C" void destructor_9WaveMaker(void* self, int flags) __asm__("_$_9WaveMaker");
extern "C" void destructor_9WaveMaker(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_9WaveMaker));
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
