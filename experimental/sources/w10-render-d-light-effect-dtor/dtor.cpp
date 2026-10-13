// Observed _$_11LightEffect at 0x003117e0, 124 bytes: the LightEffect destructor, taking the object and a
// flag word. In order: the LightEffect vtable (_vt$11LightEffect, 0x00449a70) is stored at object+72;
// listDestroy__FP8ListHead (0x00194300) runs on object+136; the Light destructor (_$_5Light, 0x0013eac8)
// runs on the embedded light at object+76 with flag 2; the GameObject destructor (_$_10GameObject,
// 0x00141658) runs on the object with flag 0; and when bit 0 of the incoming flag word is set the object
// is released with blockFree__FPv (0x001447d8, a tail call). Nothing is returned. The vtable address
// is taken from 0x00449a70 as loaded by the original (lui 0x45, addiu -26000).
extern "C" {
void listDestroy__FP8ListHead(void* list);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void game_object_destructor(void* object, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
extern unsigned char light_effect_vtable[] __asm__("_vt$11LightEffect");
}

extern "C" void light_effect_destructor(void* self, int flags) __asm__("_$_11LightEffect");
extern "C" void light_effect_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(light_effect_vtable));
    listDestroy__FP8ListHead(object + 136);
    light_destructor(object + 76, 2);
    game_object_destructor(object, 0);
    if (flags & 1) blockFree__FPv(object);
}
