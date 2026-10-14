// Observed: SetSpace__17VIParticleEmitterRC10VIMatrix44 at 0x010b8b48 is 28 bytes. It calls sceVu0CopyMatrix(emitter + 96, matrix),
// copying the 64-byte matrix argument to offset 96 of the emitter. Nothing else is read or written; nothing is returned.
// Inferred: +96 is the emitter's space matrix (Destroy__17VIParticleEmitter resets it with Identity__10VIMatrix44).
// Unknown: the coordinate space the matrix maps between.
extern "C" {
void sceVu0CopyMatrix(void* destination, const void* source);

void SetSpace__17VIParticleEmitterRC10VIMatrix44(unsigned char* emitter, const void* matrix) {
    sceVu0CopyMatrix(emitter + 96, matrix);
}
}
