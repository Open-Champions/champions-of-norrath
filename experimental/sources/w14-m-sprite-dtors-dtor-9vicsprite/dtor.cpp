// Observed _$_9VICSprite at 0x010b2748, 116 bytes.
// In order: stores _vt$9VICSprite at object+60; calls Clear__9VICSprite(object); calls _$_t7VIArray1Z20VICSpritePartEmitter(object+4616, 2); calls _$_9VIHSprite(object, 0); when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
extern unsigned char d_vt_9VICSprite[] __asm__("_vt$9VICSprite");
void d_Clear__9VICSprite(void* self) __asm__("Clear__9VICSprite");
void d___t7VIArray1Z20VICSpritePartEmitter(void* self, int flags) __asm__("_$_t7VIArray1Z20VICSpritePartEmitter");
void d___9VIHSprite(void* self, int flags) __asm__("_$_9VIHSprite");
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_9VICSprite(void* self, int flags) __asm__("_$_9VICSprite");
extern "C" void destructor_9VICSprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 60, d_vt_9VICSprite);
    d_Clear__9VICSprite(object);
    d___t7VIArray1Z20VICSpritePartEmitter(object + 4616, 2);
    d___9VIHSprite(object, 0);
    if ((flags & 1) != 0) free_object(object);
}
