// Observed: SetDirection__17VIParticleEmitterRC10VIMatrix33 at 0x010b8be8 is 28 bytes. It calls the VIMatrix33 assignment operator
// (__as__10VIMatrix33RC10VIMatrix33) with the object at offset 40 of the emitter as the destination and the argument as the source,
// and returns nothing; the operator's result is not used.
// Inferred: +40 holds an embedded 3x3 matrix, the emitter's direction. SetDirection with a VIVect3 is a separate routine.
// Unknown: the size of the embedded matrix (the assignment is not inlined here).
extern "C" {
void* __as__10VIMatrix33RC10VIMatrix33(void* destination, const void* source);

void SetDirection__17VIParticleEmitterRC10VIMatrix33(unsigned char* emitter, const void* matrix) {
    __as__10VIMatrix33RC10VIMatrix33(emitter + 40, matrix);
}
}
