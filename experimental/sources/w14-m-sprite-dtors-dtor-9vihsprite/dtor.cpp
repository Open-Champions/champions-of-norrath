// Observed _$_9VIHSprite at 0x010633c0, 188 bytes.
// In order: stores _vt$9VIHSprite at object+60; calls Clear__9VIHSprite(object); calls _$_10VIMatrix44(object+320, 2); calls _$_t6VIPool1Z17VIHSpritePlayNode(object+144, 2); calls _$_t6VIPool1Z13VIHSpritePlay(object+120, 2); calls _$_t6VIPool1Z19VIHSpriteAttachment(object+96, 2); calls _$_t7VIArray1Z16VIHSpriteTrigger(object+88, 2); calls _$_t8VIVector1Z10VIMatrix44(object+76, 2); calls _$_t8VIVector1Z13VIHSpriteNode(object+64, 2); calls _$_8VISprite(object, 0); when bit 0 of flags is set, tail-calls __builtin_delete(object).
// Inferred: this is the deleting destructor of the class from its symbol name and the store of its table
// at object+60; the order of the calls is the order in the image. Nothing here was executed on the EE.

extern "C" {
extern unsigned char d_vt_9VIHSprite[] __asm__("_vt$9VIHSprite");
void d_Clear__9VIHSprite(void* self) __asm__("Clear__9VIHSprite");
void d___10VIMatrix44(void* self, int flags) __asm__("_$_10VIMatrix44");
void d___t6VIPool1Z17VIHSpritePlayNode(void* self, int flags) __asm__("_$_t6VIPool1Z17VIHSpritePlayNode");
void d___t6VIPool1Z13VIHSpritePlay(void* self, int flags) __asm__("_$_t6VIPool1Z13VIHSpritePlay");
void d___t6VIPool1Z19VIHSpriteAttachment(void* self, int flags) __asm__("_$_t6VIPool1Z19VIHSpriteAttachment");
void d___t7VIArray1Z16VIHSpriteTrigger(void* self, int flags) __asm__("_$_t7VIArray1Z16VIHSpriteTrigger");
void d___t8VIVector1Z10VIMatrix44(void* self, int flags) __asm__("_$_t8VIVector1Z10VIMatrix44");
void d___t8VIVector1Z13VIHSpriteNode(void* self, int flags) __asm__("_$_t8VIVector1Z13VIHSpriteNode");
void d___8VISprite(void* self, int flags) __asm__("_$_8VISprite");
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void free_object(void* pointer) __asm__("__builtin_delete");

extern "C" void destructor_9VIHSprite(void* self, int flags) __asm__("_$_9VIHSprite");
extern "C" void destructor_9VIHSprite(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 60, d_vt_9VIHSprite);
    d_Clear__9VIHSprite(object);
    d___10VIMatrix44(object + 320, 2);
    d___t6VIPool1Z17VIHSpritePlayNode(object + 144, 2);
    d___t6VIPool1Z13VIHSpritePlay(object + 120, 2);
    d___t6VIPool1Z19VIHSpriteAttachment(object + 96, 2);
    d___t7VIArray1Z16VIHSpriteTrigger(object + 88, 2);
    d___t8VIVector1Z10VIMatrix44(object + 76, 2);
    d___t8VIVector1Z13VIHSpriteNode(object + 64, 2);
    d___8VISprite(object, 0);
    if ((flags & 1) != 0) free_object(object);
}
