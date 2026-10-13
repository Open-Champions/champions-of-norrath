// Deleting destructor of FireStorm (image label _$_9FireStorm).
//
// Observed in the private image at 0x0029d0f0 (160 bytes): stores the FireStorm table at
// object+72; passes the int at object+164 to soundStopSound, then writes -1 there; calls
// lightDeactivate on the embedded light at object+80; calls the Light destructor on it with
// flag 2; releases the list head at object+76; calls the GameObject destructor with flag 0;
// and releases the object when bit 0 of the incoming flags is set.
// Inferred: the int at +164 is a sound handle, and -1 marks it as no longer playing.
// Unknown: whether soundStopSound is ever given a handle that was already stopped.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_FireStorm[] __asm__("_vt$9FireStorm");
}

extern "C" void destructor_FireStorm(void* self, int flags) __asm__("_$_9FireStorm");
extern "C" void destructor_FireStorm(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_FireStorm);
    soundStopSound__Fi(*reinterpret_cast<int*>(object + 164));
    *reinterpret_cast<int*>(object + 164) = -1;
    lightDeactivate__FP5Light(object + 80);
    lightDestructor(object + 80, 2);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
