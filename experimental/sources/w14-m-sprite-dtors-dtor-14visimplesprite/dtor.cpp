// Observed _$_14VISimpleSprite at 0x01166f88, 100 bytes.
// In order: stores _vt$14VISimpleSprite at object+60; calls _$_8VISprite(object, 0); when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
extern unsigned char d_vt_14VISimpleSprite[] __asm__("_vt$14VISimpleSprite");
void d___8VISprite(void* self, int flags) __asm__("_$_8VISprite");
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_14VISimpleSprite(void* self, int flags) __asm__("_$_14VISimpleSprite");
extern "C" void destructor_14VISimpleSprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 60, d_vt_14VISimpleSprite);
    d___8VISprite(object, 0);
    if ((flags & 1) != 0) free_object(object);
}
