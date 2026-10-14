// Observed: __12VICollSphere at 0x010a0878 is 20 bytes. It does nothing but pass its argument
// (the object, a0) unchanged to __9VICollRay and finish with a tail jump to it, so the return
// value is whatever that routine returns. It sets up a stack frame and restores it around the jump.
// Inferred: this is the VICollSphere constructor that only runs its base constructor (VICollRay is
// the base by the name pair, not by any layout evidence here). Unknown: any field the sphere adds;
// none is initialised here, which Init__12VICollSphereRC12VICollSphereRC10VIMatrix44 would cover.
extern "C" {
void* __9VICollRay(void* self);

void* __12VICollSphere(void* self) {
    return __9VICollRay(self);
}
}
