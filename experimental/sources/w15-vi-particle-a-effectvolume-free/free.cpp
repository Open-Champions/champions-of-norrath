// Observed: Free__14VIEffectVolume at 0x0114a208 is 44 bytes. With the object as this it calls DetachAllResources__14VIEffectVolume(this)
// and then FreeParticles__14VIEffectVolume(this). Nothing is stored here and nothing is returned.
// Inferred: the order is detach first, then free particles.
// Unknown: what either callee releases.
extern "C" {
void DetachAllResources__14VIEffectVolume(void* self);
void FreeParticles__14VIEffectVolume(void* self);

void Free__14VIEffectVolume(void* self) {
    DetachAllResources__14VIEffectVolume(self);
    FreeParticles__14VIEffectVolume(self);
}
}
