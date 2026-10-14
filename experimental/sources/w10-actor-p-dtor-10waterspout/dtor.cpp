// Observed _$_10WaterSpout at 0x00310948, 100 bytes (weak). Stores the 10WaterSpout table pointer
// (_vt$10WaterSpout) at object+72, calls the GameObject destructor (_$_10GameObject) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 10WaterSpout destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_10WaterSpout[] __asm__("_vt$10WaterSpout");
void base_destructor(void* self, int flags) __asm__("_$_10GameObject");

void blockFree__FPv(void* block);
}

extern "C" void destructor_10WaterSpout(void* self, int flags) __asm__("_$_10WaterSpout");
extern "C" void destructor_10WaterSpout(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10WaterSpout));
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
