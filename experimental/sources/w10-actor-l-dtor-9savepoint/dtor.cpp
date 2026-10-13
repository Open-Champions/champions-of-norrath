// Observed _$_9Savepoint at 0x00311fe0, 124 bytes (weak). Stores the 9Savepoint table pointer (the
// data object at 0x449550, _vt$9Savepoint) at object+72, then calls the destructor _$_5Light on
// object+136 with flag 2; then calls the destructor _$_14AnimationState on object+76 with flag 2;
// then calls the destructor _$_10GameObject on object+0 with flag 0; finally calls blockFree__FPv on
// the object when bit 0 of the incoming flags is set (a tail call in the image). Inferred: this is
// the 9Savepoint destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_449550[];
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void animation_state_destructor(void* self, int flags) __asm__("_$_14AnimationState");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_9savepoint(void* self, int flags) __asm__("_$_9Savepoint");
extern "C" void destructor_9savepoint(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_449550));
    light_destructor(object + 136, 2);
    animation_state_destructor(object + 76, 2);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
