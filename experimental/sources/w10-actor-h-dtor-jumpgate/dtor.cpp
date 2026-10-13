// Observed _$_8JumpGate at 0x00284450, 160 bytes (weak).
// Stores the JumpGate table pointer (_vt$8JumpGate) at object+72, calls lightDeactivate__FP5Light on
// the member at object+256, then the FxCreatureState destructor on object+316, the Light destructor on
// object+256 and the AnimationState destructor on object+216 (each with flag 2), then the GameObject
// destructor (_$_10GameObject) on the object with flag 0, then calls blockFree__FPv on the object when
// bit 0 of the incoming flags is set. The free is a tail call in the image.
// Inferred: this is the JumpGate destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_JumpGate[] __asm__("_vt$8JumpGate");
void light_deactivate(void* light) __asm__("lightDeactivate__FP5Light");
void fx_creature_state_destructor(void* self, int flags) __asm__("_$_15FxCreatureState");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void animation_state_destructor(void* self, int flags) __asm__("_$_14AnimationState");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void jumpgate_destructor(void* self, int flags) __asm__("_$_8JumpGate");
extern "C" void jumpgate_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_JumpGate));
    light_deactivate(object + 256);
    fx_creature_state_destructor(object + 316, 2);
    light_destructor(object + 256, 2);
    animation_state_destructor(object + 216, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
