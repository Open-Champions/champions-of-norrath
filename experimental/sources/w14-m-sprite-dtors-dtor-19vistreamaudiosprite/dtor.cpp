// Observed _$_19VIStreamAudioSprite at 0x01018f00, 100 bytes.
// In order: stores _vt$19VIStreamAudioSprite at object+60; calls _$_8VISprite(object, 0); when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
extern unsigned char d_vt_19VIStreamAudioSprite[] __asm__("_vt$19VIStreamAudioSprite");
void d___8VISprite(void* self, int flags) __asm__("_$_8VISprite");
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_19VIStreamAudioSprite(void* self, int flags) __asm__("_$_19VIStreamAudioSprite");
extern "C" void destructor_19VIStreamAudioSprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 60, d_vt_19VIStreamAudioSprite);
    d___8VISprite(object, 0);
    if ((flags & 1) != 0) free_object(object);
}
