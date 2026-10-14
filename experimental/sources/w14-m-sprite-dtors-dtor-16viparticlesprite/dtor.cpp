// Observed _$_16VIParticleSprite at 0x010aa6b0, 116 bytes.
// In order: stores _vt$16VIParticleSprite at object+60; calls _$_17VIParticleEmitter(object+80, 2); calls _$_8VISprite(object, 0); when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
extern unsigned char d_vt_16VIParticleSprite[] __asm__("_vt$16VIParticleSprite");
void d___17VIParticleEmitter(void* self, int flags) __asm__("_$_17VIParticleEmitter");
void d___8VISprite(void* self, int flags) __asm__("_$_8VISprite");
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_16VIParticleSprite(void* self, int flags) __asm__("_$_16VIParticleSprite");
extern "C" void destructor_16VIParticleSprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 60, d_vt_16VIParticleSprite);
    d___17VIParticleEmitter(object + 80, 2);
    d___8VISprite(object, 0);
    if ((flags & 1) != 0) free_object(object);
}
