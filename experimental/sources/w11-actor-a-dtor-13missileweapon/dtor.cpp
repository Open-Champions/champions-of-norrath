// Observed _$_13MissileWeapon at 0x00145638, 228 bytes. In order: the table pointer _vt$13MissileWeapon (0x449e78)
// goes to object+72; when the word at +364 is non-zero, P_DeleteTrail__Fi runs with it; the Light destructor
// (_$_5Light) runs on object+368 with flag 2; listDestroy__FP8ListHead runs on object+292; then the five
// 24-byte elements that start at object+84 are torn down from the last to the first, each by
// listDestroy__FP8ListHead on element+8 (object+188, 164, 140, 116, 92); then _vt$10Projectile (0x44d378)
// is stored at object+72 and listDestroy runs on object+76; the GameObject destructor (_$_10GameObject) runs
// on the object with flag 0; and blockFree__FPv runs on the object when bit 0 of the incoming flags is set
// (a tail call in the image). Other flag bits do not free.
// Inferred: this is the 13MissileWeapon destructor from its symbol name; the five-element array is read from the
// descending loop with step 24 between object+204 and object+84. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_MissileWeapon[] __asm__("_vt$13MissileWeapon");
extern unsigned char vt_Projectile[] __asm__("_vt$10Projectile");
void P_DeleteTrail__Fi(int trail);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void listDestroy__FP8ListHead(void* head);
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_MissileWeapon(void* self, int flags) __asm__("_$_13MissileWeapon");
extern "C" void destructor_MissileWeapon(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_MissileWeapon));
    const int trail = *reinterpret_cast<int*>(object + 364);
    if (trail != 0) P_DeleteTrail__Fi(trail);
    light_destructor(object + 368, 2);
    listDestroy__FP8ListHead(object + 292);
    for (unsigned char* element = object + 204; element != object + 84;) {
        element -= 24;
        listDestroy__FP8ListHead(element + 8);
    }
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Projectile));
    listDestroy__FP8ListHead(object + 76);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
