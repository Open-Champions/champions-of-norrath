// Shared declarations for the w13-m-spell-dtor-a destructors.
//
// Every name here is an image label that some other unit defines or that the partial link
// leaves open. Nothing in this header defines behaviour. Each destructor in this package
// takes (object, flags): flags bit 0 means "release the storage after the body", and the
// other bits are passed through only where a base or member call needs them.
#ifndef W13_M_SPELL_DTOR_A_COMMON_HPP
#define W13_M_SPELL_DTOR_A_COMMON_HPP

extern "C" {

void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
void lightDeactivate__FP5Light(void* light);
void P_DeleteParticleSystem__Fi(int handle);
void soundStopSound__Fi(int handle);

// Base and member destructors, called with their image labels.
void gameObjectDestructor(void* self, int flags) __asm__("_$_10GameObject");
void lightDestructor(void* self, int flags) __asm__("_$_5Light");
void creatureDestructor(void* self, int flags) __asm__("_$_8Creature");
void missileWeaponDestructor(void* self, int flags) __asm__("_$_13MissileWeapon");
void animationStateDestructor(void* self, int flags) __asm__("_$_14AnimationState");

// The image releases particle-attribute storage with the runtime delete rather than blockFree.
void runtimeDelete(void* block) __asm__("__builtin_delete");

}  // extern "C"

// Writes a 32-bit table address into an object word. The EE build uses 32-bit pointers.
inline void store_table(unsigned char* object, int offset, const void* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

#endif  // W13_M_SPELL_DTOR_A_COMMON_HPP
